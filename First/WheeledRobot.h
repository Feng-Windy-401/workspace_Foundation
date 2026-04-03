#ifndef __WHEELEDROBOT_H__
#define __WHEELEDROBOT_H__
#include "RobotBase.h"

class WheeledRobot : public RobotBase
{
private:
    float wheel_num;

public:
    WheeledRobot();
    ~WheeledRobot();
    void SetInfo(float info) override;
    void ShowInfo() override;
};

#endif
