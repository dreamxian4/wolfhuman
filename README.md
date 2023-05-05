#  优化完善



？：改了端口号之后连接网络失败，改的是packdef中的宏

~~优化：使用自定义放大缩小√~~

优化：在生日当天发送祝福

~~优化？：在某个界面不再使用的时候销毁它（注册界面不行，如果注册失败还要在界面上显示提示框）不可行~~

~~优化：密码加密~~

~~优化：登陆时用户已在线，强制上一个用户下线√~~

~~优化：重写退出事件~~

~~优化：主界面tabweight页面标题栏的宽度（可以看网络课程qq主界面部分视频）√~~

~~优化：等级（个人信息设置）~~

~~完善：下线时删除服务端对应的映射~~

~~！！！！！！记得传数据库文件！！！！！！！！！！~~

~~！！！！！！删除改名的文件夹！！！！！！！！！~~

完善：游戏模式

优化：房间列表按某项排序

小bug：强制退出那里

优化：房间上限（目前是创建房间必成功）

TODO：设置中有退出登录

小bug：如果是异常退出的话，不走析构函数，不会发送退出登录的包

~~tip:退出事件还没写~~

优化：房间名

完善：反馈

优化：获取房间列表添加一个等待界面

优化：房间密码规定数字字母

完善：加入房间判断（？什么意思来着 等级？）

完善：关于 玩法 回复包中都没设置（定义都没写，）

优化：退出房间时，根据房间状态和是否观战进行移除

优化：服务端退出时，给所有用户发送连接断开，在全部发完之后，才断开

优化：目前都不考虑并发和锁的问题

优化：如果客户端连续进行多次即使操作，有可能会卡住

优化：详见第十天第一条

优化：复盘，详见第十天第二条

优化：在对局内加好友

完善：游戏内如果房主掉线，添加定时等操作

优化：现在时夜晚操作时，只要点击了头像就判定完成操作了，可以加个定时

加锁：服务端在存储狼人杀人信息时可能需要锁

​			服务端更新夜晚状态

优化：夜晚的各种操作可以封装进一个结构体，使用标识和共用体，确定是哪一个（看着有条理一点）

优化：服务端的遍历房间成员操作可以写成宏函数

完善：开始游戏后的房主退出房间

完善：目前不考虑游戏中掉线问题





# bug

1. 房间列表不能根据房间号获取房间信息：输入房间号，不管输入了什么，都是列出所有房间

   **解决**：在服务端通过房间好获取房间信息后，是很据获取的信息有无来判断客户端是根据房间号查找还是筛选条件查找，因此如果输入的房间号错误，是获取不到房间信息的，这个时候服务端误判为是根据筛选条件进行查找了。

   

2. 新注册了个账号，登陆的时候会造成服务器崩溃（逻辑错误）

   **解决**：注册的时候没有给level属性赋值，导致登陆的时候去数据库查询该属性放到链表中，数值不存在，赋值失败。

   

3. 创建房间加入房间时客户端崩溃

   **解决**：客户端的房间界面时在设置房间信息函数内部创建成员子控件的；服务端在收到加入房间请求后，是先发送成员信息，再发送房间信息的，客户端在处理设置成员信息时，还没有收到房间信息，并没有创建成员类，此时给成员类赋值，造成异常。

   

4. 加入房间后，闪退，画面只显示了两个成员控件

   **解决**：和第3条同一个原因

   

5. 房主创建房间后，如果有新成员进来，会显示新成员的信息；但是新成员只能设置房主的信息，自己的和后面再进来的成员都设置不了

   **解决**：原因1：服务端在把新用户添加到链表后没有放进映射中，所以后面的人获取不到除房主外的其他成员信息

   ​			原因2：服务端在发送成员列表时，没有给新成员发送它自己的信息包，导致新成员不能设置自己的信息、

   

6. 加入房间后退出房间，再次加入房间，会有24个成员模块（正常是12个）

   **解决**：退出房间时没有把上一次的房间销毁，加入房间是在上一次的基础上初始化和添加的

   

7. 如果客户端执行的时候服务器没开，那么即使在服务器开了之后，也收不到服务器发过来的包（猜测是初始化顺序问题）



8. 房主退出房间后，其他成员返回房间列表，此时列表中还是上一次获取的房间房间信息；按理说解散的房间会有提示，可是没有，什么反应也没有。

   **解决：**小小bug，服务器发送回复包写错地方了



9. 顶掉上一个人的登录时，后登的人没反应，要再点一次才能登陆上去；房主被顶掉后，房内其他人收不到解散通知。

   **解决**：第二点是服务端处理登录请求时，登录回复包和解散回复包重名了



