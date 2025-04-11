只适合64位

注入器,架构测试可以先注入记事本测试. 注入已注入的目标会向目标执行自我销毁消息.

![注入器](https://github.com/user-attachments/assets/1fe3254a-13c8-49c2-88e9-dc8abf9d9758)



跟随功能的上一层函数是一个根据鼠标映射对象的函数,参数根据 字符串+功能地址 的形式可以找到所有的UI操作功能.

![万能call](https://github.com/user-attachments/assets/87f44ea8-14f0-4a42-a276-c4d5d3d850cb)





游戏更新后基址和大偏移可能会变,自动根据特征码重新定位

![游戏更新后](https://github.com/user-attachments/assets/5400c649-3e1e-462b-97d8-c60e965ee5f9)

![更新基址调试输出](https://github.com/user-attachments/assets/badff842-f23e-4656-a2ed-eddc02628590)

![更新基址后](https://github.com/user-attachments/assets/f7498cd8-fef3-4c90-b39d-9632a316883c)



遍历窗口

![遍历窗口](https://github.com/user-attachments/assets/c93ac455-c1d2-43e0-8a5c-701eeeee56e9)

可以根据注入器和DLL的消息传递或者管道进行以注入器为中控的多线程操作.方法名是Viewer::PreTranslateMessage
