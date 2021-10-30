import sensor,image,lcd,time
import KPU as kpu

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.VGA)
sensor.set_windowing((320, 256))
sensor.set_vflip(1)
sensor.set_hmirror(1)
clock = time.clock()
classes = ["no_mask", "mask"]
task = kpu.load("/sd/mask.kmodel")
anchor = (0.156250, 0.222548,0.361328, 0.489583,0.781250, 0.983133,1.621094, 1.964286,3.574219, 3.940000)
kpu.init_yolo2(task, 0.3, 0.3, 5, anchor)
while(True):
    img = sensor.snapshot()
    img.pix_to_ai()
    code = kpu.run_yolo2(task, img)
    if code:
        for i in code:
            img.draw_rectangle(i.rect(),color = (0, 255, 0))
            img.draw_string(i.x(),i.y(), classes[i.classid()], color=(255,0,0), scale=3)
            lcd.display(img)
    else:
            lcd.display(img)
    print(clock.fps())
kpu.deinit(task)
