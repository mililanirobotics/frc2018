/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <iostream>
#include <memory>
#include <string>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include "ctre/Phoenix.h"
#include <Spark.h>
#include "WPILib.h"

/**
 * This is Megan's light code. We're using 2 spikes. 2 of them.
 */
class Robot : public frc::SampleRobot {
public:
	Joystick stick1;
	Relay spike1;
	Relay spike2;
//	Relay Green;

	Robot():
		stick1(0),
		spike1(0), //blue on pos, green on neg
		spike2(1) //red on pos
//		Red(0, Relay::kForwardOnly),
//		Green(0, Relay::kForwardOnly)

	{
	}

	void RobotInit()
	{

	}


	void Autonomous()
	{

	}

	/*
	 * Runs the motors with arcade steering.
	 */
	void OperatorControl() override
	{
		while (IsOperatorControl() && IsEnabled())
		{
			if (stick1.GetRawButton(1))
			{
				//blue
				spike1.Set(Relay::kForward);
				spike2.Set(Relay::kOn);
			}
			else if (stick1.GetRawButton(2))
			{
				//blue-green
				spike1.Set(Relay::kOff);
				spike2.Set(Relay::kOn);
			}

			else if (stick1.GetRawButton(3))
			{
				//green
				spike1.Set(Relay::kReverse);
				spike2.Set(Relay::kOn);

			}
			else if (stick1.GetRawButton(4))
			{
				//green-red
				spike1.Set(Relay::kReverse);
				spike2.Set(Relay::kOff);

			}
			else if (stick1.GetRawButton(5))
			{
				//red
				spike1.Set(Relay::kOn);
				spike2.Set(Relay::kOff);

			}
			else if (stick1.GetRawButton(6))
			{
				//red-blue
				spike1.Set(Relay::kForward);
				spike2.Set(Relay::kOff);

			}
			else if (stick1.GetRawButton(7))
			{
				//white?
				spike1.Set(Relay::kOff);
				spike2.Set(Relay::kOff);

			}
			else
			{
				spike1.Set(Relay::kOn);
				spike2.Set(Relay::kOn);
			}
		}
	}

	void Test() override {}

};

START_ROBOT_CLASS(Robot)
