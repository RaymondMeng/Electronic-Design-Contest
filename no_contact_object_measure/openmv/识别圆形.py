THRESHOLD = (0,100) # Grayscale threshold for dark things... (5, 70, -23, 15, -57, 0)(18, 100, 31, -24, -21, 70)
import sensor, image, time
from pyb import LED
from pyb import UART,Timer

#import car
#from pid import PID
#rho_pid = PID(p=0.4, i=0)
#theta_pid = PID(p=0.001, i=0)

uart = UART(3,921600)#初始化串口 波特率 115200

#LED(1).on()
#LED(2).on()
#LED(3).on()
sensor.reset()
#sensor.set_vflip(True)
#sensor.set_hmirror(True)
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA) # 80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
#sensor.set_windowing([0,20,80,40])
sensor.skip_frames(time = 2000)     # WARNING: If you use QQVGA it may take seconds
clock = time.clock()                # to process a frame sometimes.

up_roi   = [0,   0, 80, 15]#上采样区0
down_roi = [0, 55, 80, 15]#下采样区0
#middle_roi = []
left_roi = [0,   0, 25, 60]#左采样区0
righ_roi = [55, 0,  25, 40]#右采样区0

thresholds = [0, 90]#自定义灰度阈值




class Dot(object):
    x = 0
    y = 0
    pixels = 0
    num = 0
    ok = 0
    flag = 0

class Line(Dot):
    x_angle = 0
    y_angle = 0
    #flag = 0

class singleline_check():
    flager = 0
    rho_err = 0
    theta_err = 0

class receive(object):
    uart_buf = []
    _data_len = 0
    _data_cnt = 0
    state = 0
R=receive()

class ctrl(object):
    work_mode = 0x02 #工作模式.默认是点检测，可以通过串口设置成其他模式
    check_show = 1   #开显示，在线调试时可以打开，离线使用请关闭，可提高计算速度

ctr=ctrl()

dot  = Dot()
up   = Line()
down = Line()
left = Line()
righ = Line()
line = Line()
singleline_check = singleline_check()
#line.flag = 0

timer_update_flag=0

timer = Timer(4, freq=20) #定时器初始化20Hz,50ms



# func会自动接收timer这个对象
def func(timer):
    global timer_update_flag
    #print(timer.counter())
    # 定义回调函数
    timer_update_flag=1

timer.callback(func)



#线检测数据打包
def pack_line_data():

    pack_data=bytearray([0xFF,0xFE,0xF3,0x00,
        singleline_check.rho_err>>8,singleline_check.rho_err,
        singleline_check.theta_err>>8,singleline_check.theta_err,
        line.flag,0x00,0x00,0x00])
    singleline_check.rho_err = 0
    singleline_check.theta_err = 0

    lens = len(pack_data)#数据包大小
    pack_data[3] = lens-5#有效数据个数

    i = 0
    sum = 0

    #和校验
    while i<(lens-1):
        sum = sum + pack_data[i]
        i = i+1
    pack_data[lens-1] = sum

    return pack_data

#点检测数据打包
def pack_dot_data():
    pack_data=bytearray([0xFF,0xFE,0xF2,0x00,
        dot.x>>8,dot.x,
        dot.y>>8,dot.y,dot.num>>8,dot.num,
        dot.flag,0x00])

    dot.x = 0
    dot.y = 0

    lens = len(pack_data)#数据包大小
    pack_data[3] = lens-5;#有效数据个数

    i = 0
    sum = 0

    #和校验
    while i<(lens-1):
        sum = sum + pack_data[i]
        i = i+1
    pack_data[lens-1] = sum

    return pack_data


#串口数据解析
def Receive_Anl(data_buf,num):

    #和校验
    sum = 0
    i = 0
    while i<(num-1):
        sum = sum + data_buf[i]
        i = i + 1

    sum = sum%256 #求余
    if sum != data_buf[num-1]:
        return
    #和校验通过

    #if data_buf[2]==0x01:
    #    print("receive 1 ok!")

    #if data_buf[2]==0x02:
    #   print("receive 2 ok!")

    if data_buf[2]==0xFC:

        #设置模块工作模式
        ctr.work_mode = data_buf[4]

        #print("Set work mode success!")


