#2020 年 TI 杯大学生电子设计竞赛非接触物体尺寸形态测量（G 题） openmv测试代码
# author:mc
# school:whut
import sensor, image, time
import math
from pyb import UART
import json
import ustruct
sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # or sensor.RGB565  565
sensor.set_framesize(sensor.QQVGA) # or sensor.QVGA (or others)
sensor.skip_frames(50)             #跳过50frame，使新设置生效,并自动调节白平衡
sensor.set_auto_gain(False)         #关闭自动自动增益。默认开启的，在颜色识别中，一定要关闭白平衡。
sensor.set_auto_whitebal(False)     #关闭白平衡。白平衡是默认开启的，在颜色识别中，一定要关闭白平衡。



#参数初始化
fps_cnt=0
fps_cnt_set=10000
show_fps_set=15
target_x=0
target_y=0
shape = 0
graylight_x=0
graylight_y=0
distance=0
PI=3.14159
#防误判变量
now_length = 0
last_length = 0
exist_times = 0
sum_length = 0
last_center_X = 0
last_center_Y = 0

tri_times=0
rect_times=0
circle_times=0
xy_times=0
xy_not_times=0




#测量色块像素数
class Circle:
    def __init__(self):  #_init_(self),主要给类初始化一些属性，对象创建时自动调用该方法执行
        self.left_pixelsLimit = 3000
        self.right_pixelsLimit = 50000
        self.left_SolidtyLimit = 0.37
        self.right_SolidtyLimit = 0.85
        self.left_RoundnessLimit = 0.30
        self.right_RoundnessLimit = 1
class Triangle:
    def __init__(self):  #_init_(self),主要给类初始化一些属性，对象创建时自动调用该方法执行
        self.left_pixelsLimit = 218
        self.right_pixelsLimit = 4583
        self.left_SolidtyLimit = 0.2
        self.right_SolidtyLimit = 0.94
        self.left_RoundnessLimit = 0.5
        self.right_RoundnessLimit = 0.99
class Rectangle:
    def __init__(self):  #_init_(self),主要给类初始化一些属性，对象创建时自动调用该方法执行
        self.left_pixelsLimit = 380
        self.right_pixelsLimit = 5323
        self.left_SolidtyLimit = 0.80
        self.right_SolidtyLimit = 1
        self.left_RoundnessLimit = 0.19
        self.right_RoundnessLimit = 1

MyCircle = Circle()
MyTriangle = Triangle()
MyRectangle = Rectangle()

#激光阈值
#graylight_thred=(230,255)

#色块阈值  首先测颜色阈值
#(88,-79,81)
green_threshold = (57,95,-80,-20,-25,32)  #(57,95,-80,-20,-25,32)
red_threshold = (3, 100, -68, 28, -45, 46)   #(21,71,56,124,38,114)
blue_threshold = (0, 95, -60, 98, -29, 85)   #(20,73,-46,112,-128,-31)

