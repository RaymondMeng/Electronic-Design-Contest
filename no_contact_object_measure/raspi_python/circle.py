import cv2
import numpy as np

cap = cv2.VideoCapture(0)

lower_red_1 = np.array([0, 80, 128]) #先找出HSV色彩空间红绿蓝三种颜色的大致范围。红色有两个是因为hsv空间中，色相h最上面和最下面都是红色。可以看下面这张图你就懂了。
upper_red_1 = np.array([6, 255, 255])
lower_red_2 = np.array([170, 110, 128])
upper_red_2 = np.array([180, 255, 255])
lower_green = np.array([35, 80, 80])
upper_green = np.array([77, 255, 255])
lower_blue = np.array([90, 110, 110])
upper_blue = np.array([124, 255, 255])
# range of yellow
lower_yellow = np.array([10,100,100])
upper_yellow = np.array([45,255,255])


# img = cv2.imread("paiqiu.jpeg")
while True:
    _, img = cap.read()
    output = img.copy()
    img = cv2.medianBlur(img,5)
    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

    mask_r = cv2.inRange(hsv, lower_blue, upper_blue)

    mask_r2 = cv2.inRange(hsv, lower_yellow, upper_yellow)

    mask = mask_r + mask_r2

    kernel = np.ones((5,5))

    imgdial = cv2.dilate(mask,kernel,iterations=5)
    imgthr = cv2.erode(imgdial,kernel,iterations=2)

    contours, hierarchy = cv2.findContours(imgthr, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for cnt in range(len(contours)):
        area = cv2.contourArea(contours[cnt])
        if area > 8000:
            # 提取与绘制轮廓
            cv2.drawContours(output, contours, cnt, (0, 255, 0), 2)

            # 轮廓逼近
            epsilon = 0.02 * cv2.arcLength(contours[cnt], True)
            approx = cv2.approxPolyDP(contours[cnt], epsilon, True)
            # 求解中心位置
            mm = cv2.moments(contours[cnt])
            try:
                cx = int(mm['m10'] / mm['m00'])
                cy = int(mm['m01'] / mm['m00'])
            except ZeroDivisionError as e:
                continue
            cv2.circle(output, (cx, cy), 3, (0, 0, 255), -1)
            print("中心：",cx,cy)

    cv2.imshow("origin", np.hstack([img,output]))
    cv2.imshow('Mask', np.hstack([mask,imgdial,imgthr]))
    cv2.waitKey(1)

cv2.destroyAllWindows()
cap.release()