#串口通信协议接收
def Receive_Prepare(data):

    if R.state==0:

        if data == 0xAA:#帧头
            R.state = 1
            R.uart_buf.append(data)
        else:
            R.state = 0

    elif R.state==1:
        if data == 0xAF:#帧头
            R.state = 2
            R.uart_buf.append(data)
        else:
            R.state = 0

    elif R.state==2:
        if data <= 0xFF:#控制字
            R.state = 3
            R.uart_buf.append(data)
        else:
            R.state = 0

    elif R.state==3:#数据个数
        if data <= 33:
            R.state = 4
            R.uart_buf.append(data)
            R._data_len = data
            R._data_cnt = 0
        else:
            R.state = 0

    elif R.state==4:
        if R._data_len > 0:
            R. _data_len = R._data_len - 1
            R.uart_buf.append(data)
            if R._data_len == 0:
                R.state = 5
        else:
            R.state = 0

    elif R.state==5:
        R.state = 0
        R.uart_buf.append(data)
        Receive_Anl(R.uart_buf,R.uart_buf[3]+5)
        R.uart_buf=[]#清空缓冲区，准备下次接收数据
    else:
        R.state = 0

#读取串口缓存
def uart_read_buf():
    i = 0
    buf_size = uart.any()
    while i<buf_size:
        Receive_Prepare(uart.readchar())
        i = i + 1
#点检测函数
def check_dot(img):
    for blob in img.find_blobs([thresholds], pixels_threshold=3, area_threshold=3, merge=True, margin=5):
        if dot.pixels<blob.pixels():#寻找最大的黑点
            dot.pixels=blob.pixels()
            dot.x = blob.cx()
            dot.y = blob.cy()
            dot.ok= 1
            img.draw_cross(dot.x, dot.y, color=127, size = 10)
            img.draw_circle(dot.x, dot.y, 5, color = 127)

    #判断标志位 赋值像素点数据
    dot.flag = dot.ok
    dot.num = dot.pixels

    #清零标志位
    dot.pixels = 0
    dot.ok = 0
    global timer_update_flag
    #发送数据
    if timer_update_flag:
        uart.write(pack_dot_data())
        #timer_update_flag=0

def fine_border(img,area,area_roi):

    line = img.get_regression([(255,255)],roi=area_roi, robust = True)
    if (line):
        area.ok=1
    #判断标志位
    dot.flag = dot.ok
    #清零标志位

    #dot.pixels = 0
    dot.ok = 0

#找线
def found_line(img):
    singleline_check.flager = img.get_regression([(255,255)], robust = True)
    if (singleline_check.flager):
        #print(clock.fps())
        singleline_check.rho_err = abs(singleline_check.flager.rho())-0
        if singleline_check.flager.theta()>90:
            singleline_check.theta_err = singleline_check.flager.theta()-0
        else:
            singleline_check.theta_err = singleline_check.flager.theta()-0
        img.draw_line(singleline_check.flager.line(), color = 127)
        #print(singleline_check.theta_err)


def check_line(img):
    fine_border(img,up,up_roi)
    fine_border(img,down,down_roi)
    fine_border(img,left,left_roi)
    fine_border(img,righ,righ_roi)

    line.flag = 0
    if up.ok:
        line.flag = line.flag | 0x01
    if down.ok:
        line.flag = line.flag | 0x02
    if left.ok:
        line.flag = line.flag | 0x04
    if righ.ok:
        line.flag = line.flag | 0x08
    #print(line.flag)

    found_line(img)

    up.ok = down.ok = left.ok = righ.ok = 0
    up.num = down.num = left.num = righ.num = 0
    up.pixels = down.pixels = left.pixels = righ.pixels = 0

    #发送数据
    global timer_update_flag
    #发送数据
    if timer_update_flag:
        uart.write(pack_line_data())
        #timer_update_flag=0





while(True):
    clock.tick()

    #img = sensor.snapshot().binary([THRESHOLD])
    #img = sensor.snapshot()

    if (ctr.work_mode&0x01)!=0:
        img = sensor.snapshot().lens_corr(1.8)
        check_dot(img)
        #LED(1).on()           #亮灯
        #time.sleep(10)     #延时150ms
        #LED(1).off()
        #time.sleep(10)     #延时150ms

    #线检测
    if (ctr.work_mode&0x02)!=0:
        img = sensor.snapshot()
        for c in img.find_circles(threshold = 7800, x_margin = 10, y_margin = 10, r_margin = 10,r_min = 2, r_max = 100, r_step = 2):
            print(c.x())
            img.draw_cross(int(c.x()), int(c.y()), color = (255, 255, 255), size = 10, thickness = 2)
            #check_line(img)
        #LED(3).on()        #亮灯
        #time.sleep(10)     #延时150ms
        #LED(3).off()

    #接收串口数据
    uart_read_buf()
    #found_line(img)
    #print(clock.fps())

