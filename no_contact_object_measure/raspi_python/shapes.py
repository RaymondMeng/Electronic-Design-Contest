import cv2 as cv
import numpy as np
from numpy.core.fromnumeric import shape
import serial

use_hsv = True

lower_red_1 = np.array([0, 100, 20]) #先找出HSV色彩空间红绿蓝三种颜色的大致范围。红色有两个是因为hsv空间中，色相h最上面和最下面都是红色。可以看下面这张图你就懂了。
upper_red_1 = np.array([10, 255, 255])
lower_red_2 = np.array([160, 100, 20])
upper_red_2 = np.array([179, 255, 255])
lower_green = np.array([35, 30, 30])
upper_green = np.array([77, 255, 255])
lower_blue = np.array([90, 110, 110])
upper_blue = np.array([124, 255, 255])
# range of yellow
lower_yellow = np.array([10,100,100])
upper_yellow = np.array([45,255,255])

class ShapeAnalysis:
    def __init__(self):
        pass

    def analysis(self, frame,cThr=[100,100]):
        result = frame.copy()

        if use_hsv:
            img = cv.medianBlur(frame,5)
            hsv = cv.cvtColor(img,cv.COLOR_BGR2HSV)

            mask_blue = cv.inRange(hsv, lower_blue, upper_blue)
            mask_green = cv.inRange(hsv, lower_green, upper_green)
            
            mask_r = cv.inRange(hsv,lower_red_1,upper_red_1)
            mask_r2 = cv.inRange(hsv,lower_red_2,upper_red_2)
            mask_red = mask_r + mask_r2

            kernel = np.ones((5,5))
            
            imgdial = cv.dilate(mask,kernel,iterations=1)
            imgThre = cv.erode(imgdial,kernel,iterations=1)
            cv.imshow("process",np.hstack([mask,imgdial,imgThre]))
        else:
            # 二值化图像
            # print("start to detect lines...\n")
            gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
            blurred = cv.GaussianBlur(gray, (5, 5), 1)
            imgCanny = cv.Canny(blurred,cThr[0],cThr[1])
            kernel = np.ones((5,5))
            imgDial = cv.dilate(imgCanny,kernel,iterations=5)
            imgThre = cv.erode(imgDial,kernel,iterations=3)
            cv.imshow("process", np.hstack([imgCanny,imgThre]))
        

    

        contours, hierarchy = cv.findContours(imgThre, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        for cnt in range(len(contours)):
            area = cv.contourArea(contours[cnt])
            if area > 8000 and area < 100000:

                # 提取与绘制轮廓
                cv.drawContours(result, contours, cnt, (0, 255, 0), 2)

                # 轮廓逼近
                epsilon = 0.02 * cv.arcLength(contours[cnt], True)
                approx = cv.approxPolyDP(contours[cnt], epsilon, True)

                # 分析几何形状
                corners = len(approx)
                shape_type = ""
                if corners == 3:
                    count = self.shapes['triangle']
                    count = count+1
                    self.shapes['triangle'] = count
                    shape_type = "三角形"
                if corners == 4:
                    count = self.shapes['rectangle']
                    count = count + 1
                    self.shapes['rectangle'] = count
                    shape_type = "矩形"
                if corners >= 6:
                    count = self.shapes['circles']
                    count = count + 1
                    self.shapes['circles'] = count
                    shape_type = "圆形"
                if 4 < corners < 6:
                    count = self.shapes['polygons']
                    count = count + 1
                    self.shapes['polygons'] = count
                    shape_type = "多边形"


                # 求解中心位置
                mm = cv.moments(contours[cnt])
                try:
                    cx = int(mm['m10'] / mm['m00'])
                    cy = int(mm['m01'] / mm['m00'])
                except ZeroDivisionError as e:
                    return
                cv.circle(result, (cx, cy), 3, (0, 0, 255), -1)
                print("中心：",cx,cy)

                # 颜色分析
                color = frame[cy][cx]
                color_str = "(" + str(color[0]) + ", " + str(color[1]) + ", " + str(color[2]) + ")"
                
                p = cv.arcLength(contours[cnt], True)
                print("周长: %.3f, 面积: %.3f 颜色: %s 形状: %s 角点: %d"% (p, area, color_str, shape_type,corners))
                x = int(cx*255/640)
                y = int(cy*255/480)
                c_H = int(p) >> 8
                c_L = int(p) & 0xff
                pack_data = bytearray([0xff,x,y,c_H,c_L,0xfe])
                
                print(pack_data)


        # cv.imshow("Analysis Result", self.draw_text_info(result))
        cv.imshow("Analysis Result", result)
        # cv.imwrite("D:/test-result.png", self.draw_text_info(result))
        return self.shapes

    # def draw_text_info(self, image):
    #     c1 = self.shapes['triangle']
    #     c2 = self.shapes['rectangle']
    #     c3 = self.shapes['polygons']
    #     c4 = self.shapes['circles']
    #     cv.putText(image, "triangle: "+str(c1), (10, 20), cv.FONT_HERSHEY_PLAIN, 1.2, (255, 0, 0), 1)
    #     cv.putText(image, "rectangle: " + str(c2), (10, 40), cv.FONT_HERSHEY_PLAIN, 1.2, (255, 0, 0), 1)
    #     cv.putText(image, "polygons: " + str(c3), (10, 60), cv.FONT_HERSHEY_PLAIN, 1.2, (255, 0, 0), 1)
    #     cv.putText(image, "circles: " + str(c4), (10, 80), cv.FONT_HERSHEY_PLAIN, 1.2, (255, 0, 0), 1)
    #     return image


if __name__ == "__main__":
    count = int(0)
    cap = cv.VideoCapture(0)
    cap.set(3,640)
    cap.set(4,480)
    ld = ShapeAnalysis()
    while True:
        ret, frame = cap.read()
        if not ret:
            cap.release()
        ld.analysis(frame)
        key = cv.waitKey(20)
        if key == 'c':
            cv.imwrite("%d.jpg"%(count),frame)
            count += 1
    cv.destroyAllWindows()
