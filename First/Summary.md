# C++ 

## 一.封装

### 1.定义

封装的定义是，把**数据（变量）**和**行为（函数）**包在类里面，==对外部隐藏其中的实现==

### 2.访问控制

在类中有三种访问控制符，分别为$private$、$protect$、$private$

- 对于 $private$ 访问说明符之后的成员，**只能自己访问**，比如自己的==类成员函数==
- 对于 $protect$ 访问说明符之后的成员，**只能自己及子类访问**
- 对于 $public$ 访问说明符之后的成员，**可以被整个程序访问**，==自己+子类+外部==

### 3.类的基本结构

常规的类是存在基本的结构，即**类的架构**，其内容如下：

- 构造函数 `类名()`
- 析构函数`~类名()`
- 成员变量、成员函数

对于构造 函数，本质上就是生成类的对象

对于析构函数，本质上就是释放类的对象

对于成员变量，本质就是类自身的数据，存储内容

对于成员函数，本质就是类自身的行为，处理方法

### 4.实例操作

**目标：**实现一个基础的机器人基类，包含：

私有变量：当前速度、位置

公有方法：设置/获取、移动、打印状态

```c++
#include <iostream>

using namespace std;

class RobotBase
{
private:
    float speed; // 速度
    float pos_x; // X坐标

public:
    RobotBase()
    {
        speed = 0.0;
        pos_x = 0.0;
        cout << "RobotBase Init" << endl;
    }

    /* 设置速度 */
    void setSpeed(float s)
    {
        if(s >= 0)
        {
            speed = s;
        }
    }

    /* 获取速度 */
    float getSpeed()
    {
        return speed;
    }

    /* 移动一秒 */
    void move()
    {
        pos_x = speed + pos_x;
    }

    /* 打印状态 */
    void showStatus()
    {
        cout << "[Base] pos_x: " << pos_x << " | speed: " << speed << endl;
    }
};

int main()
{
    RobotBase robot;
    robot.setSpeed(0.5);
    robot.move();
    robot.showStatus();
    return 0;

}
```

使用G++编译

```bash
g++ robot_base.cpp -o robot
./robot
```

------



## 二.继承

### 1.继承的作用

复用父类的代码，增加扩展性

- 语法：`class 子类: 访问控制符 父类` （访问控制符有`private`、`protect`、`public`）

### 2.继承的规则

总共有三种继承方式，分别是公有继承`public`、保护继承`protect`、私有继承`private`

对于公有继承：

- 父类中的`public`成员，在子类中依旧是`public`成员
- 父类中的`protect`成员，在子类中依旧是`protect`成员
- 父类中的`private`成员，不可被子类访问

对于保护继承：

- 父类中的`public`成员，在子类中变为`protect`成员
- 父类中的`protect`成员，在子类中依旧是`protect`成员
- 父类中的`private`成员，不可被子类访问

对于私有继承：

- 父类中的`public`成员，在子类中变为`private`成员
- 父类中的`protect`成员，在子类中变成`private`成员
- 父类中的`privarte`成员，不可被子类访问

### 3.实例操作

对于上述的`RobotBase`类追加一个派生类`WheeleRobot`，增加一个轮子属性

```c++
class WheeleRobot : public RobotBase
{
private:
    int wheel_num; // 轮子数量

public:
    WheeleRobot()
    {
        wheel_num = 4;
        cout << "WheeleRobot Init" << endl;
    }

    /* 设置轮子数量 */
    void setWheelNum(int n)
    {
        wheel_num = n;
    }

    /* 打印完整信息 */
    void showFullInfo()
    {
        showStatus();
        cout << "[Wheel] 轮子数量：" << wheel_num << endl;
    }
};
```

------



## 三.多态与虚函数

### 1.定义

简单来说**多态 = 同一个接口，不同的实现**，通过父类的指针/引用，调用同一个函数，根据指向的派生类对象不同，执行不同的逻辑



### 2.分类

多态分为：静态多态和动态多态

`静态多态`：

- 也称为**编译期多态**，在编译时被确定，主要依靠函数重载、运算符重载
- 特点为速度快

`动态多态`：

- 也称为**运行期多态**，在运行时被确定，主要依靠==虚函数+继承==
- 特点为灵活性高



### 3.多态的前提条件

- 存在继承关系
- 子类**重写**父类的方法
- 被重写的父类方法必须是**虚函数（virtual）**



### 4.虚函数语法

在**父类**成员函数前加`virtual`关键字，这个函数就变成了虚函数

