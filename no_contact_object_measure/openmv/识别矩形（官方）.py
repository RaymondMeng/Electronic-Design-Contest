# Find Rects Example
#
# 这个例子展示了如何使用april标签代码中的四元检测代码在图像中找到矩形。 四元检测算法以非常稳健的方式检测矩形，并且比基于Hough变换的方法好得多。 例如，即使镜头失真导致这些矩形看起来弯曲，它仍然可以检测到矩形。 圆角矩形是没有问题的！
# (但是，这个代码也会检测小半径的圆)...

import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565) # 灰度更快(160x120 max on OpenMV-M7)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot().lens_corr(strength = 1.8, zoom = 1.0)

    # 下面的`threshold`应设置为足够高的值，以滤除在图像中检测到的具有
    # 低边缘幅度的噪声矩形。最适用与背景形成鲜明对比的矩形。

    for r in img.find_rects(threshold = 10000):
        img.draw_rectangle(r.rect(), color = (255, 0, 0))
        for p in r.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
        print(r)

    print("FPS %f" % clock.fps())