#football_threshold = (0,99,-66,118,16,127)
#white_black_football_threshold= (0,27,-127,127,-127,127)
#vollyball_threshold = (0,99,-66,118,25,127)
#vollyball_threshold_blue = (0,80,-20,37,30,70)
#basketball_threshold = (0,99,14,127,7,64)
#这里不需要测量激光只需测量画面中心即可
#获取激光坐标函数
'''
def get_graylight(grayMat,paint):
   #global real_exist_times
   #mask1=grayMat.binary([graylight_thred])
   #计算连通域
   center_X=0
   center_Y=0
   for blob in grayMat.find_blobs([graylight_thred], pixels_threshold=0, area_threshold=0, merge=True, margin=0):
    pixels=blob.pixels()
    Roundness= blob.roundness()
    if(fps_cnt%show_fps_set==0):
        print("圆形度",Roundness)
        print("像素个数",pixels)
    #光亮像素过大或者圆形度过小视作无
    if pixels>=60 :
        center_X=0
        center_Y=0
    else:
        center_X=blob.cx()
        center_Y=blob.cy()
        #paint.draw_circle(center_X,center_Y,4, color =0, thickness = 3, fill = True)
   return (center_X,center_Y)
'''
#获取形状、目标像素长度、目标中心坐标函数
def get_shapeANDcolor(srcImg,color_thred):
   center_X=0
   center_Y=0
   shape = 0
   target_length=0
   for blob in srcImg.find_blobs([color_thred], pixels_threshold=10, area_threshold=20, merge=True, margin=0):
    pixels=blob.pixels()       #获取像素个数，用来判断大小
    print(pixels)
    #首先用像素限制掉杂点
    if(pixels >= MyTriangle.left_pixelsLimit):
        Roundness= blob.roundness()#获取连通域圆形度
        Solidty = blob.solidity()  #获取连通域与它最小外接矩形的密度比
        if(fps_cnt%show_fps_set==0):
            print("pixels",pixels)
            print("Roundness",Roundness)
            print("Solidty",Solidty)

        #条件限制
        if(pixels<=MyTriangle.right_pixelsLimit and Solidty>=MyTriangle.left_SolidtyLimit and Solidty<=MyTriangle.right_SolidtyLimit and Roundness>=MyTriangle.left_RoundnessLimit and Roundness<=MyTriangle.right_RoundnessLimit):
            srcImg.draw_rectangle(blob[0:4],color = (0, 0, 255)) #用矩形标记出目标颜色区域
            srcImg.draw_cross(blob[5], blob[6]) #在目标颜色区域的中心画十字形标记
            center_X=blob.cx()
            center_Y=blob.cy()
            shape = 1
            target_length = pixels
            #print("三角形")
        elif(pixels<=MyRectangle.right_pixelsLimit and Solidty>=MyRectangle.left_SolidtyLimit and Solidty<=MyRectangle.right_SolidtyLimit and Roundness>=MyRectangle.left_RoundnessLimit and Roundness<=MyRectangle.right_RoundnessLimit):
            srcImg.draw_rectangle(blob[0:4],color = (0, 255, 0)) #用矩形标记出目标颜色区域
            srcImg.draw_cross(blob[5], blob[6]) #在目标颜色区域的中心画十字形标记
            center_X=blob.cx()
            center_Y=blob.cy()
            shape = 3
            target_length =pixels
            #print("矩形")

        else:
            center_X=0
            center_Y=0
            shape = 0
            target_length = 0
   return (shape,center_X,center_Y,target_length)
#获取最大可能色块函数
def get_maxsize_color(srcImg):
   R_MAXpixels=0
   G_MAXpixels=0
   B_MAXpixels=0
   #寻找red最大连通域
   for blob in srcImg.find_blobs([red_threshold], pixels_threshold=10, area_threshold=20, merge=True, margin=0):
       pixels=blob.pixels()       #获取像素个数，用来判断大小
       R_MAXpixels = max(pixels,R_MAXpixels)
   #寻找green最大连通域
   for blob in srcImg.find_blobs([green_threshold], pixels_threshold=10, area_threshold=20, merge=True, margin=0):
       pixels=blob.pixels()       #获取像素个数，用来判断大小
       G_MAXpixels = max(pixels,G_MAXpixels)
   #寻找blue最大连通域
   for blob in srcImg.find_blobs([blue_threshold], pixels_threshold=10, area_threshold=20, merge=True, margin=0):
       pixels=blob.pixels()       #获取像素个数，用来判断大小
       B_MAXpixels = max(pixels,B_MAXpixels)
   maxpixels=max(R_MAXpixels,G_MAXpixels,B_MAXpixels)
   if(R_MAXpixels==maxpixels):
       return red_threshold
   elif(G_MAXpixels==maxpixels):
       return green_threshold
   else:
       return blue_threshold
#获取背景板左右边界函数
'''
def get_leftANDrightLINE(srcImg,center_X,center_Y):
    grayimg =srcimg.to_grayscale()
    canny_thred=13
    cannyimg=grayimg.find_edges(image.EDGE_CANNY, threshold=(canny_thred,canny_thred*2))
    cannyimg.dilate(1)
    centerx = center_X
    centery = center_Y
    flag =0
    right_x = 160
    left_x = 0
    for i in range(centerx,cannyimg.width()-1):
        if(cannyimg.get_pixel(i,centery)==0 and cannyimg.get_pixel(i + 1,centery)==255):
            flag+=1
            if(flag == 1):
                right_x =  i
                break
    flag =0
    for i in range(centerx,1,-1):
        if(cannyimg.get_pixel(i,centery)==0 and cannyimg.get_pixel(i - 1,centery)==255):
            flag+=1
            if(flag == 1):
                left_x =  i
                break
    if(fps_cnt%show_fps_set==0):
        print(right_x)
        print(left_x)
    return (left_x,right_x)
'''
#防误判函数，目标的尺寸,并且将值限制在30到40之间
def Prevent_misslength(now_length,last_length):
    global exist_times
    global sum_length
    if last_length==0 and now_length==0:
        sum_length=0
    if abs(last_length-now_length)<=7 and now_length!=0 and last_length!=0:
        sum_length+=now_length
        exist_times+=1
    else:
        sum_length=0
        exist_times=0
    if(exist_times!=0):
        result=sum_length/exist_times
        if(result>=40):
            result=40
        elif(result<=30):
            result=30
        return result
    else:
        return 0
