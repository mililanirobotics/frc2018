/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <memory>
#include "ctre/Phoenix.h"
#include <Drive/DifferentialDrive.h>
#include <Spark.h>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include "WPILib.h"
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
class Robot : public frc::SampleRobot {
public:
	TalonSRX L1Motor;
	TalonSRX L2Motor;
	TalonSRX L3Motor;
	TalonSRX R1Motor;
	TalonSRX R2Motor;
	TalonSRX R3Motor;
	TalonSRX LIntake;
	TalonSRX RIntake;
	TalonSRX Lift1;
	TalonSRX Lift2;
	TalonSRX Arm1;
	TalonSRX Arm2;
	TalonSRX Claw1;
	TalonSRX Claw2;
	Joystick gamepad1;
	float percentOutput = 0;
	double motorSpeed = 0.5;
	DigitalInput switch1;
	DigitalInput switch2;
	Relay clawStatus;
	int lightTimer = 0;

	Robot():
	L1Motor(10),
	L2Motor(11),
	L3Motor(12),
	R1Motor(13),
	R2Motor(14),
	R3Motor(15),
	LIntake(16),
	RIntake(17),
	Lift1(18),
	Lift2(19),
	Arm1(20),
	Arm2(21),
	Claw1(22),
	Claw2(23),
	gamepad1(0),
	switch1(0),
	switch2(1),
	clawStatus(0, Relay::kForwardOnly)

	{
	}

	void RobotInit()
	{

	}


	void Autonomous()
	{
		clawStatus.Set(Relay::kForward);
		Wait(2);
		clawStatus.Set(Relay::kOff);
	}


	void OperatorControl() override
	{
		while (IsOperatorControl() && IsEnabled())
		{
			bool switch1status = switch1.Get();
			bool switch2status = switch2.Get();

			if ((switch1status == 1)&&(switch2status == 1))
			{
				clawStatus.Set(Relay::kOn);

				//std::cout<<"two pressed"<<std::endl;
				//Wait(0.1);
			}
			else if ((switch1status == 1)||(switch2status == 1))
			{
				//std::cout<<"one pressed"<<std::endl;
				//Wait(0.1);
				if (lightTimer <= 8000)
				{
					lightTimer = lightTimer + 1;
					//std::cout << lightTimer <<std::endl;
					clawStatus.Set(Relay::kOn);
					//Wait(0.1);
				}
				else if (lightTimer <= 16000 && lightTimer >= 8000)
				{
					clawStatus.Set(Relay::kOff);
					lightTimer = lightTimer + 1;
					//std::cout<< lightTimer <<std::endl;
					//Wait (0.1);
				}
				else
				{
					lightTimer = 0;
				}

			}

			else
			{
				clawStatus.Set(Relay::kOff);
				//std::cout<<"none pressed"<<std::endl;
				//Wait(0.1);
			}

		}
	}


	void Test() override {}

private:


};

START_ROBOT_CLASS(Robot)
