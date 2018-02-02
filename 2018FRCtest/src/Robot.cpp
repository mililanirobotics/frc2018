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
	gamepad1(0)
	{
	}

	void RobotInit()
	{
	}

	void Autonomous()
	{
		Wait(1);
		std::cout<< "woohoo"<<std::endl;
	}

	void runMotor(int button, TalonSRX& motor)
	{

		if (gamepad1.GetRawButton(button) == 1) //1 = True, so == 1 is not needed -Shane
		{
			motor.Set(ControlMode::PercentOutput, percentOutput);
		}
		else
		{
			motor.Set(ControlMode::PercentOutput, 0);
		}
	}

	void Test() override
	{

	}

	void OperatorControl() override
	{
		bool uppressed = false;
		bool downpressed = false;
		bool testModeOn = false;
		bool testcontrol = false;
		while(IsOperatorControl() && IsEnabled())
		{
			while (!testModeOn) //Real teleop code
			{
				//L1Motor.Set(ControlMode::PercentOutput, 0.3);
				if(gamepad1.GetRawButton(5) && gamepad1.GetRawButton(6) && gamepad1.GetRawButton(7) && gamepad1.GetRawButton(8))
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
				if(gamepad1.GetRawButton(5) && gamepad1.GetRawButton(6) && gamepad1.GetRawButton(7) && gamepad1.GetRawButton(8))
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

				double LStick1 = gamepad1.GetRawAxis(1); //I lied about the talons using percent, so I've removed the *100 -Shane
				double RStick1 = gamepad1.GetRawAxis(5);
				double DPad = gamepad1.GetPOV(0); //Not sure how *100 changes POV, so leaving for now.

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


				if (DPad == 90)
				{
					//right button on DPad
					Claw1.Set(ControlMode::PercentOutput, percentOutput);
				}
				else
				{
					Claw1.Set(ControlMode::PercentOutput, 0);
				}
				if (DPad == 270)
				{
					//left button on DPad
					Claw2.Set(ControlMode::PercentOutput, percentOutput);
				}
				else
				{
					Claw2.Set(ControlMode::PercentOutput, 0);
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
	}
};

START_ROBOT_CLASS(Robot)
