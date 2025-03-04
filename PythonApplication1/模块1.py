import os
import pandas as pd

# 读取数据文件
data_file = '领导.xlsx'  # 假设数据文件是Excel格式
df = pd.read_excel(data_file)

# 遍历文件夹中的Word文件
for index, row in df.iterrows():
    name = row['姓名']
    serial_number = row['序号']

    # 查找对应的Word文件
    word_file = None
    for file in os.listdir('.'):
        if file.endswith('.docx') and name in file:
            word_file = file
            break

    if word_file:
        # 重命名Word文件
        new_name = f'{serial_number}_{word_file}'
        os.rename(word_file, new_name)
        print(f'已重命名: {word_file} -> {new_name}')
    else:
        print(f'未找到对应的Word文件: {name}')

# 保存处理后的数据文件

print('数据处理完成并已保存！')