#防误判函数，中心坐标
def Prevent_misstarget(point1_X,point1_Y,lastpoint1_X,lastpoint1_Y):
    global xy_times
    global xy_not_times
    if point1_X==0 and point1_Y==0:
        xy_not_times+=1
    if point1_X==0 and point1_Y==0 and lastpoint1_X!=0 and lastpoint1_Y!=0:
        point1_X=lastpoint1_X
        point1_Y=lastpoint1_Y
    if lastpoint1_X!=0 and lastpoint1_Y!=0 and (abs(lastpoint1_X-point1_X)>=20 or abs(lastpoint1_Y-point1_Y)>=20):
        point1_X=lastpoint1_X
        point1_Y=lastpoint1_Y
    if xy_not_times>=10:
        point1_X=0
        point1_Y=0
        xy_not_times=0
    return point1_X,point1_Y

#白色足球检测函数
'''
def detect_whiteANDblack_football(srcimg):
    img_new = srcimg.copy()
    binimg=img_new.binary([white_black_football_threshold])
    listX = []
    listY = []
    point_nums=0
    for blob in binimg.find_blobs([(1,255)], pixels_threshold=5, area_threshold=5, merge=True, margin=2):
        pixels=blob.pixels()       #获取像素个数，用来判断大小
        Solidity = blob.solidity()
        #if(pixels>=4 and pixels<=48):
            #print(Solidity)
            #print(blob.w()/blob.h())
        #print(pixels)
        if(pixels>=4 and pixels<=48 and Solidity>=0.75  and 0.8<=blob.w()/blob.h()<=1.3 and 20<=blob.cy()<=100):
            listX.append(blob.cx())
            listY.append(blob.cy())
            #srcimg.draw_rectangle(blob[0:4], color = (255, 0, 0)) #用矩形标记出目标颜色区域
            #srcimg.draw_cross(blob[5], blob[6]) #在目标颜色区域的中心画十字形标记
    mean_X=0
    mean_Y=0
    if(len(listX)>=2):
        #求密度最大的点
        while(len(listX)>1):
            #求mean_x,mean_y
            mean_X = Get_Average(listX)
            mean_Y = Get_Average(listY)
            #找到点集中距离mean_X,mean_Y最大的点，并将它去除
            max_distance=0
            distance=0
            max_i=0
            for i in range(len(listX)):
                distance = (listX[i]-mean_X)*(listX[i]-mean_X)+(listY[i]-mean_Y)*(listY[i]-mean_Y)
                if distance>=max_distance:
                   max_i=i
            del listX[max_i]
            del listY[max_i]
        #剩下点是球点
        mean_X = Get_Average(listX)
        mean_Y = Get_Average(listY)
    #srcimg.draw_circle(mean_X,mean_Y,10, color = (255, 0, 0))
    return (mean_X,mean_Y,len(listX))
'''
#获取平均数
def Get_Average(list):
   sum = 0
   for item in list:
      sum += item
   if(len(list)==0):
    return 0
   return int(sum/len(list))
#模型为简化后的模型
'''
def get_real_length(pixels,distance,color):
   reallength = 0
   if(color==1):
       #蓝色k
       #0.3001   -2.0901    4.0956
       ratio = 0.3001 * distance * distance -2.0901 * distance +4.0956
       reallength = pixels/ratio
       #reallength =reallength*0.8
   elif(color==2):
       #红色k
       #0.4336   -2.8160    5.1373
       ratio = 0.4336 * distance * distance -2.8160 * distance +5.1373
       reallength = pixels/ratio
       #reallength =reallength*1.105
   elif(color==3):
       #绿色k
       #0.4364   -2.8062    5.0621
       ratio = 0.4364 * distance * distance -2.8062 * distance +5.0621
       reallength = pixels/ratio
       #reallength =reallength*0.99
   else:
       reallength=0
   return reallength
   '''
#初始化时钟
clock = time.clock() # Tracks FPS.