10. 本应在夜晚到达15s时，客户端向服务端发送半夜包，但是可能由于定时函数的原因，发送的是开始游戏包

    **解决**：修改定时器，之前使用的是

​	







#  asjda



1. 我是按照游戏的正常流程来写功能的，所以先完成了进入房间、准备开始、正式开始等步骤，然后才写身份分配，天黑天亮操作等，导致前面太过完善，调试后面功能找bug的时候很麻烦，要按照流程走（等待5s才能开始）







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





# 第三天



完成主界面绘制（半成品）

优化退出和最小化，设置部分背景

密码加密优化

完成个人信息设置

下一步：创建房间





# 第四天



创建房间：游戏玩法（基础玩法：3（测试用）6-12人只有神民狼 神：预言家、女巫 ）、是否允许抢身份（优化项）、是否加密、等级限制、

完成了客户端发送创建房间请求

新建了几张表（好像并不用这么多张...）

优化了退出登录

下一步：完善表，服务端处理创建房间请求





# 第五天



思考：房间列表和信息可以不用数据库表来存储，直接在服务端创建一个结构体，用映射来进行存储即可。由于后面要获取房间列表（根据房间模式进行获取）

roomid->roomInfo

roomid->list< userinfo>

服务端只发送开始结束包

客户端根据身份进行不同操作

房间类中有一个编号到成员窗口的映射，可以根据该映射修改信息（头像）

完成了创建房间的发送、回复处理；房间界面的绘制

下一步：加入房间、退出房间





# 第六天

完成房间列表功能

下一步：点击x退出登录，如果在房间内，先退房间，再退登录；

​				界面内有单独的退出按钮，可以返回上一级；

​				加入房间





# 第七天



完成加入房间：

1. 客户端发送加入房间请求

​	父类控件所在类维护一个保存子控件链表，在子控件添加进去的时候进行子控件加入房间信号和处理槽的连接；

​	在客户端判断是否加密，密码是否正确；服务端判断等级是否符合房间规定；服务端根据人数和房间状态判断是否观战；

​	密码设定后不可更改，房间内左上角显示房间密码（开放房间；密码：xxx）

2. 服务端处理请求，发送回复

​	服务端发送一个加入房间回复包和一群房间成员列表包，

​	服务端的房间信息中有一个座位号数组（座位号从1开始，到12），每次有新成员加入，遍历该数组，找到一个空座位，赋值给成员（userinfo中加一个座位号，在加入房间的时候，发送给客户端，客户端根据座位号给组件初始化）

​	服务端给刚进入的成员发送原有成员的信息（原有成员的信息：先从房间号->成员列表映射表中获取成员列表，判断该成员是不是新成员本人；根据成员列表的成员id，从数据库中查找该用户的头像等个人信息；再从用户id->用户信息映射表中找到该用户的座位号），给原有成员发送新成员的信息（也要查数据库）。

3. 客户端处理成员列表请求

   通过房间界面类找到成员所在子控件，设置信息

4. 客户端处理加入房间回复

   初始化房间界面信息，生成子控件

   














# 第八天



退出：

1. 客户端发送退出请求：

​		如果点击页面右上角的x，是直接退出软件；

​			如果处于登陆后的状态，向服务端发送离线申请；如果处于在房间中的状态，向服务端发送退出房间申请和离线申请；

​		启动界面、注册界面、登陆界面：直接退出

​		主界面：退出登录

​		房间列表：返回上一级、退出登录

​		房间内：返回上一级、退出房间、退出登录

2. 服务端处理退出房间请求

​		判断是不是房主，如果是，销毁房间；如果不是，发送个人退出

3. 客户端处理退出房间回复

   ​	判断房间是否存在，如果存在，删除个人；如果不存在，返回房间列表



完成了退出房间、退出登录、房间销毁

完善了房间列表界面（加了个房间号）



强制退出：

​	在登陆的时候，服务端判断当前在线id中是否有该id，如果有的话，给上一个客户端发送强制下线包；并判断该用户是否在房间内，如果在房间内，直接从房间中删除（如果该用户为房主，判断房间状态，如果时准备阶段，直接解散；如果是游戏阶段，游戏结束后解散）

​	客户端收到强制下线包时，直接退出登录，回到登录界面，如果在房间内，隐藏房间界面





下一步：解决房间列表不显示房内人数的bug 

ok



下一步：完善个人资料，退出登录，设置等





# 第九天



开始游戏：

​	房主在房间人满后可以点击开始游戏，如果五秒内没有玩家退出，游戏开始；

