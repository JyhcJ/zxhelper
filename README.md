只适合64位

跟随功能的上一层函数是一个根据鼠标映射对象的函数,参数根据 字符串+功能地址 的形式可以找到所有的UI操作功能.

![游戏更新后](https://github.com/user-attachments/assets/5400c649-3e1e-462b-97d8-c60e965ee5f9)
游戏更新后基址和大偏移可能会变,自动根据特征码重新定位
![更新基址调试输出](https://github.com/user-attachments/assets/badff842-f23e-4656-a2ed-eddc02628590)
![更新基址后](https://github.com/user-attachments/assets/f7498cd8-fef3-4c90-b39d-9632a316883c)


遍历窗口
![遍历窗口](https://github.com/user-attachments/assets/c93ac455-c1d2-43e0-8a5c-701eeeee56e9)

可以根据注入器和DLL的消息传递或者管道进行以注入器为中控的多线程操作.方法名是Viewer::PreTranslateMessage
