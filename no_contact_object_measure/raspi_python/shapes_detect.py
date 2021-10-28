import cv2 as cv
import numpy as np
from numpy.core.fromnumeric import shape

shapes = {'triangle': 0, 'rectangle': 0, 'polygons': 0, 'circles': 0}

frame = cv.imread("Qv2ray.png")

lower_red_1 = np.array([0, 80, 128]) #先找出HSV色彩空间红绿蓝三种颜色的大致范围。红色有两个是因为hsv空间中，色相h最上面和最下面都是红色。可以看下面这张图你就懂了。
upper_red_1 = np.array([6, 255, 255])
lower_red_2 = np.array([170, 110, 128])
upper_red_2 = np.array([180, 255, 255])
lower_green = np.array([35, 80, 80])
upper_green = np.array([77, 255, 255])
lower_blue = np.array([90, 110, 110])
upper_blue = np.array([124, 255, 255])

hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV) #frame就是输入的图像

red_mask_1 = cv.inRange(hsv,lower_red_1,upper_red_1) #将图像二值化，在lower和upper之间的颜色变为白色，其他全为黑色
red_mask_2 = cv.inRange(hsv,lower_red_2,upper_red_2)
red_mask = cv.bitwise_or(red_mask_1, red_mask_2) #两种红色统一
green_mask = cv.inRange(hsv, lower_green, upper_green)
blue_mask = cv.inRange(hsv, lower_blue, upper_blue)  

red_res = cv.bitwise_or(frame, frame, mask = red_mask) #或运算，将彩色图像中红色部分选中，忽略其余颜色
green_res = cv.bitwise_and(frame, frame, mask = green_mask)
blue_res = cv.bitwise_and(frame, frame, mask = blue_mask)

red_gray = cv.cvtColor(red_res, cv.COLOR_BGR2GRAY) #转灰度图
green_gray = cv.cvtColor(green_res, cv.COLOR_BGR2GRAY)
blue_gray = cv.cvtColor(blue_res, cv.COLOR_BGR2GRAY)

final_gray = cv.bitwise_or(red_res, green_res) #将选出的红色，蓝色，绿色都集成起来
final_gray = cv.bitwise_or(final_gray, blue_res)
final_gray = cv.cvtColor(final_gray,cv.COLOR_BGR2GRAY) #得到最终的灰度图。就是下一步轮廓提取的输入

# __, contours,hierarchy = cv.findContours(final_gray, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
contours,hierarchy = cv.findContours(final_gray, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
#这个地方要注意一下。这个函数根据版本不同，返回的值可能有两个或者三个。如果opencv版本比较新，就只有后面两个返回值。如果比较旧（我代码是在树莓派上跑的，所以比较旧），就有三个返回值。不过我们只用到countours这个返回值就行。
for cnt in range(len(contours)): #对检测到的每个轮廓遍历
    p = cv.arcLength(contours[cnt],True) #p是Perimeter周长的意思，当时偷懒了
    area = cv.contourArea(contours[cnt]) #area是该轮廓的像素面积

for cnt in range(len(contours)):
    p = cv.arcLength(contours[cnt],True)
    area = cv.contourArea(contours[cnt])

    if area > 2500:
        mm = cv.moments(contours[cnt]) #计算图像轮廓中的中心矩，原理见代码后链接
        if mm['m00'] != 0: #如果算出了中心距（感觉这里应该用try,except，当时写的不严谨）
            cx = int(mm['m10'] / mm['m00']) #归一化计算得出轮廓中心的横纵坐标
            cy = int(mm['m01'] / mm['m00'])
        else:
            continue
        epsilon = 0.04 * cv.arcLength(contours[cnt], True) #多边形拟合的距离参数，下一个函数要用到。原理见代码后链接
        approx = cv.approxPolyDP(contours[cnt], epsilon, True)  #轮廓近似。将圆润曲线折线化，以此得到该图像的角点坐标。
        corners = len(approx) #得到角点数量
        if corners == 3: #三个角点的就是三角形
            shapes['triangle'] = shapes['triangle'] + 1
            shapeLenth = p/3 #得到三角形边长
        elif corners == 4: #四个角点就是矩形
            shapes['rectangle'] = shapes['rectangle'] + 1
            shapeLenth = p/4 #得到正方形边长
        else: #圆有好多角点
            shapes['circles'] = shapes['circles'] + 1
            pi = 3.1415926
            rad = p/(2*pi) #得到圆周长

print(shape)