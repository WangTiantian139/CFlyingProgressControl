航迹生成应用说明文档 by王众丞 

    该应用可以通过用户输入的命令生成航迹。生成航迹后，还可以在运动终止时刻前的任意时刻添加设定好的运动，并生成新的航迹。

实现方法：
    具体实现时，为了使飞行器飞行这一过程在计算机模拟时具有一般化的物理规律，我选择将控制过程和飞行器运动的过程分离开。该应用定义了两个类，分别是Motion, FlyingProgress. 在Flyingprogress 类使用加速度列表来储存控制量，并且定义了一系列匀速、匀加速、转弯、翻滚、起飞、降落等运动的方法，使用这些方法来生成相应的加速度列表。在Motion 类使用由时刻，位置，速度，加速度构成的状态向量表示一段运动过程，并且定义了move方法，使得下一时刻的状态都由上一时刻的状态决定。最后使用matlab读取所有的位置生成航迹。
    在生成转弯翻滚等运动的加速度列表时，由于它们的本质都是定轴匀速转动，所以下一时刻的加速度矢量依赖飞行器当前时刻的状态确定。当要求做半径为R、转过角度为rad的匀速定轴转动时，只需要输入转动开始时刻的位置 p，速度 v 和加速度 a 矢量，即可求出角速度 w 矢量，之后的每个时刻 a = w x v 求得a，由已转过角度 r = v / R 来控制转过的角度。为了减小误差，对转过角度rad进行了一些修正。

误差分析：
    因为运动过程中，该应用把两个时刻间的运动过程简化为了匀加速直线运动，所以运动过程中难免出现误差。另外，在插入运动时，由于之前的加速度已生成且不能区分原轨迹中各个运动的起止时间，如果插入的运动过程在原轨迹的某一运动过程中间，可能对最终的航迹产生不可控的影响。  

数据结构：
class vector3d
       | double x
       | double y
       | double z

class status
       | vector3d p //位置
       | vector3d v //速度
       | vector3d a //加速度
       | double   t //时间
                
class Motion
       | vector<status> _status //状态
       | double  _dt //时间间隔

template<typename T>
    using ListNodePosi = ListNode<T>*;

template<T> class ListNode
       | T data;
       | ListNodePosi<T> pred;
       | ListNodePosi<T> succ;

class List
       | int _size;
       | ListNodePosi<T> header;
       | ListNodePosi<T> tailer;

class FlyingProgress
       | List<int> _lst_command //命令列表
       | List<vector3d> _lst_acceleration //加速度列表
       | double _dt //时间间隔

使用方法：
1. 使用命令生成航迹
    在 FlyCmdIn.txt 写入以整数表示的命令，用空格分隔
     | 0 - 匀加速直线运动，持续时间1s，加速度{10，0，0}；
     | 1 - 匀加速直线运动，持续时间1s；
     | 2 - 起飞，高度20m，加速度{0，0，10}；
     | 3 - 降落，加速度{0，0，-10}；
     | 4 - 翻滚，半径20m
     | 5 - 左转弯，半径20m，转过角度90度
     | 6 - 右转弯，半径20m，转过角度90度

2. 添加航迹
    在 main 函数中调用FlyingProgress类中的方法