#ifndef __ROBOTBASE_H__
#define __ROBOTBASE_H__

#include <iostream>

class RobotBase
{
private:
    float pos_x;
    float speed;

public:
    RobotBase();
    virtual ~RobotBase();

    void setSpeed(float speed);
    int move();
    virtual void ShowInfo();              // 虚函数（多态展示）
    virtual void SetInfo(float info) = 0; // 纯虚函数（抽象展示）
};

#endif