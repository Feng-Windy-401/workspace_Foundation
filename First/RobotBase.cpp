#include "RobotBase.h"

RobotBase::RobotBase()
    : pos_x(0.0f), speed(0.0f)
{
}

RobotBase::~RobotBase()
{
    std::cout << "RobotBase Delete" << std::endl;
}

void RobotBase::setSpeed(float speed)
{
    this->speed = speed;
}

int RobotBase::move()
{
    pos_x += speed;
    return 0;
}

void RobotBase::ShowInfo()
{
    std::cout << "[RobotBase] Info: pos_x[" << pos_x << "] speed[" << speed << "]" << std::endl;
}