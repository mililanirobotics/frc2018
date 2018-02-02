#pragma once

#include <ctre/Phoenix.h>
#include <math.h>
#include <DoubleSolenoid.h>


enum Motor //A little enum I made to allow easy selection of motors and make sure dumb programmers don't put in wierd parameters.
{
	L1,
	L2,
	L3,
	R1,
	R2,
	R3
};

enum Arm
{
	Forklift
	//Add more later
};

enum Claw
{
	Talon,
	Pneumatics
};

/*
 * A neat class that makes programming auto easier.
 * Allows you to control multiple motors, string commands, set encoders, and more.
 * Works with up to 6 motors right now.
 */

/* TODO:
 * Encoder on motor other than middle? This would require funky stuff with arguments or TONS of if statements.
 * Allow manual movement (move left treads x steps forward and right y steps) instead of inches forward. Not sure why you would want this over in. though.
 * Claw/arm support
 * Debug mode?
 * Add function to set PID and add PID defaults to costructor.
 * Fix dem pneumatics NAOW
 * Literally everything else
 */


class AutoTank {

	TalonSRX L1; //Declare motors as private, because we don't want other stuff tampering with them. (Not that this would actually happen)
	TalonSRX L2;
	TalonSRX L3;

	TalonSRX R1;
	TalonSRX R2;
	TalonSRX R3;

	//TalonSRX tCL;
	//TalonSRX tCR;

	//DoubleSolenoid pCL;
	//DoubleSolenoid pCR;

	int WheelOffset;
	int StepsPerRot;
	int WheelCircumference;
	int StepsPerIn;
	Arm ArmType;
	Claw ClawType;

public:
	AutoTank(int left1, int left2, int left3, int right1, int right2, int right3):
		L1(left1), //Assign CAN ids to motors from constructor arguments
		L2(left2),
		L3(left3),
		R1(right1),
		R2(right2),
		R3(right3)



	{
		L1.Set(ControlMode::Follower, left2); //Set motors to follower in bulk of constructor
		L3.Set(ControlMode::Follower, left2); //This will need to be changed if I want to allow encoder selection
		R1.Set(ControlMode::Follower, right2);
		R3.Set(ControlMode::Follower, right2);

		L2.Config_kP(0, 1, 10); //Base PID values
		//L2.Config_kI(0, 0, 10);
		//L2.Config_kD(0, 0, 10);
		R2.Config_kP(0, 1, 10);
		//R2.Config_kI(0, 0, 10);
		//R2.Config_kD(0, 0, 10);

		WheelOffset = 10; //An estimate. Please set the real thing if you want good rotations.
		StepsPerRot = 4100; //Typical for our motors
		WheelCircumference = 12.566; //For a 4 in diameter. Seems typical.
		StepsPerIn =  StepsPerRot / WheelCircumference;

		ArmType = Arm::Forklift; //More defaults
		ClawType = Claw::Pneumatics;
		Wait(5);

	}


	void SetEncValue(Motor motorenc, int value) //Allows you to manually set the encoder values
	{
		switch(motorenc)
		{
		case Motor::L1 :
			L1.SetSelectedSensorPosition(value, 0, 10);
			break;
		case Motor::L2 :
			L2.SetSelectedSensorPosition(value, 0, 10);
			break;
		case Motor::L3 :
			L3.SetSelectedSensorPosition(value, 0, 10);
			break;
		case Motor::R1 :
			R1.SetSelectedSensorPosition(value, 0, 10);
			break;
		case Motor::R2 :
			R2.SetSelectedSensorPosition(value, 0, 10);
			break;
		case Motor::R3 :
			R3.SetSelectedSensorPosition(value, 0, 10);
			break;
		}
	}

	void SetWheelOffset(int in) //Takes the wheel distance from the center in in. Used for rotations.
	{
		WheelOffset = in;
	}

	void SetEncoderStepsPerRot(int steps) //Encoder ticks per rotation
	{
		StepsPerRot = steps;
		StepsPerIn =  StepsPerRot / WheelCircumference;
	}

