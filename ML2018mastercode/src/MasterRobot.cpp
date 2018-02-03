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
#include <ctre/Phoenix.h>
#include <Drive/DifferentialDrive.h>
#include <Spark.h>
#include "WPILib.h"
#include "Drive/DifferentialDrive.h"
/**
MASTER CODE
 */
class Robot : public frc::SampleRobot {
public:
	TalonSRX LFront; //Left Front
	TalonSRX LMiddle; //Left Middle
	TalonSRX LBack; //Left Back
	TalonSRX RFront;
	TalonSRX RMiddle;
	TalonSRX RBack;
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
	int rotation = 4100;
	const bool FORWARD = true;
	const bool BACKWARD = false;
	const bool LEFT = false;
	const bool RIGHT = true;
	float power = 0.2;

	ADXRS450_Gyro gyro;

	DigitalInput chooseAuto1;
	DigitalInput chooseAuto2;

	Robot():
	LFront(10),
	LMiddle(11),
	LBack(12),
	RFront(13),
	RMiddle(14),
	RBack(15),
	LIntake(16),
	RIntake(17),
	Lift1(18),
	Lift2(19),
	Arm1(20),
	Arm2(21),
	stick1(0),
	stick2(1),
	gyro(frc::SPI::kOnboardCS0),
	chooseAuto1(4),
	chooseAuto2(5)

	{
	}

	void RobotInit()
	{

	}

	void AutoLift(){

		}
	void AutoDeposit(){

	}
	void AutoDrive(float inches, bool forward) {
		//float rotations = inches * 217.51175555937088446065331354019;
		int ticksperinch = 4100/12.556; //Steps per rot/circumference
		float ticks = inches * ticksperinch;
		//sets up the sensor/encoder
		SetEncoders();
		float leftAmount = ticks;
		float rightAmount = -ticks;
		std::cout << "leftAmount: " << leftAmount << std::endl;
		if(forward)
		{
			leftAmount = leftAmount*-1;
			rightAmount = rightAmount*-1;
		}
		//flipRight -1 for backwards, 1 for forwards
		//flipLeft -1 for forwards, 1 for backwards
		//int currentPositionRight= RMiddle.GetEncPosition();
		//int currentPositionLeft= LMiddle.GetEncPosition();
		//power = 1;
		//Set the middle motors to what you need distance-wise and the other motors to the main talon's port #.
		//sets the motors to run using encoders
		setFollow();
		//set to follower because the middle motors are the ones with the encoders
		RMiddle.Set(ControlMode::Position, rightAmount);
		std::cout<<"Right Amount:"<<rightAmount<<"\n";
		std::cout<<"Left Amount:" << leftAmount<<"\n";
		LMiddle.Set(ControlMode::Position, leftAmount);
		std::cout<<"after sets\n";
		int rightEncoder = RMiddle.GetSelectedSensorPosition(0);
		int leftEncoder = LMiddle.GetSelectedSensorPosition(0);
		int LPrevious = 10000000;
		int RPrevious = 10000000;
		while (rightEncoder != RPrevious || leftEncoder != LPrevious) //Wait until old and current values are the same, to make sure the motors are no longer spinnig before continuing.
		{
			LPrevious = leftEncoder;
			RPrevious = rightEncoder;
			Wait(0.1);
			rightEncoder = RMiddle.GetSelectedSensorPosition(0);
			leftEncoder = LMiddle.GetSelectedSensorPosition(0);
			std::cout<<"In the loop\n";
			std::cout<< rightEncoder << std::endl;

		}
		std::cout<<"Out the loop\n";
//		//decides the behavior of the motors when power is set to 0 (other option is coast)
		//next step: PLEASE PUT WHILE LOOP HERE, THAT MAKES IT SO THAT WHILE CURRENTPOSITION < SETPOSITION
		//THE MOTOR KEEPS RUNNING! :)
		RMiddle.SetNeutralMode(NeutralMode::Brake);
		LMiddle.SetNeutralMode(NeutralMode::Brake);
	}
	void TestMode()
	{
		SetEncoders();
		RMiddle.SetSelectedSensorPosition(0, 0, 10);
		LMiddle.SetSelectedSensorPosition(0, 0, 10);
		while(IsEnabled())
		{
		Wait(0.25);
		std::cout << "Right: " << RMiddle.GetSelectedSensorPosition(0) << std::endl;
		std::cout << "Left: " << LMiddle.GetSelectedSensorPosition(0) << std::endl;
		}
	}
	void AutoTurn(float degrees, bool turn){
		gyro.Reset();
		SetEncoders();
		setFollow();
		int turnMove = 25;
		if (turn == LEFT){
			turnMove = -25;
		}
		while(IsAutonomous() && IsEnabled() && gyro.GetAngle() <= degrees)
		{
			RMiddle.Set(ControlMode::Position, turnMove);
			LMiddle.Set(ControlMode::Position, turnMove);
			std::cout<<gyro.GetAngle()<<std::endl;
			Wait(0.1);
		}
		RMiddle.SetNeutralMode(NeutralMode::Brake);
		LMiddle.SetNeutralMode(NeutralMode::Brake);
		//RFront.SetNeutralMode(NeutralMode::Brake); <---- DELETE ME IF WORK WITHOUT
		//LFront.SetNeutralMode(NeutralMode::Brake);<---- DELETE ME IF WORK WITHOUT
		//RBack.SetNeutralMode(NeutralMode::Brake);<---- DELETE ME IF WORK WITHOUT
		//LBack.SetNeutralMode(NeutralMode::Brake);<---- DELETE ME IF WORK WITHOUT
		//stops robot
	}

