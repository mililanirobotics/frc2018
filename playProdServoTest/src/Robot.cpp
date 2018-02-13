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
#include <Servo.h>

/**
 * This is a demo program showing the use of the DifferentialDrive class.
 * The SampleRobot class is the base of a robot application that will
 * automatically call your Autonomous and OperatorControl methods at the right
 * time as controlled by the switches on the driver station or the field
 * controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're
 * inexperienced, don't. Unless you know what you are doing, complex code will
 * be much more difficult under this system. Use IterativeRobot or Command-Based
 * instead if you're new.
 */
class Robot : public frc::SampleRobot
{
public:
	Servo LServo;
	Servo RServo;
	Joystick gamepad1;
	Robot():

		LServo(0),
		RServo(1),
		gamepad1(0)
	{}

	void RobotInit() {}
	void Autonomous() {}
	void OperatorControl() override
	{
		double LservoPos = 0;
		double RservoPos = 0;
		while(IsOperatorControl() &&  IsEnabled())
		{
			Wait(0.1);
			std::cout<<"L Servo:"<<LservoPos<<std::endl;
			std::cout<<"R Servo:"<<RservoPos<<std::endl;
			if(gamepad1.GetRawAxis(1) >= .1)
			{
				LservoPos += 0.1;
				RservoPos += 0.1;
				LServo.Set(LservoPos);
				RServo.Set(RservoPos);
				std::cout<<"L Servo:"<<LservoPos<<std::endl;
				std::cout<<"R Servo:"<<RservoPos<<std::endl;
			}
			else if(gamepad1.GetRawAxis(1) <= -0.1)
			{
				LservoPos -= 0.1;
				RservoPos -= 0.1;
				LServo.Set(LservoPos);
				RServo.Set(RservoPos);
			}
			if(LservoPos >= 1)
			{
				LservoPos = 1;
			}
			if(LservoPos <= 0)
			{
				LservoPos = 0;
			}
			if(RservoPos >= 1)
			{
				RservoPos = 1;
			}
			if(RservoPos <= 0)
			{
				RservoPos = 0;
			}

		}
	}
	void Test() override {}

};
START_ROBOT_CLASS(Robot)

