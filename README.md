# wolfhuman
狼人杀！

？：改了端口号之后连接网络失败，改的是packdef中的宏

优化：使用自定义放大缩小

优化：在生日当天发送祝福

优化？：在某个界面不再使用的时候销毁它（注册界面不行，如果注册失败还要在界面上显示提示框）

优化：密码加密

优化：登陆时用户已在线，强制上一个用户下线√

优化：重写退出事件

# 第一天



设计启动界面（点击公告，弹出公告栏；点击进入游戏：连接网络隐藏窗口，显示登录窗口）

ckernel类（单例模式）

点击进入游戏->启动窗口发送信号-》ckernel类处理信号（打开网络，隐藏启动窗口，显示登录窗口）





# 第二天



绘制登录界面、注册界面

完成注册功能：用户名（英文or数字or下划线）、密码、个人信息等（优化：验证码）

完成登录功能：服务端发送个人信息给客户端，客户端设置个人信息

下一步：客户端处理个人信息

未解：强制退出账号时，如果在游戏中的话...
