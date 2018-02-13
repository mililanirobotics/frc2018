/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <ctre/Phoenix.h>
#include <Drive/DifferentialDrive.h>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <Spark.h>
#include <Timer.h>
#include "WPILib.h"
#include "Drive/DifferentialDrive.h"

/**
FINAL AUTO CODE
 */
class Robot : public frc::SampleRobot {
	public:
		TalonSRX RFront;
		TalonSRX LFront;
		TalonSRX RMiddle;
		TalonSRX LMiddle;
		TalonSRX RBack;
		TalonSRX LBack;
		ADXRS450_Gyro gyro;
		int rotation = 4100;
		const bool FORWARD = true;
		const bool BACKWARD = false;
		const bool LEFT = false;
		const bool RIGHT = true;
		DigitalInput chooseAuto1;
		DigitalInput chooseAuto2;
		float power = 0.2;
	Robot():
		RFront(11),
		LFront(12),
		RMiddle(13),
		LMiddle(14),
		RBack(15),
		LBack(16),
		gyro(frc::SPI::kOnboardCS0),
		chooseAuto1(4),
		chooseAuto2(5)
		{
		}
	void RobotInit() {

	}

	/*
	well.
	 */
	void setFollow(){
		RBack.Set(ControlMode::Follower, 13);
		LBack.Set(ControlMode::Follower, 14);
		RFront.Set(ControlMode::Follower, 13);
		LFront.Set(ControlMode::Follower, 14);
	}
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
//		LMiddle.Config_kP(0, 0.05, 10);
//		RMiddle.Config_kP(0, 0.05, 10);
//		std::cout<<"test\n";
//		AutoDrive(6, 1);
//		std::cout<<"test2\n";
		//AutoDrive(37, true);
		AutoTurn(45, true);
		AutoTurn(45,false);
		//autotest();
		std::cout<<"Done!"<<std::endl;
	}



	/*
	 * Runs the motors with arcade steering.
	 */
	void OperatorControl() override
		{

		}


	/*
	 * Runs during test mode
	 */
	void Test() override {}

};


START_ROBOT_CLASS(Robot)
