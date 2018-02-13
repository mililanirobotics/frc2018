/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
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
#include <Drive/DifferentialDrive.h>
#include <Spark.h>
#include "WPILib.h"
#include "Drive/DifferentialDrive.h"

/**
 * This is a test code for the arm, using the optical encoders.
 * Tape is going to be placed on 3 different sides of the gearbox that is attached to
 * the arm pivot motor. The encoder is going to be placed on the forklift right below the
 * gearbox, looking up at it.
 */
class Robot : public frc::SampleRobot {

public:
	TalonSRX Arm1;
	TalonSRX Arm2;
	Joystick stick2;
	DigitalInput opti;

	Robot():
	Arm1(20),
	Arm2(21),
	stick2(1),
	opti(0)

	{

	}

	void RobotInit()
	{

	}


	void Autonomous()
	{
		while (IsAutonomous() && IsEnabled())
		{
			std::cout<<opti.Get()<<std::endl;
			Wait (0.2);
		}
	}

	/*
	 * Runs the motors with arcade steering.
	 */
	void OperatorControl() override
	{
		bool button3Pressed = false;
		bool button4Pressed = false;
		bool button3Clicked = false;
		bool button4Clicked = false;
		float armPower = 0;
		int tapeTimer = 0;
		int tapeTimer1 = 0;
		int positionNumber = 0;
		bool pivotingF = false;
		bool pivotingB = false;

		while (IsOperatorControl() && IsEnabled())
		{

			std::cout<< "tapetimer:"<<tapeTimer <<std::endl;
			//manual mode
			if (stick2.GetRawButton(1))
			{
				armPower = 0.1;
				Arm1.Set(ControlMode::PercentOutput, armPower);
				Arm2.Set(ControlMode::PercentOutput, armPower);
				std::cout<<"1 button"<< std::endl;

			}
			else if (stick2.GetRawButton(2))
			{
				armPower = -0.1;
				Arm1.Set(ControlMode::PercentOutput, armPower);
				Arm2.Set(ControlMode::PercentOutput, armPower);
				std::cout<<"2 button"<< std::endl;
			}
			else
			{
				armPower = 0;
				Arm1.Set(ControlMode::PercentOutput, armPower);
				Arm2.Set(ControlMode::PercentOutput, armPower);
			}

			//optical sensor mode
			if (stick2.GetRawButton(3) && positionNumber <2)
			{
				button3Pressed = true;
				std::cout<<"arm power:"<< armPower<< std::endl;
			}
			else
			{
				if (button3Pressed == true)
				{
					button3Pressed = false;
					positionNumber = positionNumber + 1;
					button3Clicked = true;
					std::cout<<"arm power:"<< armPower<< std::endl;
				}
			}
			if (button3Clicked == true)
			{

				pivotingF = true;
				button3Clicked = false;
				std::cout<<"arm power:"<< armPower<< std::endl;
			}
			if (opti.Get() == 0 && tapeTimer > 100)
			{
				armPower = 0;
				Arm1.Set(ControlMode::PercentOutput, armPower);
				Arm2.Set(ControlMode::PercentOutput, armPower);
				pivotingF = false;
				std::cout << "tape timer:" << tapeTimer <<std::endl;
				tapeTimer = 0;
			}
			else if (pivotingF == true)
			{
				armPower = 0.1;
				Arm1.Set(ControlMode::PercentOutput, armPower);
				Arm2.Set(ControlMode::PercentOutput, armPower);
				std::cout<<"arm power:"<< armPower<< std::endl;
				tapeTimer = tapeTimer + 1;
			}
			else
			{
				armPower = 0;
				Arm1.Set(ControlMode::PercentOutput, armPower);
				Arm2.Set(ControlMode::PercentOutput, armPower);
			}




			if (stick2.GetRawButton(4) && positionNumber >=0)
			{
				button4Pressed = true;
				std::cout<<"arm power:"<< armPower<< std::endl;
			}
			else
			{
				if (button4Pressed == true)
				{
					button4Pressed = false;
					positionNumber = positionNumber - 1;
					button4Clicked = true;
					std::cout<<"arm power:"<< armPower<< std::endl;
				}
			}
			if (button4Clicked == true)
			{

				pivotingB = true;
				button4Clicked = false;
				std::cout<<"arm power:"<< armPower<< std::endl;
			}
			if (opti.Get() == 0 && tapeTimer1 > 100)
			{
				armPower = 0;
				Arm1.Set(ControlMode::PercentOutput, armPower);
				Arm2.Set(ControlMode::PercentOutput, armPower);
				pivotingB = false;
				std::cout << "tape timer:" << tapeTimer <<std::endl;
				tapeTimer1 = 0;
			}
			else if (pivotingB == true)
			{
				armPower = -0.1;
				Arm1.Set(ControlMode::PercentOutput, armPower);
				Arm2.Set(ControlMode::PercentOutput, armPower);
				std::cout<<"arm power:"<< armPower<< std::endl;
				tapeTimer1 = tapeTimer1 + 1;
			}
			else
			{
				armPower = 0;
				Arm1.Set(ControlMode::PercentOutput, armPower);
				Arm2.Set(ControlMode::PercentOutput, armPower);
			}


		}
	}

	/*
	 * Runs during test mode
	 */
	void Test() override {}


};

START_ROBOT_CLASS(Robot)
