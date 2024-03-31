import matplotlib.pyplot as plt
x_list = []  # 二维X坐标
y_list = []  # 二维Y坐标
X_list = []  # 三维X坐标
Y_list = []  # 三维Y坐标
Z_list = []  # 三维Z坐标


with open('./3d_data', 'r') as f:
    nfile_lines = f.readlines()  # 按行读取N文件

    for line in nfile_lines:
        # 移除行末的换行符
        line = line.strip()

        # 分割行并转换为浮点数
        numbers = [float(num) for num in line.split()]

        X_list.append(numbers[0])
        Y_list.append(numbers[1])
        Z_list.append(numbers[2])
        x_list.append(numbers[3])
        y_list.append(numbers[4])



# 3.1 绘制二维图像
fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.scatter(x_list, y_list, marker='.', color='red', s=8)

# 3.2 绘制三维图像
fig = plt.figure()
ax2 = fig.add_subplot(111, projection='3d')
ax2.scatter(X_list, Y_list, Z_list, color='blue', s=8, alpha=0.5)
#展示图像
plt.show()
