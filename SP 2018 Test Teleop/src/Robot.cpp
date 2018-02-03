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
#include <Solenoid.h>
#include <DoubleSolenoid.h>
#include <Compressor.h>

/**
 * This is a demo program showing the use of the RobotDrive class.
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
class Robot: public frc::SampleRobot
{


public:

	TalonSRX L1Motor; //Left Front
	TalonSRX L2Motor; //Left Middle
	TalonSRX L3Motor; //Left Back
	TalonSRX R1Motor;
	TalonSRX R2Motor;
	TalonSRX R3Motor;
	TalonSRX LIntake;
	TalonSRX RIntake;
	TalonSRX Lift1;
	TalonSRX Lift2;
	TalonSRX Arm1;
	TalonSRX Arm2;


	Joystick stick1;
	Joystick stick2;

	DoubleSolenoid *claw = new DoubleSolenoid(0,1);
	DoubleSolenoid *claw2 = new DoubleSolenoid(2, 3);

	Robot():
		L1Motor(10), //this motor goes backwards so I'm fixing that
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
		stick1(0),
		stick2(1)
	{
	}

	void RobotInit()
	{

	}


	void Autonomous()
	{

	}


	void OperatorControl() override
		{
		while (IsOperatorControl() && IsEnabled())
		{
			DriverOne();
			DriverTwo();
		}
	}
	void DriverOne()
	{
		double LStick1;
		double RStick1;
		//double LTrigger2;
		//double RTrigger2;
		double speedMultiplier;
		//Change speed based on button 1 and 2 on either joystick
		if (stick1.GetRawAxis(2) > 0.1 || stick1.GetRawAxis(3) > 0.1)
		{
			speedMultiplier = 0.5;
		}
		else if (stick1.GetRawButton(5) || stick1.GetRawButton(6))
		{
			speedMultiplier = 0.33;
		}
		else
		{
			speedMultiplier = 1;
		}
		//Tank drive using the 2 sticks on gamepad 1
		LStick1 = stick1.GetRawAxis(1);
		RStick1 = stick1.GetRawAxis(5);

		if (LStick1 >= 0.05 || LStick1 <= -0.05) //If outside of deadzone set motors to stick value
		{
			L1Motor.Set(ControlMode::PercentOutput, LStick1 * speedMultiplier);
			L2Motor.Set(ControlMode::PercentOutput, LStick1 * speedMultiplier);
			L3Motor.Set(ControlMode::PercentOutput, LStick1 * speedMultiplier);
		}
		else //If inside deadzone set motors to 0
		{
			L1Motor.Set(ControlMode::PercentOutput, 0);
			L2Motor.Set(ControlMode::PercentOutput, 0);
			L3Motor.Set(ControlMode::PercentOutput, 0);
		}

		if (RStick1 >= 0.05 || RStick1 <= -0.05) //Same for right motors
		{
			R1Motor.Set(ControlMode::PercentOutput, RStick1 * speedMultiplier);
			R2Motor.Set(ControlMode::PercentOutput, RStick1 * speedMultiplier);
			R3Motor.Set(ControlMode::PercentOutput, RStick1 * speedMultiplier);
		}
		else //The same as left, but for right
		{
			R1Motor.Set(ControlMode::PercentOutput, 0);
			R2Motor.Set(ControlMode::PercentOutput, 0);
			R3Motor.Set(ControlMode::PercentOutput, 0);
		}



	}
	void DriverTwo()
	{

		double LStick2;
		LStick2 = stick2.GetRawAxis(1);

		if(stick2.GetRawButton(8))
		{
			claw->Set(DoubleSolenoid::Value::kForward);
			claw2->Set(DoubleSolenoid::Value::kForward);
		}
		else if(stick2.GetRawButton(7))
		{
			claw->Set(DoubleSolenoid::Value::kReverse);
			claw2->Set(DoubleSolenoid::Value::kReverse);
		}

		//Claw intake is controlled by triggers on gamepad 2
		double LTrigger2 = stick2.GetRawAxis(3);
		double RTrigger2= stick2.GetRawAxis(2);

		if (RTrigger2 >= 0.2)
		{
			LIntake.Set(ControlMode::PercentOutput, RTrigger2*0.3);
			RIntake.Set(ControlMode::PercentOutput, -RTrigger2*0.3);
			//std::cout<<"wheel intake time"<<std::endl;
			//Pull left trigger to make wheels spin outwards
		}
		else if (LTrigger2 >= 0.2)
		{
			LIntake.Set(ControlMode::PercentOutput, -LTrigger2*0.3);
			RIntake.Set(ControlMode::PercentOutput, LTrigger2*0.3);
			//std::cout<<"wheel intake time"<<std::endl;
			//Pull right trigger to make wheels spin inwards
		}
		else
		{
			LIntake.Set(ControlMode::PercentOutput, 0);
			RIntake.Set(ControlMode::PercentOutput, 0);
			//Set to 0 if deadzone in effect
		}
		if (stick2.GetRawButton(9))
		{
			Lift1.Set(ControlMode::PercentOutput, 1);
			Lift2.Set(ControlMode::PercentOutput, 1);
			//std::cout<< "lift time"<<std::endl;

		}
		else if (stick2.GetRawButton(10))
		{
			Lift1.Set(ControlMode::PercentOutput, -1);
			Lift2.Set(ControlMode::PercentOutput, -1);
			//std::cout<< "lift time"<<std::endl;
		}
		else
		{
			Lift1.Set(ControlMode::PercentOutput, 0);
			Lift2.Set(ControlMode::PercentOutput, 0);
		}
		if (LStick2 >= 0.05 || LStick2 <= -0.05)
		{
			Arm1.Set(ControlMode::PercentOutput, LStick2);
			Arm2.Set(ControlMode::PercentOutput, LStick2);
			//std::cout<< "arm time"<<std::endl;
		}
		else //If inside deadzone set motors to 0
		{
			Arm1.Set(ControlMode::PercentOutput, 0);
			Arm2.Set(ControlMode::PercentOutput, 0);

		}

	}

	void Test() override
	{

	}
};


START_ROBOT_CLASS(Robot)
