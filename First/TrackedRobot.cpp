#include "TrackedRobot.h"

TrackedRobot::TrackedRobot()
    : track_len(0.0f)
{
}

TrackedRobot::~TrackedRobot()
{
    std::cout << "TrackedRobot Delete" << std::endl;
}

void TrackedRobot::SetInfo(float info)
{
    track_len = info;
}

void TrackedRobot::ShowInfo()
{
    std::cout << "[TrackedRobot] Info: track_len[" << track_len << "]" << std::endl;
    RobotBase::ShowInfo();
}