uart = UART(3,115200)   #定义串口3变量
#设置串口
uart.init(115200, bits=8, parity=None, stop=0) # init with given parameters

#我们要传送的数据有:形状、几何中心坐标 以及大小
def sending_data(shap,cx,cy,leng,color): #发送函数
    pack_data = bytearray([0xff,shape,int(cx),int(cy),int(leng)>>8,int(leng)&0xff,color,0xfe])
    return pack_data

#主要接受的是模式
def recive_data():   #接收函数
    global uart
    global mode
    if uart.any():
        #接收字节串
        #tmp_data = uart.readline()
        #接收字符串
        tmp_data = uart.readline().decode()
        #如果想把回车换行去除掉，
        #tmp_data = uart.readline().decode().strip()
        #将形式为D=0.604m,90#的字符串转换为float类型的数据
        #new_data = tmp_data[2,7]
        #print(tmp_data)
        #获取mode编号
        length = len(tmp_data)
        if( 2<=length<=4 and  tmp_data[0]=='Q'):
            mode = tmp_data[length-1]
            #print(mode)
    return mode

#图像循环
while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    #【step 1:】获取去畸变彩图，这里采用的无畸变摄像头
    srcimg = sensor.snapshot().lens_corr(strength = 1.8, zoom = 1.0)
    fps_cnt=fps_cnt+1
    #flag=0时没有圆
    flag=0
    #初始化参数
    now_length = 0
    circle_r=0
    circle_x=0
    circle_y=0
    center_X=0
    center_Y=0
    shape=0
    #left_line = 0
    #right_line = 160
    target_length = 0
    #黑白足球黑块初始化
    black_nums=0
    color_flag=0        #蓝1 红2 绿3
    #distance = recive_distance()
    mode = 1
    #mode='C'
    real_target_length=0
    #flag_football=0：检测黄色足球 flag_football=1：检测黑白足球
    flag_football = 1
    #mode1是球类检测
    #mode0是几何物体检测
    #print(mode)
    target_pixels=0
    if(mode == 1):
        #观察是否有圆
        for c in srcimg.find_circles(threshold = 2700, x_margin = 10, y_margin = 10, r_margin = 10,r_min = 5, r_max = 30, r_step = 2):
           #后期调整

           area = (c.x()-c.r(), c.y()-c.r(), 2*c.r(), 2*c.r())
           #area为识别到的圆的区域，即圆的外接矩形框
           statistics = srcimg.get_statistics(roi=area)#像素颜色统计
           print(statistics)
           if(-73<statistics.a_mode()<-18 and 9<statistics.b_mode()<59):
            color_flag = 3
           elif(17<statistics.a_mode()<124 and -30<statistics.b_mode()<70):
            color_flag = 2
           elif(-54<statistics.a_mode()<105 and -80<statistics.b_mode()<-14):
            color_flag = 1
           if 1<=color_flag<=3:
               #找到最大的圆作为我们的目标圆，很重要
               flag = 1

           if(c.r()>=circle_r):
               circle_r = c.r()
               circle_x = c.x()
               circle_y = c.y()
                   #if(fps_cnt%show_fps_set==0):
                       #print(c)
           srcimg.draw_rectangle((circle_x-circle_r, circle_y-circle_r, 2*circle_r, 2*circle_r), color=(255, 0, 0))
        #为圆

        if(flag == 1 ):
           center_X = circle_x
           center_Y = circle_y
           target_pixels = PI*circle_r*circle_r
           shape = 2
           #print("圆")
        #不为圆，进行其他检测
        else:
           #首先确定色块颜色
           color_thred = get_maxsize_color(srcimg)
           if(color_thred==green_threshold):
            color_flag = 3
           elif(color_thred==blue_threshold):
            color_flag = 1
           elif(color_thred==red_threshold):
            color_flag = 2
           #print(color_flag)
           #color_thred=red_threshold
           #在该色域中寻找特征，然后得到形状和中心坐标
           (shape,center_X,center_Y,target_pixels) = get_shapeANDcolor(srcimg,color_thred)
        uart.write(sending_data(shape, center_X, center_Y, target_pixels, color_flag))
        #print(sending_data(shape, center_X, center_Y, target_pixels, color_flag))
    '''
    else:
        #先找篮球
        ball_pixels = 0
        for blob in srcimg.find_blobs([basketball_threshold], pixels_threshold=10, area_threshold=20, merge=True, margin=2):
            pixels=blob.pixels()       #获取像素个数，用来判断大小
            #print(pixels)
            #print(blob.w()/blob.h())
            #print(blob.h())
            if(pixels>=200 and pixels<=780 and 0.87<=blob.w()/blob.h()<=1.15 and 12<=blob.w()<=40 and 12<=blob.h()<=40 and 20<=blob.cy()<=100):
                if(pixels>=ball_pixels):
                    ball_pixels=pixels
                    srcimg.draw_rectangle(blob[0:4]) #用矩形标记出目标颜色区域
                    #srcimg.draw_cross(blob[5], blob[6]) #在目标颜色区域的中心画十字形标记
                    shape=4
                    center_X = blob.cx()
                    center_Y = blob.cy()
        ball_pixels=0
        if(shape==0):
            #再找足球
            #if(mode=='C'):
                #第一种，纯黄足球
            for blob in srcimg.find_blobs([vollyball_threshold], pixels_threshold=10, area_threshold=20, merge=True, margin=2):
                pixels=blob.pixels()       #获取像素个数，用来判断大小
                Roundness = blob.roundness()
                #print(pixels)
                #print(Roundness)
                #print(pixels)
                if(pixels>=120 and pixels<=600 and Roundness>=0.75 and 3<=blob.w()<=40 and 3<=blob.h()<=40 and 20<=blob.cy()<=100):
                    if(pixels>=ball_pixels):
                        srcimg.draw_rectangle(blob[0:4]) #用矩形标记出目标颜色区域
                        #srcimg.draw_cross(blob[5], blob[6]) #在目标颜色区域的中心画十字形标记
                        shape=5
                        center_X = blob.cx()
                        center_Y = blob.cy()
            #else:
            if(shape==0):
                #第二种，黑白足球
                (center_X,center_Y,black_nums) = detect_whiteANDblack_football(srcimg)
                if(center_X!=0 and center_Y!=0):
                    shape=5
        ball_pixels=0
        if(shape==0 and black_nums<=2):
            img_new = srcimg.copy()
            binimg=img_new.binary([vollyball_threshold,vollyball_threshold_blue])
            dilateimg = binimg.dilate(3)
            dilateimg = binimg.erode(2)
            #再找排球
            for blob in dilateimg.find_blobs([(1,255)], pixels_threshold=10, area_threshold=20, merge=True, margin=2):
                pixels=blob.pixels()       #获取像素个数，用来判断大小
                #print(pixels)
                #print(blob.w()/blob.h())
                #print(pixels))
                if(pixels>=110 and pixels<=500 and 3<=blob.w()<=40 and 3<=blob.h()<=40 and 0.7<=blob.w()/blob.h()<=1.3 and 20<=blob.cy()<=100 and 20<=blob.cx()<=140):
                    if(pixels>=ball_pixels):
                        srcimg.draw_rectangle(blob[0:4], color = (255, 0, 0)) #用矩形标记出目标颜色区域
                        #srcimg.draw_cross(blob[5], blob[6]) #在目标颜色区域的中心画十字形标记
                        shape=6
                        center_X = blob.cx()
                        center_Y = blob.cy()
        #else:
            #print("NONE")
            '''

    #计算尺寸
    if(mode == 2):
        real_target_length = get_real_length(target_pixels,distance,color_flag)
        if(shape == 1):
            real_target_length = math.sqrt(real_target_length*2.31)
        elif(shape == 2):
            real_target_length = math.sqrt(real_target_length*1.273)
        elif(shape == 3):
            real_target_length = math.sqrt(real_target_length)
        #形状防误判
    #shapeList 对应：无形状、三角形、圆形、正方形、篮球、足球、排球
    srcimg.draw_cross(80, 60,color = (0, 0, 0))
    shape_result=shape
    shapeList = [0,0,0,0,0,0,0]
    shapeList[shape]+=1
    if(fps_cnt%5==0):
        shape_result=shapeList.index(max(shapeList))
        shapeList = [0,0,0,0,0,0,0]
        #中心防误判措施
        #(center_X,center_Y)=Prevent_misstarget(center_X,center_Y,last_center_X,last_center_Y)
        srcimg.draw_cross(center_X, center_Y)
        #srcimg.draw_cross(center_X,center_Y,color = (0, 255, 0))
        #print(shape)
        #print(center_X,center_Y)
        #print(real_target_length)
        #sending_data(int(real_target_length),shape_result,center_X,center_Y,int(distance),int(distance*100%100),color_flag)
        last_center_X = center_X
        last_center_Y = center_Y