```cpp
// 父类（基类）中声明函数
virtual 返回值类型 函数名(参数列表){
    // 函数体
}
```



### 5.重写VS重载

在多态之中，对于静态多态常用便是**重载**，而对于动态多态常用便是**重写**

- 重载：同一个作用域内，函数名相同，参数不同的函数
- 重写：子类（派生类）对父类（基类）的**虚函数**进行==重新实现==，需要**保证函数签名完全一致**（返回值、函数名、参数、const都必须相同）

重写虚函数的语法

```cpp
返回值类型 函数名(参数列表) override{
    // 函数体
}
```



### 6.实例操作

将`封装与继承章节`的`RobotBase`基类进行多态化处理，两个派生类`wheeleRobot`、`TrackedRobot`，将基类中的`showStatus()`方法设置为虚函数，两个派生类分别进行实现

```cpp
class RobotBase
{
private:
    float speed; // 速度
    float pos_x; // X坐标

public:
    RobotBase()
    {
        speed = 0.0;
        pos_x = 0.0;
        cout << "RobotBase Init" << endl;
    }

    /* 设置速度 */
    void setSpeed(float s)
    {
        if (s >= 0)
        {
            speed = s;
        }
    }

    /* 获取速度 */
    float getSpeed()
    {
        return speed;
    }

    /* 移动一秒 */
    void move()
    {
        pos_x = speed + pos_x;
    }

    /* 打印状态 */
    virtual void showStatus() 
    {
        cout << "[Base] pos_x: " << pos_x << " | speed: " << speed << endl;
    }
};

class WheeleRobot : public RobotBase
{
private:
    int wheel_num; // 轮子数量

public:
    WheeleRobot()
    {
        wheel_num = 4;
        cout << "WheeleRobot Init" << endl;
    }

    /* 设置轮子数量 */
    void setWheelNum(int n)
    {
        wheel_num = n;
    }

    /* 打印完整信息 */
    void showStatus() override
    {
        cout << "[Wheel] num:" << wheel_num << endl;
        RobotBase::ShowStatus();
    }
};

class TrackRobot : public RobotBase
{
    public:
    TrackRobot()
    {
        cout <<"TrackRobot Init" << endl;
    }

    void showStatus()  override{
        cout<< "[Track] here!" << endl;
        RobotBase::ShowStatus();
    }
};
```

调用方式，使用父类指针指向子类对象，以此证实多态核心

```cpp
	RobotBase *Wr = new WheeleRobot();
    RobotBase *Tr = new TrackRobot();

    Wr->setSpeed(0.8);
    Wr->move();
    Wr->showStatus();

    cout <<"----------------------------" << endl;

    Tr->setSpeed(1.5);
    Tr->move();
    Tr->showStatus();

	delete Wr;
    delete Tr;
```

------



## 四.纯虚函数与抽象类

### 1.定义

没有实际意义的虚函数，便是纯虚函数。其语法为：

```cpp
// 纯虚函数语法：没有函数体，末尾加 = 0
virtual void speak() = 0;
```



### 2.抽象类

包含**至少一个纯虚函数**的类，叫做**抽象类**

- 抽象类不能实例化对象
- 派生类**必须重写所有纯虚函数**，否则派生类也无法实例化
- 抽象类的作用：定义统一接口，约束派生类的行为



### 3.虚析构函数

在多态使用过程中，若**使用基类指针指向派生类对象**，删除基类指针，只会调用基类的析构函数，不会调用派生类的析构函数，因此会造成内存泄露。

解决方法，将基类的析构函数声明为**虚析构函数**：

```cpp
virtual ~类名() {
    // 析构逻辑
}
```



### 4.实例操作

将`RobotBase`基类中的`showStatus`方法设置为纯虚函数，这样`RobotBase`便成为的抽象类，`WheeleRobot`和`TrackRobot`两个类都重写`showStatus`方法。

编写`Robotbase`的虚析构函数，`WheeleRobot`和`TrackRobot`也应该有自己的析构函数。

调用方式，采用父类指针指向，派生类对象

代码如下：