	void AutoRight()
	{
		//std::string gameData;
		//gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		//if(gameData[0] == 'L')
		//{
			//Left Code
			//Forward 14 feet
			//AutoDrive(168, FORWARD);
			//Turn Left 90 degrees
			//AutoTurnL(90);
			//Forward 6 INCHES
			//AutoDrive(6, FORWARD);
		//}
		//else
		//{
			//Put right auto code here
			//Forward 4 feet
		std::cout << "test1" << std::endl;
		AutoDrive(12.556, 1);
		std::cout << "test" << std::endl;
		//Turn Left 90 degrees
		//AutoTurnL(90);
		//Forward 13 feet
		//AutoDrive(156, FORWARD);
		//Turn Right 90 degrees
		//AutoTurnR(90);
		//Forward 3 feet
		//AutoDrive(36, FORWARD);
		//}
	}
	void AutoLeft(){
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		if(gameData[0] == 'L'){
			//Left Code
			//Forward 14 feet.
			AutoDrive(168, FORWARD);
			//Turn Right That 90 degrees
			AutoTurn(90, RIGHT);
			//Forward 6 INCHES
			AutoDrive(6, FORWARD);
		} else {
			//Put right auto code here
			//Forward 4 feet
			AutoDrive(48, FORWARD);
			//Turn Right That 90 degrees
			AutoTurn(90, RIGHT);
			//Go forward 13 feet
			AutoDrive(156, FORWARD);
			//turn left 90 degrees
			AutoTurn(90, LEFT);
			//Forward 3 feet
			AutoDrive(36, FORWARD);
		}
	}
	void AutoMid(){
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		if(gameData[0] == 'L')
		{
			//Left Code
			//Forward 6 feet
			AutoDrive(72, FORWARD);
			//turn right 90 degrees
			AutoTurn(90, RIGHT);
			//Forward 6 feet
			AutoDrive(72, FORWARD);
			//Turn left 90 degrees
			AutoTurn(90, LEFT);
			//Forward 6 feet
			AutoDrive(60, FORWARD);
		} else {
			//Put right auto code here
			//Forward 6 feet
			AutoDrive(72, FORWARD);
			//turn left 90 degrees
			AutoTurn(90, LEFT);
			//Forward 6 feet
			AutoDrive(72, FORWARD);
			//Turn Right to a Right angle
			AutoTurn(90, RIGHT);
			//Forward 5 feet
			AutoDrive(60, FORWARD);
		}
	}
	void autotest()
	{
		LMiddle.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
		LMiddle.SetSelectedSensorPosition(0,0,10);
		LMiddle.SetSensorPhase(true);
		RMiddle.ConfigPeakOutputForward(power, 10);
		LMiddle.ConfigPeakOutputForward(power, 10);
		RMiddle.ConfigPeakOutputReverse(-power, 10);
		LMiddle.ConfigPeakOutputReverse(-power, 10);
		//LMiddle.SetSelectedSensorPosition(0,0,10);
		//int leftEncoder = LMiddle.GetSensorCollection().GetQuadraturePosition();
		int leftEncoder = LMiddle.GetSelectedSensorPosition(0);
		LMiddle.Set(ControlMode::PercentOutput, 0.4);
		RMiddle.Set(ControlMode::PercentOutput, 0.4);
		LFront.Set(ControlMode::PercentOutput, 0.4);
		LBack.Set(ControlMode::PercentOutput, 0.4);
		RFront.Set(ControlMode::PercentOutput, 0.4);
		RBack.Set(ControlMode::PercentOutput, 0.4);
		while (IsEnabled())
		{
			leftEncoder = LMiddle.GetSelectedSensorPosition(0);
			Wait(0.1);
			std::cout << leftEncoder << std::endl;
		}
	}

