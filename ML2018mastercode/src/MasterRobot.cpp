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
#include <Solenoid.h>
#include <DoubleSolenoid.h>
#include <Compressor.h>

/**
MASTER CODE
 */
class Robot : public frc::SampleRobot {
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

	float percentOutput = 0;
	double motorSpeed = 0.5;

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

	/*
	 * Runs the motors with arcade steering.
	 */
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

	void runMotor(int button, TalonSRX& motor)
	{

		if (stick1.GetRawButton(button) == 1) //1 = True, so == 1 is not needed -Shane
		{
			motor.Set(ControlMode::PercentOutput, percentOutput);
		}
		else
		{
			motor.Set(ControlMode::PercentOutput, 0);
		}
	}
	void OperatorControl() override
	{
		bool uppressed = false;
		bool downpressed = false;
		bool testModeOn = false;
		bool testcontrol = false;
		while (IsOperatorControl() && IsEnabled())
		{

			while (!testModeOn) //Real teleop code
				{
					DriverOne();
					DriverTwo();
					if(stick1.GetRawButton(5) && stick1.GetRawButton(6) && stick1.GetRawButton(7) && stick1.GetRawButton(8))
					{
						//L2Motor.Set(ControlMode::PercentOutput, 0.3);
						testcontrol = true;

					}
					else
					{
						if (testcontrol == true)
						{
							testModeOn = true;
							testcontrol = false;
							std::cout << "Test Mode Enabled"<<std::endl;
						}
					}
				}
			while (testModeOn) //Test mode code
				{
					Wait(0.1);
					//L3Motor.Set(ControlMode::PercentOutput, 0.3);
					if(stick1.GetRawButton(5) && stick1.GetRawButton(6) && stick1.GetRawButton(7) && stick1.GetRawButton(8))
					{
						//L2Motor.Set(ControlMode::PercentOutput, 0.3);
						testcontrol = true;
					}
					else
					{
						if (testcontrol == true)
						{
							testModeOn = false;
							testcontrol = false;
							std::cout<< "Test Mode Disabled :)"<<std::endl;
						}
					}
					std::cout << "Percent Output:" << percentOutput<< std::endl;

					double LStick1 = stick1.GetRawAxis(1); //I lied about the talons using percent, so I've removed the *100 -Shane
					double RStick1 = stick1.GetRawAxis(5);
					double DPad = stick1.GetPOV(0); //Not sure how *100 changes POV, so leaving for now.

					if (LStick1 >= 0.2) //Not needed, but || works as an "or" comparison. Try "LStick >= 0.2 || LStick <= -0.2"
					{
						//left wheel
						LIntake.Set(ControlMode::PercentOutput, LStick1);
					}
					else if (LStick1 <= -0.2)
					{
						LIntake.Set(ControlMode::PercentOutput, LStick1);
					}
					else
					{
						LIntake.Set(ControlMode::PercentOutput, 0);
					}
					if (RStick1 >= 0.2) //Same as above. || returns true if either side of it is true. && is and, and only returns true if both sides are true
					{
						//right wheel
						RIntake.Set(ControlMode::PercentOutput, RStick1);
					}
					else if (RStick1 <= -0.2)
					{
						//right wheel
						RIntake.Set(ControlMode::PercentOutput, RStick1);
					}
					else
					{
						RIntake.Set(ControlMode::PercentOutput, 0);
					}
					if (DPad == 0)
					{
						//top button on DPad
						uppressed = true;
					}
					else
					{
						if (uppressed == true)
						{
							uppressed = false;
							percentOutput = percentOutput + 0.10;
							if (percentOutput > 1.00)
							{
								percentOutput = 1.00;
							}
						}
					}

					if (DPad == 180)
					{
						//down button on DPad
						downpressed = true;

						//if it's too fast, reduce the step from 10 to like.. 1 or something lol
					}
					else
					{
						if (downpressed == true)
						{
							downpressed = false;
							percentOutput = percentOutput - 0.10;
							if (percentOutput < -1.00)
							{
								percentOutput = -1.00;
							}
						}
					}


					runMotor(1,  L1Motor);
					runMotor(2,  L2Motor);
					runMotor(3,  L3Motor);
					runMotor(4,  R1Motor);
					runMotor(5,  R2Motor);
					runMotor(6,  R3Motor);
					runMotor(7,  Arm1);
					runMotor(8,  Arm2);
					runMotor(9,  Lift1);
					runMotor(10, Lift2);
				}
	}




};

START_ROBOT_CLASS(Robot)
