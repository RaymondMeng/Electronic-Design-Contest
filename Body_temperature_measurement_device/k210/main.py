import sensor
import image
import lcd
import KPU as kpu
import time
from Maix import FPIOA, GPIO
import gc
from fpioa_manager import fm
from board import board_info
from machine import UART

global state
state = 0

classes = ["no_mask", "mask"]

#定义串口引脚
fm.register(25, fm.fpioa.UART1_TX)
fm.register(24, fm.fpioa.UART1_RX)

uart_A = UART(UART.UART1, 115200, 8, None, 1, timeout=1000, read_buf_len=4096)

write_buf = 0x00 #0：无

#KPU加载路径
task_fd = kpu.load("/sd/1.smodel")
task_ld = kpu.load("/sd/2.smodel")
task_fe = kpu.load("/sd/3.smodel")


clock = time.clock()

fm.register(board_info.BOOT_KEY, fm.fpioa.GPIOHS0)
key_gpio = GPIO(GPIO.GPIOHS0, GPIO.IN)
start_processing = False


BOUNCE_PROTECTION = 50


def set_key_state(*_):
    global start_processing
    start_processing = True
    time.sleep_ms(BOUNCE_PROTECTION)

key_gpio.irq(set_key_state, GPIO.IRQ_RISING, GPIO.WAKEUP_NOT_SUPPORT)




fm.register(6, fm.fpioa.GPIOHS6)
mask_gpio = GPIO(GPIO.GPIOHS6, GPIO.IN, GPIO.PULL_DOWN)



lcd.init()
#lcd.rotation(2)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(1)
sensor.set_vflip(1)
sensor.run(1)
anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437,
          6.92275, 6.718375, 9.01025)  # anchor for face detect
dst_point = [(44, 59), (84, 59), (64, 82), (47, 105),
             (81, 105)]  # standard face key point position
a = kpu.init_yolo2(task_fd, 0.5, 0.3, 5, anchor)
img_lcd = image.Image()
img_face = image.Image(size=(128, 128))
a = img_face.pix_to_ai()
record_ftr = []
record_ftrs = []
names = ['Mr.1', 'Mr.2', 'Mr.3', 'Mr.4', 'Mr.5',
         'Mr.6', 'Mr.7', 'Mr.8', 'Mr.9', 'Mr.10']


ACCURACY = 85

while (1):
    io_state = mask_gpio.value()
    if io_state == 1 and state == 0:
        state = 1
<<<<<<< HEAD


=======
>>>>>>> e6ea875e31731b71cfdbfe7cb254347ff31d0cb9

        kpu.deinit(task_fd)
        kpu.deinit(task_ld)
        kpu.deinit(task_fe)
        del task_fd
        del task_ld
        del task_fe
        gc.collect()

        lcd.init()
<<<<<<< HEAD
=======
        lcd.rotation(2)
>>>>>>> e6ea875e31731b71cfdbfe7cb254347ff31d0cb9
        sensor.reset()
        sensor.set_pixformat(sensor.RGB565)
        sensor.set_framesize(sensor.VGA)
        sensor.set_windowing((320, 256))
        sensor.set_vflip(1)
        sensor.set_hmirror(1)

        task = kpu.load("/sd/mask.kmodel")
        anchor = (0.156250, 0.222548,0.361328, 0.489583,0.781250, 0.983133,1.621094, 1.964286,3.574219, 3.940000)
        kpu.init_yolo2(task, 0.3, 0.3, 5, anchor)

    if state == 0:
        img = sensor.snapshot()
        img.rotation_corr(x_rotation=0,y_rotation=180) #旋转y轴180°
        clock.tick()
        code = kpu.run_yolo2(task_fd, img)
        if code:
            for i in code:
                # Cut face and resize to 128x128
                a = img.draw_rectangle(i.rect())
                face_cut = img.cut(i.x(), i.y(), i.w(), i.h())
                face_cut_128 = face_cut.resize(128, 128)
                a = face_cut_128.pix_to_ai()
                # a = img.draw_image(face_cut_128, (0,0))
                # Landmark for face 5 points
                fmap = kpu.forward(task_ld, face_cut_128)
                plist = fmap[:]
                le = (i.x() + int(plist[0] * i.w() - 10), i.y() + int(plist[1] * i.h()))
                re = (i.x() + int(plist[2] * i.w()), i.y() + int(plist[3] * i.h()))
                nose = (i.x() + int(plist[4] * i.w()), i.y() + int(plist[5] * i.h()))
                lm = (i.x() + int(plist[6] * i.w()), i.y() + int(plist[7] * i.h()))
                rm = (i.x() + int(plist[8] * i.w()), i.y() + int(plist[9] * i.h()))
                a = img.draw_circle(le[0], le[1], 4)
                a = img.draw_circle(re[0], re[1], 4)
                a = img.draw_circle(nose[0], nose[1], 4)
                a = img.draw_circle(lm[0], lm[1], 4)
                a = img.draw_circle(rm[0], rm[1], 4)
                # align face to standard position
                src_point = [le, re, nose, lm, rm]
                T = image.get_affine_transform(src_point, dst_point)
                a = image.warp_affine_ai(img, img_face, T)
                a = img_face.ai_to_pix()
                # a = img.draw_image(img_face, (128,0))
                del (face_cut_128)
                # calculate face feature vector
                fmap = kpu.forward(task_fe, img_face)
                feature = kpu.face_encode(fmap[:])
                reg_flag = False
                scores = []
                for j in range(len(record_ftrs)):
                    score = kpu.face_compare(record_ftrs[j], feature)
                    scores.append(score)
                max_score = 0
                index = 0
                for k in range(len(scores)):
                    if max_score < scores[k]:
                        max_score = scores[k]
                        index = k
                if max_score > ACCURACY: #如果得分大于阈值则设定为真
                    a = img.draw_string(i.x(), i.y(), ("%s :%2.1f" % (
                        names[index], max_score)), color=(0, 255, 0), scale=2) #绿线
                    if index >= 0 and index <= 2:
                        num = int(1)
                    elif index >= 3 and index <= 5:
                        num = int(2)
                    elif index >= 6 and index <= 9:
                        num = int(3)
                    print(index)

                    score = int(max_score)
                    pack_data = bytearray([0xff,num,score,0xfe])
                    uart_A.write(pack_data)
                else:
                    a = img.draw_string(i.x(), i.y(), ("X :%2.1f" % (
                        max_score)), color=(255, 0, 0), scale=2) #红线
                    pack_data = bytearray([0xff,0x00,int(max_score),0xfe])
                    uart_A.write(pack_data)
                if start_processing:
                    record_ftr = feature
                    record_ftrs.append(record_ftr)
                    start_processing = False

                break

        fps = clock.fps()
        #print("%2.1f fps" % fps)
        img.rotation(2)
        a = lcd.display(img)
        gc.collect()
    elif state == 1:
        img = sensor.snapshot()
        img.pix_to_ai()
        code = kpu.run_yolo2(task, img)
        if code:
            for i in code:
                img.draw_rectangle(i.rect(),color = (0, 255, 0))
                img.draw_string(i.x(),i.y(), classes[i.classid()], color=(255,0,0), scale=3)
                lcd.display(img)

                sc = int(i.value())
                state_num = int(i.classid())
                pack_data = bytearray([0xff,state_num,sc,0xfe])
                uart_A.write(pack_data)
        else:
                lcd.display(img)

uart_A.deinit()
del uart_A
    # kpu.memtest()