	void Autonomous()
	{

	}

	/*
	 * Runs the motors with arcade steering.
	 */
	void SetEncoders(){
			//zeroes the sensor position
			//Encoder must be set before everything else
			RMiddle.SetSelectedSensorPosition(0, 0, 10);
			LMiddle.SetSelectedSensorPosition(0, 0, 10);
			//sets up the sensor/encoder
			RMiddle.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
			LMiddle.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
			//controls the power of the motor. change the first value in configpeakoutput to change the speed the motor runs at
			RMiddle.ConfigPeakOutputForward(power, 10);
			LMiddle.ConfigPeakOutputForward(power, 10);
			RMiddle.ConfigPeakOutputReverse(-power, 10);
			LMiddle.ConfigPeakOutputReverse(-power, 10);
			//makes it so that when the motor goes forward, the encoder values increase
			RMiddle.SetSensorPhase(true);
			LMiddle.SetSensorPhase(true);
			//Set PID values
			//RMiddle.Config_kF(0, 0.0, 10);
			RMiddle.Config_kP(0, 1, 10);
			//RMiddle.Config_kI(0, 0, 10);
			//RMiddle.Config_kD(0, 0, 10);
			//LMiddle.Config_kF(0, 0, 10);
			LMiddle.Config_kP(0, 1, 10);
			//LMiddle.Config_kI(0, 0.0, 10);
			//LMiddle.Config_kD(0, 0.0, 10);
		}
	void setFollow(){
			RBack.Set(ControlMode::Follower, 13);
			LBack.Set(ControlMode::Follower, 14);
			RFront.Set(ControlMode::Follower, 13);
			LFront.Set(ControlMode::Follower, 14);
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
			LFront.Set(ControlMode::PercentOutput, LStick1 * speedMultiplier);
			LMiddle.Set(ControlMode::PercentOutput, LStick1 * speedMultiplier);
			LBack.Set(ControlMode::PercentOutput, LStick1 * speedMultiplier);
		}
		else //If inside deadzone set motors to 0
		{
			LFront.Set(ControlMode::PercentOutput, 0);
			LMiddle.Set(ControlMode::PercentOutput, 0);
			LBack.Set(ControlMode::PercentOutput, 0);
		}

		if (RStick1 >= 0.05 || RStick1 <= -0.05) //Same for right motors
		{
			RFront.Set(ControlMode::PercentOutput, RStick1 * speedMultiplier);
			RMiddle.Set(ControlMode::PercentOutput, RStick1 * speedMultiplier);
			RBack.Set(ControlMode::PercentOutput, RStick1 * speedMultiplier);
		}
		else //The same as left, but for right
		{
			RFront.Set(ControlMode::PercentOutput, 0);
			RMiddle.Set(ControlMode::PercentOutput, 0);
			RBack.Set(ControlMode::PercentOutput, 0);
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
						//LMiddle.Set(ControlMode::PercentOutput, 0.3);
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
					//LBack.Set(ControlMode::PercentOutput, 0.3);
					if(stick1.GetRawButton(5) && stick1.GetRawButton(6) && stick1.GetRawButton(7) && stick1.GetRawButton(8))
					{
						//LMiddle.Set(ControlMode::PercentOutput, 0.3);
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


					runMotor(1,  LFront);
					runMotor(2,  LMiddle);
					runMotor(3,  LBack);
					runMotor(4,  RFront);
					runMotor(5,  RMiddle);
					runMotor(6,  RBack);
					runMotor(7,  Arm1);
					runMotor(8,  Arm2);
					runMotor(9,  Lift1);
					runMotor(10, Lift2);
				}
	}




};

START_ROBOT_CLASS(Robot)
