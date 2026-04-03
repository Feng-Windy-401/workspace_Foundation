#include"RobotBase.h"
#include"TrackedRobot.h"
#include"WheeledRobot.h"
#include <iostream>
#include <memory>

int main()
{
    /* 抽象类无法实体，搭配智能指针 */
    std::unique_ptr<RobotBase> wr = std::make_unique<WheeledRobot>();
    std::unique_ptr<RobotBase> tr = std::make_unique<TrackedRobot>();

    /* 调用基类函数 */
    wr->setSpeed(0.6f);
    wr->move();
    wr->ShowInfo();
    wr->SetInfo(4.0f);
    wr->ShowInfo();

    std::cout <<"----------------------------------"<< std::endl;

    /* 调用基类函数 */
    tr->setSpeed(0.6f);
    tr->move();
    tr->ShowInfo();
    tr->SetInfo(10.0f);
    tr->ShowInfo();

    std::cout <<"----------------------------------"<< std::endl;
    
    return 0;
}