	void SetWheelDiameter(int diam) //Wheel diameter. Required for AutoTurn, AutoDrive, and AutoCombo.
	{
		WheelCircumference = diam/2; //Convert to radius
		WheelCircumference = 2 * M_PI * WheelCircumference; //Radius to circumference
		StepsPerIn =  StepsPerRot / WheelCircumference; //Recalculate with new value
	}

	void SetSensorPhase(bool on) //Makes encoders go positive when motors are set to positive power.
	{
		L2.SetSensorPhase(on);
		R2.SetSensorPhase(on);
	}

	void SetNeutralMode(NeutralMode mode) //Does the motor acive brake or passively coast when set to 0?
	{
		L2.SetNeutralMode(mode);
		R2.SetNeutralMode(mode);
	}

	void SetPeakOutput(float peak) //Peak output when moving. Stops your robot from shaking itself to death.
	{
		L2.ConfigPeakOutputForward(peak, 10);
		L2.ConfigPeakOutputReverse(peak, 10);
		R2.ConfigPeakOutputForward(peak, 10);
		R2.ConfigPeakOutputForward(peak, 10);
	}

	void SetPID(float p, float i, float d) //Used to ramp up your motors over time instead of trying to get to your set value instantly.
	{
		L2.Config_kP(0, p, 10); //Custom PID values
		L2.Config_kI(0, i, 10);
		L2.Config_kD(0, d, 10);
		R2.Config_kP(0, p, 10);
		R2.Config_kI(0, i, 10);
		R2.Config_kD(0, d, 10);
	}

	void InitEncoders() //Please add ability to only do some encoders. Set to middle for now.
	{
		//L1.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
		L2.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
		//L3.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
		//R1.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
		R2.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
		//R3.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
	}

	void InitArm(Arm type, int id, int id2) //Set your arm type. Add as we come up with new designs.
	{
		switch(type)
		{
		case(Arm::Forklift) :
				TalonSRX Fork1(id);
				ArmType = Arm::Forklift;
		}
	}
/*
	void InitClaw(Claw type, int id, int id2, int id3, int id4) //Set your claw type. Are you using lame talons or amazing pneumaics?
	{
		switch(type)
		{
		case Claw::Talon :
		{
			DoubleSolenoid AutoTank::tCL(id);
			DoubleSolenoid AutoTank::tCR(id2);
			ClawType = Claw::Talon;
			break;
		}
		case Claw::Pneumatics :
		{
			DoubleSolenoid AutoTank::pCL(id, id2);
			DoubleSolenoid AutoTank::pCR(id3, id4);
			ClawType = Claw::Pneumatics;
			break;
		}
		}
	}

	void ClawToggle(bool open) //Open/close your selected claw type
	{
		switch(ClawType)
		{
		case Claw::Talon :
			//Something with limit switches and encoders probably
			break;

		case Claw::Pneumatics :
			if(open)
			{
				pCL.Set(DoubleSolenoid::Value::kReverse);
				pCR.Set(DoubleSolenoid::Value::kForward);
			}
			else if(!open)
			{
				pCL.Set(DoubleSolenoid::Value::kForward);
				pCR.Set(DoubleSolenoid::Value::kReverse);
			}
		}
	}
*/
	void AutoMove(int in) //Move forward/back some inches.
	{
		int StepsToMove = in * StepsPerIn;

		L2.Set(ControlMode::Position, StepsToMove); //Both positive because we want a forward/back movement
		R2.Set(ControlMode::Position, StepsToMove);

		std::cout << "Steps To Move: " << StepsToMove << std::endl;
		Wait(2);
		/*
		if(StepsToMove > 0) //A very complicated way of making sure the motors get to where they are going before the function exits
		{
			while(StepsToMove > L2.GetSelectedSensorPosition(0) && StepsToMove > R2.GetSelectedSensorPosition(0))
			{
				Wait(0.1);
			}
		}
		else if(StepsToMove < 0)
		{
			while(StepsToMove < L2.GetSelectedSensorPosition(0) && StepsToMove < R2.GetSelectedSensorPosition(0))
			{
				Wait(0.1);
			}
		}
		*/
		L2.SetSelectedSensorPosition(0, 0, 10);
		R2.SetSelectedSensorPosition(0, 0, 10);
	}

