#include "WheeledRobot.h"

WheeledRobot::WheeledRobot()
    : wheel_num(0.0f)
{
}

WheeledRobot::~WheeledRobot()
{
    std::cout << "WheeledRobot Delete" << std::endl;
}

void WheeledRobot::SetInfo(float info) 
{
    wheel_num = info;
}

void WheeledRobot::ShowInfo()
{
    std::cout << "[WheeledRobot] Info: wheel_num[" << wheel_num << "]" << std::endl;
    RobotBase::ShowInfo();
}
