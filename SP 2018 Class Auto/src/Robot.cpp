/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>

#include <Drive/DifferentialDrive.h>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <Spark.h>
#include <Timer.h>
#include "AutoTank.h"


class Robot : public frc::SampleRobot {
public:
	Robot()

	{
	}

	void RobotInit()
	{

	}


	void Autonomous()
	{
		AutoTank Auto(12, 14, 16, 11, 13, 15);
		Auto.InitEncoders();
		Auto.SetSensorPhase(true);
		Auto.SetPeakOutput(0.5);
		Auto.AutoMove(12.566);
	}


	void OperatorControl() override
	{

	}

	void Test() override {}


private:

};

START_ROBOT_CLASS(Robot)