​	只有房主可以点击开始按钮，其他人点击的话提示只有房主才可以开始游戏

​	人满后，房主发送一个准备开始游戏包，服务端将准备开始回复发给房内的每个人，玩家客户端开始倒计时，五秒后房主再向服务端发送一个正式开始游戏包，服务端将正式开始回复发给房内每个人，游戏开始。

​	如果在倒计时期间有人退出，倒计时结束，房主不发送正式开始游戏包。

​	房主在倒计时期间不能再次点击开始按钮，也不能点击退出按钮

​	保险起见，服务端收到开始包后，判断是否满员



下一步：正式开始，服务器优化（判断是否满员）





# 第十天



服务器优化：判断是否满员

​	万一客户端在判断是否满员的时候正有一个成员退出包在路上，又可能会造成人没满，却开始游戏的情况

​	多一层保险，服务端再判断一次

正式开始：

​	房主发送开始游戏

​	服务端生成身份信息，发送给玩家

​	服务端发送天黑请闭眼

​	玩家根据身份信息和天黑信息进行操作，将操作发送给服务端

​	服务端更新当晚状态

​	服务端发送状态信息和天亮请睁眼信息

​	服务端发送开始发言

​	客户端发言，发言完毕发送结束发言

​	服务端收到结束发言，给下一个人发送开始发言

​	服务端收到最后一个人的发言结束通知，发送开始投票

​	服务端计算投票信息（搞个数组记录每个人的投票，遍历数组计算票数（省的用锁））

​	更新状态，判断结果（是否结束游戏）

​	是：发送游戏结束（复盘...）

​	否：发送天黑请闭眼...

（只要在房间内，服务端就发送包给他，客户端根据自己是不是已死亡，进行不同操作）



下一步：客户端进行天黑操作











# 第十一天

天黑：

​	服务端发送天黑请求

​	客户端根据自己的身份设置不同的操作：预言家点击一个人的头像获取它的身份，狼人点击一个人的头像杀掉他，在狼人杀完人后，女巫获知是谁死掉了并选择救不救，选择毒不毒人，其余身份不能进行操作，点击头像可以查看用户信息（可加好友）

​	room在创建player的时候，连接信号（点击头像）和槽（获取发送信号的player的座位号），在每次点击头像的时候，判断现在处于什么状态，room显示对应的画面。

​	狼人的杀人信息只要选择了就发送给房内每个，客户端判断如果自己是狼人，就显示队友的杀人信息，如果不是，就忽略；

​	狼人是在天黑后前15秒进行操作的（狼人、预言家、守卫），房主进行计时，15秒时发送时间到信息包，服务端计算谁死掉了，将被杀人的座位号发送给每个人，服务端判断自己是不是女巫（10s），如果是，操作，如果不是，忽略；

​	玩家有可能在时间内没有做出操作，因此在收到结束包时，要将控制状态的变量恢复

下一步：客户端处理狼人杀人信息



处理狼人杀人信息：

​	如果自己是狼人，将杀人信息显示出来，如果不是，忽略；

​	房主在15s时向服务端发送前半夜结束包

​	！！！房主如果掉线了。。。。。。（假设房主不会掉线）

​	客户端将狼人的杀人信息发送给女巫

​	女巫进行操作



下一步：客户端女巫







# 第十二天

客户端女巫：

​	服务端会将狼人的杀人信息发送给每个人，客户端判断，如果自己是女巫，就将该包收下，女巫进行显示，如果自己不是女巫，就忽略

​	服务端如果收到女巫的救人包，就将死亡人恢复成默认

​	服务端如果收到女巫的毒人包，将将毒死的人加入到死亡名单



守卫：

​	服务端收到守人信息，判断守的人是不是狼人杀的人，如果是，清除，如果不是，忽略

​	客户端保留守卫的守人信息，判断不能连续两晚守护同一个人



完成黑夜操作



天亮：

​	房主在第二个15s时发送整夜包，服务端统计夜晚信息，判断游戏是否结束，发送夜晚信息包（重置前一晚的信息）

​	服务端收到夜晚结束包后，将当夜的死亡信息发送给房内成员，客户端接收到后，判断死的是不是自己，如果不是，显示死亡玩家，如果是，将自己的身份发送给服务端，服务端更新各身份存活人数，判断游戏是否结束，如果没有结束，给房主发送开始发言包，如果结束了，给所有人发送游戏结束包。



下一步：bug:如果狼人没有杀人，在半夜的时候服务器会崩掉；第一天白天，大家举手竞选警长，从竞选的第一个开始发言。。。