	void AutoTurn(int deg) //Positive for clockwise and negative for CCW
	{
		int Circumference = 2 * M_PI * WheelOffset;
		int InPerDegree = Circumference / 360;

		int StepsToMove = InPerDegree * deg * StepsPerIn;

		L2.Set(ControlMode::Position, StepsToMove);
		R2.Set(ControlMode::Position, StepsToMove); //Negative to make it rotate

		if(StepsToMove > 0) //A very complicated way of making sure the motors get to where they are going before the function exits
		{
			while(StepsToMove > L2.GetSelectedSensorPosition(0) && StepsToMove < R2.GetSelectedSensorPosition(0))
			{
				Wait(0.25);
			}
		}
		else if(StepsToMove < 0)
		{
			while(StepsToMove < L2.GetSelectedSensorPosition(0) && StepsToMove > R2.GetSelectedSensorPosition(0))
			{
				Wait(0.25);
			}
		}

	}

	void AutoCombo(int initDeg, int in, int endDeg) //Combines the awesomeness of turn and move to make an UNSTOPABLE FORCE OF PROGRAMMING
	{
		//FIRST ROTATE
		int Circumference = 2 * M_PI * WheelOffset;
		int InPerDegree = Circumference / 360;

		int OldPosL = L2.GetSelectedSensorPosition(0);
		int OldPosR = R2.GetSelectedSensorPosition(0);

		int StepsToMove = InPerDegree * initDeg * StepsPerIn;

		L2.Set(ControlMode::Position, OldPosL + StepsToMove);
		R2.Set(ControlMode::Position, OldPosR - StepsToMove); //Negative to make it rotate

		if(StepsToMove > 0) //A very complicated way of making sure the motors get to where they are going before the function exits
		{
			while(OldPosL + StepsToMove > L2.GetSelectedSensorPosition(0) && OldPosR - StepsToMove < R2.GetSelectedSensorPosition(0))
			{
				Wait(0.25);
			}
		}
		else if(StepsToMove < 0)
		{
			while(OldPosL + StepsToMove < L2.GetSelectedSensorPosition(0) && OldPosR - StepsToMove > R2.GetSelectedSensorPosition(0))
			{
				Wait(0.25);
			}
		}

		//MOVE
		OldPosL = L2.GetSelectedSensorPosition(0);
		OldPosR = R2.GetSelectedSensorPosition(0);

		StepsToMove = in * StepsPerIn;

		L2.Set(ControlMode::Position, OldPosL + StepsToMove); //Both positive because we want a forward/back movement
		R2.Set(ControlMode::Position, OldPosR + StepsToMove);

		if(StepsToMove > 0) //A very complicated way of making sure the motors get to where they are going before the function exits
		{
			while(OldPosL + StepsToMove > L2.GetSelectedSensorPosition(0) && OldPosR + StepsToMove > R2.GetSelectedSensorPosition(0))
			{
				Wait(0.25);
			}
		}
		else if(StepsToMove < 0)
		{
			while(OldPosL + StepsToMove < L2.GetSelectedSensorPosition(0) && OldPosR + StepsToMove < R2.GetSelectedSensorPosition(0))
			{
				Wait(0.25);
			}
		}

		//END ROTATE
		OldPosL = L2.GetSelectedSensorPosition(0);
		OldPosR = R2.GetSelectedSensorPosition(0);

		StepsToMove = InPerDegree * endDeg * StepsPerIn;

		L2.Set(ControlMode::Position, OldPosL + StepsToMove);
		R2.Set(ControlMode::Position, OldPosR - StepsToMove); //Negative to make it rotate

		if(StepsToMove > 0) //A very complicated way of making sure the motors get to where they are going before the function exits
		{
			while(OldPosL + StepsToMove > L2.GetSelectedSensorPosition(0) && OldPosR - StepsToMove < R2.GetSelectedSensorPosition(0))
			{
				Wait(0.25);
			}
		}
		else if(StepsToMove < 0)
		{
			while(OldPosL + StepsToMove < L2.GetSelectedSensorPosition(0) && OldPosR - StepsToMove > R2.GetSelectedSensorPosition(0))
			{
				Wait(0.25);
			}
		}
	}
};
