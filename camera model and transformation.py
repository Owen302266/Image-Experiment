# 实现三维坐标向二维坐标的转换

import numpy as np
import matplotlib.pyplot as plt
import os
import imageio

path = '/imgs_exp2'

"""相机内、外参矩阵"""

world_coordinate_list = [[0, 0, 0, 1], [50, 0, 0, 1], [50, 50, 0, 1], [0, 50, 0, 1],
                         [0, 0, 20, 1], [50, 0, 20, 1], [50, 50, 20, 1], [0, 50, 20, 1]]

# 外参矩阵
Out = np.mat([[1, 0, 0, 0],
              [0, -1, 0, 0],
              [0, 0, -1, -1000],
              [0, 0, 0, 1]
              ])

# 内参矩阵
K = np.mat([
    [50, 0, 320],
    [0, 50, 240],
    [0, 0, 1]
])

# 焦距
focal_length = np.mat([
    [10, 0, 0, 0],
    [0, 10, 0, 0],
    [0, 0, 1, 0]
])


def calculate(world_coordinate_lists, out, k, focal_distance, j):
    """坐标转换"""
    pixel_coordinate_list = []

    for item in world_coordinate_lists:
        world_coordinate = np.mat([
            [item[0]],
            [item[1]],
            [item[2]],
            [item[3]]
        ])
        print(f'世界坐标为：\n{world_coordinate}')

        # 世界坐标系转换为相机坐标系 （Xw,Yw,Zw）--> (Xc,Yc,Zc)
        camera_coordinate = out * world_coordinate
        print(f'相机坐标为：\n{camera_coordinate}')
        Zc = float(camera_coordinate[2])
        print(f'Zc={Zc}')

        # 相机坐标系转图像坐标系 (Xc,Yc,Zc) --> (x, y)  
        image_coordinate = (focal_distance * camera_coordinate) / Zc
        print(f'图像坐标为：\n{image_coordinate}')

        # 图像坐标系转换为像素坐标系
        pixel_coordinate = k * image_coordinate
        print(f'像素坐标为：\n{pixel_coordinate}')
        pixel_coordinate_list.append(pixel_coordinate)
        print('---------------------分割线--------------------------------')

    # 将数据转换为numpy array格式
    pixel_coordinate_list = np.array(pixel_coordinate_list)
    X = pixel_coordinate_list[:, 0]
    Y = pixel_coordinate_list[:, 1]

    # 在二维平面上绘制长方体在像素空间中的投影
    plt.plot([X[0], X[1], X[2], X[3], X[0]],
             [Y[0], Y[1], Y[2], Y[3], Y[0]], c='b')
    #
    plt.plot([X[4], X[5], X[6], X[7], X[4]],
             [Y[4], Y[5], Y[6], Y[7], Y[4]], c='b')

    plt.plot([X[4], X[0]],
             [Y[4], Y[0]], c='b')
    # 
    plt.plot([X[5], X[1]],
             [Y[5], Y[1]], c='b')
    # 
    plt.plot([X[6], X[2]],
             [Y[6], Y[2]], c='b')
    # 
    plt.plot([X[7], X[3]],
             [Y[7], Y[3]], c='b')
    # plt.show()
    plt.savefig(f'{path}/{j}.png')  # 保存图片
    plt.close()


for i in range(4):
    # 让相机每次沿x轴运动10mm，并计算长方体各顶点在像素空间中的坐标
    Out[0, 3] = i * 10

    calculate(world_coordinate_list, Out, K, focal_length, i)

# 生成.gif图，用于动态可视化
filenames = []
for files in os.listdir(path):
    if files.endswith('jpg') or files.endswith('jpeg') or files.endswith('png'):
        file = os.path.join(path, files)
        filenames.append(file)

images = []
for filename in filenames:
    images.append(imageio.imread(filename))
imageio.mimsave(f'{path}/OR_perceptron.gif', images, duration=0.8)
