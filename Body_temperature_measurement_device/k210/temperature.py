import sensor,image,lcd,time
import KPU as kpu
from machine import UART
from fpioa_manager import fm

fm.register(25, fm.fpioa.UART1_TX)
fm.register(24, fm.fpioa.UART1_RX)

uart_A = UART(UART.UART1, 115200, 8, None, 1, timeout = 1000, read_buf_len = 4096)



lcd.init(freq=15000000)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_vflip(1)
sensor.run(1)
clock = time.clock()
classes = ['unmask','masks']
#task = kpu.load(0x500000)
task = kpu.load("/sd/mask2.kmodel")
anchor = (0.64, 0.67, 0.93, 0.97, 0.98, 1.06, 1.07, 1.17, 1.17, 1.3)
a = kpu.init_yolo2(task, 0.5, 0.3, 5, anchor)
while(True):
    clock.tick()
    img = sensor.snapshot()
    code = kpu.run_yolo2(task, img)
    print(clock.fps())
    if code:
        for i in code:
            a=img.draw_rectangle(i.rect())
            a = lcd.display(img)
        
            lcd.draw_string(i.x(), i.y(), classes[i.classid()], color=(255, 0, 0), scale = 2)
            lcd.draw_string(i.x(), i.y()+12, '%.3f'%i.value(), color=(255, 0, 0), scale = 2)
            
            dat = i.classid()<<8 | int(i.value) 
    else:
        a = lcd.display(img)
        dat = 0xff
    uart_A.write(dat)
    
a = kpu.deinit(task)
