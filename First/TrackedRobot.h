#ifndef __TRACKEDROBOT_H__
#define __TRACKEDROBOT_H__
#include "RobotBase.h"

class TrackedRobot : public RobotBase
{
private:
    float track_len;

public:
    TrackedRobot();
    ~TrackedRobot();
    void SetInfo(float info) override;
    void ShowInfo() override;
};
#endif