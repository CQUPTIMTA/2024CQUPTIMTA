'''
Description: 生成2024物流起重机创意赛的随机砝码位置
Author: qingmeijiupiao
Date: 2024-05-26 20:23:08
'''
import cv2
import numpy as np
import random
import time
# 添加图片到底图
def add_image(background, foreground, x_offset=2000, y_offset=1000):

    x_offset=x_offset-foreground.shape[1]//2
    y_offset=y_offset-foreground.shape[0]//2
    # 提取前景图像的RGB通道和alpha通道
    foreground_rgb = foreground[:,:,0:3]
    alpha = foreground[:,:,3] / 255.0  # 将alpha通道归一化到0-1范围

    # 获取前景图像在底图中的位置
    rows, cols, _ = foreground_rgb.shape
    roi = background[y_offset:y_offset+rows, x_offset:x_offset+cols]

    # 对前景图像和背景图像进行alpha混合
    result = np.empty_like(roi, dtype=np.uint8)
    for y in range(rows):
        for x in range(cols):
            alpha_blend = alpha[y, x]
            if alpha_blend == 1.0:
                result[y, x] = foreground_rgb[y, x]
            else:
                result[y, x] = alpha_blend * foreground_rgb[y, x] + (1 - alpha_blend) * roi[y, x]
    # 更新底图中的前景部分
    background[y_offset:y_offset+rows, x_offset:x_offset+cols] = result
    return background

# 12个砝码坐标
points={
    1:(375.000,649.519),
    2:(1125.000,649.519),
    3:(562.500,324.760),
    4:(937.500,324.760),
    5:(0,0),
    6:(375,0),
    7:(1125,0),
    8:(1500,0),
    9:(562.500,-324.760),
    10:(937.500,-324.760),
    11:(375.000,-649.519),
    12:(1125.000,-649.519)
    }

def rand_creat_weight():

    wp = []
    
    num = random.sample(range(1, 13), 6)  # 从1到12中随机选择6个不重复的数
    num=sorted(num)
    for i in num:
        wp.append(list(points[i]))
    return wp

if __name__ == '__main__':
    #显示分辨率
    show_resolution = (1280, 720)

    # 设置随机数种子,默认为时间戳
    random.seed(time.time())
    # 生成随机砝码
    weight_point = rand_creat_weight()
    for i in range(0,6):
        weight_point[i][0]=int(weight_point[i][0])+2000
        weight_point[i][1]=int(-1*weight_point[i][1])+1000

    # 读取底图和带alpha通道的图像
    background = cv2.imread('./background.png')
    foreground = cv2.imread('./weight.png', -1)
    for point in weight_point:
        background=add_image(background, foreground, x_offset=point[0], y_offset=point[1])
    
    background=cv2.resize(background,show_resolution)
    cv2.imshow('Result', background)

    if cv2.waitKey() == ord('q'):
        exit()
