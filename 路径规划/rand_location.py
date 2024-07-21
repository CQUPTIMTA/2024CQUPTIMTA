'''
Description:
Author: qingmeijiupiao
Date: 2024-05-26 20:23:08
'''
import cv2
import numpy as np
import random
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

num_list=[]
def rand_creat_weight():
    global num_list
    while True:
        wp = []
        num = random.sample(range(1, 13), 6)  # 从1到12中随机选择6个不重复的数
        num=sorted(num)
        for i in num:
            wp.append(list(points[i]))
        if(num in num_list):
            continue
        num_list.append(num)
        return wp


def test(background,weight_point):
        # ##################
        #找到最靠左的点位
        left_point=weight_point[0]
        for point in weight_point:
            if point[0]<left_point[0]:
                left_point=point
        #找到最靠右的点位
        right_point=weight_point[0]
        for point in weight_point:
            if point[0]>right_point[0]:
                right_point=point

        ponit_map={
            'L1':left_point,
            'R1':right_point
            }


        cv2.putText(background, 'L1', (left_point[0]-50, left_point[1]-70), cv2.FONT_HERSHEY_SIMPLEX, 4, (0, 0, 255), 5)
        cv2.circle(background,(left_point[0],left_point[1]), 50, (0, 0, 255), 5)
        cv2.putText(background, 'R1', (right_point[0]-50, right_point[1]-70), cv2.FONT_HERSHEY_SIMPLEX, 4, (0, 0, 255), 5)
        cv2.circle(background,(right_point[0],right_point[1]), 50, (0, 0, 255), 5)


        # 删除这3个点
        weight_point.remove(left_point)
        weight_point.remove(right_point)

        #找到最靠左的点位
        left_point=weight_point[0]
        for point in weight_point:
            if point[0]<left_point[0]:
                left_point=point
        #找到最靠右的点位
        right_point=weight_point[0]
        for point in weight_point:
            if point[0]>right_point[0]:
                right_point=point

        ponit_map['L2']=left_point
        ponit_map['R2']=right_point

        cv2.putText(background, 'L2', (left_point[0]-50, left_point[1]-70), cv2.FONT_HERSHEY_SIMPLEX, 4, (0, 0, 255), 5)
        cv2.circle(background,(left_point[0],left_point[1]), 50, (0, 0, 255), 5)
        cv2.putText(background, 'R2', (right_point[0]-50, right_point[1]-70), cv2.FONT_HERSHEY_SIMPLEX, 4, (0, 0, 255), 5)
        cv2.circle(background,(right_point[0],right_point[1]), 50, (0, 0, 255), 5)

        # 删除这3个点
        weight_point.remove(left_point)
        weight_point.remove(right_point)


        #找到最靠近(2750,1000)的点位
        middle_point=weight_point[0]
        dis1=abs(middle_point[0]-2750)
        for point in weight_point:
            dis=abs(point[0]-2750)
            if dis<abs(middle_point[0]-2750):
                middle_point=point

        for point in ponit_map.values():
            if(point[0]==middle_point[0]):
                if(middle_point==weight_point[0]):
                    middle_point=weight_point[1]
                else:
                    middle_point=weight_point[0]



        ponit_map['M1']=middle_point

        cv2.putText(background, 'M1', (middle_point[0]-50, middle_point[1]-70), cv2.FONT_HERSHEY_SIMPLEX, 4, (0, 0, 255), 5)
        cv2.circle(background,(middle_point[0],middle_point[1]), 50, (0, 0, 255), 5)

        case="error"
        if(ponit_map['M1'][0]!=ponit_map['L1'][0] and ponit_map['M1'][0]!=ponit_map['L2'][0] and ponit_map['M1'][0]!=ponit_map['R1'][0] and ponit_map['M1'][0]!=ponit_map['R2'][0] and ponit_map['M1'][0]>ponit_map['L2'][0] and ponit_map['M1'][0]<ponit_map['R2'][0]):
            case="normal"
        elif(ponit_map['M1'][0]>ponit_map['L1'][0] and ponit_map['R2'][0]>2750 and  ponit_map['R1'][0]>2750 and ponit_map['L2'][0]<2750):
            case="need_wait"
        elif(ponit_map['L1'][0]==2000 and ponit_map['L2'][0]==2375 and (ponit_map['R1'][0]==2375 or ponit_map['M1'][0]==2375)):
            case ="fly2"
        else :
            case ="error"
        return background,case
        # ###################


points_list=[]
if __name__ == '__main__':
    total=0
    error_num=0
    # 生成随机点
    while True:
        weight_point = rand_creat_weight()
        weight_point=sorted(weight_point)
        total+=1
        for i in range(0,6):
            weight_point[i][0]=int(weight_point[i][0])+2000
            weight_point[i][1]=int(-1*weight_point[i][1])+1000

        # 读取底图和带alpha通道的图像
        background = cv2.imread('./background.png')
        foreground = cv2.imread('./weight.png', -1)
        for point in weight_point:
            background=add_image(background, foreground, x_offset=point[0], y_offset=point[1])
        # background = add_image(background, foreground, x_offset=2000, y_offset=1000)
        #background,case = test(background,weight_point)





        background=cv2.resize(background,(1280,720))
        cv2.imshow('Result', background)

        if cv2.waitKey() == ord('q'):
            break

        # if(case=="normal"):
        #     pass
        #     # if cv2.waitKey(100000) == ord('q'):
        #         # continue
        #     # break
        # elif(case=="need_wait"):

        #     cv2.imshow('Result', background)
        #     if cv2.waitKey(100000) == ord('q'):
        #         pass


        # elif(case=="fly2"):
        #     error_num=error_num+1
        #     # cv2.imshow('Result', background)
        #     # if cv2.waitKey(100000) == ord('q'):
        #     #     pass
        # else:
        #     error_num=error_num+1
        #     #保存错误图片
        #     # cv2.imwrite('./err/'+str(error_num)+'.png',background)


        #print("total:",total,"error:",error_num,"percent:",error_num/total)
        #cv2.destroyAllWindows()