```cpp
#include <iostream>

using namespace std;

class RobotBase
{
private:
    float speed; // 速度
    float pos_x; // X坐标

public:
    RobotBase()
    {
        speed = 0.0;
        pos_x = 0.0;
        cout << "RobotBase Init" << endl;
    }

    /* 设置速度 */
    void setSpeed(float s)
    {
        if (s >= 0)
        {
            speed = s;
        }
    }

    /* 获取速度 */
    float getSpeed()
    {
        return speed;
    }

    /* 移动一秒 */
    void move()
    {
        pos_x = speed + pos_x;
    }

    /* 打印状态 */
    virtual void showStatus()=0;

    virtual ~RobotBase() {cout << "RobotBase Deinit" << endl;}
};

class WheeleRobot : public RobotBase
{
private:
    int wheel_num; // 轮子数量

public:
    WheeleRobot()
    {
        wheel_num = 4;
        cout << "WheeleRobot Init" << endl;
    }

    ~WheeleRobot() {cout << "WheeleRobot Deinit" << endl;}

    /* 设置轮子数量 */
    void setWheelNum(int n)
    {
        wheel_num = n;
    }

    /* 打印完整信息 */
    void showStatus() override
    {
        cout << "[Wheel] num:" << wheel_num << endl;
    }
};

class TrackRobot : public RobotBase
{
    public:
    TrackRobot()
    {
        cout <<"TrackRobot Init" << endl;
    }

    ~TrackRobot() {cout << "TrackRobot Deinit" << endl;}

    void showStatus()  override{
        cout<< "[Track] here!" << endl;
    }
};

int main()
{
    /* 栈空间写法，无需释放内存 */
    // WheeleRobot wr;
    // TrackRobot tr;
    // RobotBase *Wr = &wr;
    // RobotBase *Tr = &tr;

    // Wr->setSpeed(0.8);
    // Wr->move();
    // Wr->showStatus();

    // cout <<"----------------------------" << endl;

    // Tr->setSpeed(1.5);
    // Tr->move();
    // Tr->showStatus();

    // /* 堆空间写法，需要调用 */
    RobotBase *Wr = new WheeleRobot();
    RobotBase *Tr = new TrackRobot();

    Wr->setSpeed(0.8);
    Wr->move();
    Wr->showStatus();

    cout <<"----------------------------" << endl;

    Tr->setSpeed(1.5);
    Tr->move();
    Tr->showStatus();

    delete Wr;
    delete Tr;
    return 0;
}
```



编译命令

```bash
 g++ Robot_base_virtual.cpp -o robot_virtual
```



### 5.总结

- 构造函数不能是虚函数：构造对象时，对象未完全创建，无法找到虚表
- 静态成员函数不可以是虚函数：静态函数属于类，不属于对象，没有this指针，无法通过虚表调用
- 友元函数不能是虚函数：友元函数不是类的成员函数
- 重写必须严格匹配
- C++11`override`关键字：写在派生类末尾，显示声明重写
- 虚函数可以有默认参数：但默认参数的值由**基类**决定

------



## 五.RAll详解

### 1.RAll的作用

在C++中，会使用各种**资源**：

- 动态内存（`new`/`malloc`）
- 文件句柄（`fopen`）
- 互斥锁（`mutex`）
- 网络套接字、数据库连接

**手动管理资源的致命问题：**

- 容易忘记释放资源 -> 内存/句柄泄露
- 代码抛出异常是，释放资源的代码未执行 -> 泄露
- 多分支逻辑中，重复释放/漏释放 -> 程序崩溃



RAll的作用便是彻底解决上述问题



### 2.RAll的核心原理

把**资源**交给**栈上的局部对象**管理：

- 对象构造时 -> 自动获取资源
- 对象析构时 -> 自动释放资源
- C++会**自动调用栈对象的析构函数**（函数退出、异常抛出、栈展开时，编译器兜底）

==资源释放全自动，异常也不会泄露==



### 3.RAll机制的三大黄金规则

- **必须使用栈上的局部对象**（不可使用`new`创建对象）
- **资源绑定到对象生命周期：**对象活着，资源可用；对象死了，资源释放
- **析构函数中释放资源**：不抛异常



### 4.RAll机制的工具

- 动态内存管理
  - `std::unique_ptr`： 独占式智能指针
  - `std::shared_ptr`：共享式智能指针
- 互斥锁管理（多线程必备）
  - `std::lock_guard`：自动加锁、解锁
- 文件操作
  - `std::fstream`：构造打开文件，析构自动关闭

------



## 六.三大智能指针详解

### 1.智能指针的作用

智能指针本质时**用RAll封装裸指针**，自动管理堆内存生命周期，**告别手动`new/delete`、杜绝内存泄漏**

C++11后的三大核心

`unique_ptr`(独占) -> `shared_ptr`(共享) -> `weak_ptr`(解决循环引用)



### 2.三大智能指针的区别

| 指针类型     | 所有权 | 拷贝 | 移动 | 引用计数 | 开销 | 适用场景                 |
| ------------ | ------ | ---- | ---- | -------- | ---- | ------------------------ |
| `unique_ptr` | 独占   | 不可 | 可   | 无       | 极小 | 单个对象独占、高效场景   |
| `share_ptr`  | 共享   | 可   | 可   | 有       | 较大 | 多对象共享同一资源       |
| `weak_ptr`   | 无     | 可   | 可   | 无       | 小   | 解决`shared_ptr`循环引用 |



### 3.`unique_ptr`：独占所有权

**核心特点**

- **同一时间只有一个`unique_ptr`拥有对象**
- **不能拷贝，只能移动**
- 离开作用域**自动`delete`**，无额外开销
- 效率 ≈ 裸指针

==用法如下：==

```c++
#include<memory>
using namespace std;

void test_unique()
{
    // 使用make_unique 创建，等价 unique_ptr<int> p1(new int(10));
    unique_str<int> p1 = make_unique<int>(10);
    
    // 访问同裸指针一样
    cout << *p1 << endl;
    
    // 不能拷贝：unique_ptr<int>p2 = p1;
    
    // 可以移动
    unique_ptr<int>p2 = move(p1);
    if(!p1) cout <<"p1 已空" << endl;
    
    // 离开作用域：p2自动释放内存
}
```

==常用方法:==

- `reset()`：释放当前对象，置空
- `release()`：释放所有权，返回裸指针（不释放内存）
- `get()`：获取裸指针



### 4.`shared_ptr`：共享所有权

**核心特点**

- **多个`shared_ptr`共享一个对象**
- 内部有**原子引用计数**
  - 拷贝 ->  计数 + 1
  - 析构 / 置空 -> 计数 - 1
  - 计数 = 0 -> 真正释放内存
- 有额外开销（计数 + 控制块），比`unique_ptr`慢

==用法如下：==

```c++
void test_shared(){
    //使用make_shared 创建
    share_ptr<int>p1 = make_shared<int>(20);
    cout << "计数： " << p1.use_count() <<endl;
    
    // 拷贝：计数 + 1
    share_ptr<int> p2 = p1;
    cout << "计数：" << p1.use_count() << endl;
    
    // 离开作用域：p2先析构，p1再析构
}
```

<font color = red size = 3>注意：</font>不能使用裸指针初始化多个`shared_ptr`

```c++
int *raw = new int(30);
shared_ptr<int> p1(raw);
shared_ptr<int> p2(raw);  // 错误，两个独立指针，会造成双重释放崩溃
```



### 5.`Weak_ptr`解决`shared_ptr`循环引用

循环引用问题

```c++
struct A;
struct B{
    shared_ptr<A> a;
    ~B(){cout <<"B 析构" << endl;}
};

struct A{
    shared_ptr<B> b;
    ~A(){cout << "A 析构" << endl;}
};

void test_cycle(){
    shared_ptr<A> pa = make_shared<A>();
    shared_ptr<B> pb = make_shared<B>();
    pa->b = pb;
    pb->a = pa;  //互相引用！
    
    // 离开作用域：计数都是1，永远不释放 -> 内存泄漏
}
```

用`weak_ptr`解决

`weak_ptr`不拥有所有权，不增加引用计数，只是“观察者”

```c++
struct A;
struct B{
    weak_ptr<A> a;
    ~B() {cout <<"B 析构" << endl;}
};

struct A{
    shared_ptr<B> b;
    ~A(){cout << "A 析构" << endl;}
};
```

==常用方法==

`lock()`：转为 `shared_ptr`（安全访问）

`expired()`：判断对象是否已释放

`use_count()`：查看引用计数



### 6.智能指针规范

**优先用 `unique_ptr`**：绝大多数场景（独占、高效）

**共享才用 `shared_ptr`**：别滥用，有开销

**循环引用必用 `weak_ptr`**

**永远用 `make_unique`/`make_shared` 创建**：

- 异常安全
- 内存分配更高效

**禁止裸指针 `new/delete` 管理堆内存**

**不要混用裸指针和智能指针**



### 7.Linux上内存检查命令

#### (1) 安装内存检测工具

```bash
apt install valgrind
```

#### (2) 使用内存检查函数运行

```bash
valgrind --leak-check=full ./robot_project # 可执行程序名称
```

