#include <iostream>
#include <memory>
#include <string>
#include <cmath>

#include <Timer.h>

#include <IterativeRobot.h>
#include <DriverStation.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/Sendable.h>
#include <SmartDashboard/SmartDashboard.h>
#include <CameraServer.h>
#include <tables/ITable.h>

#include "RobotDefs.h"

#include <PowerDistributionPanel.h>
#include <ADXRS450_Gyro.h>
#include <DigitalInput.h>

#include "CANTalonDrivetrain.h"
#include "Winch.h"
#include "GearLift.h"


bool g_bPracticeRobot = false;
const double maxSpeed = 2000;

class Robot: public frc::IterativeRobot
{

private:
	frc::PowerDistributionPanel	m_PDP {0};
	frc::XboxController	m_controller{0};
	frc::ADXRS450_Gyro 	m_gyro{frc::SPI::kOnboardCS0};

	DriverStation::Alliance m_allianceColor;
	int m_allianceLocation;


	CANTalonDriveTrain 	m_driveTrain {&m_controller, &m_gyro};
	Winch 				m_winchMotor {&m_PDP};
	GearLift			m_gearLift   {};
	DigitalInput 		m_diPracticeRobot {DIO_PRACTICE_ROBOT};

	double 	m_gyroAngle;
	double 	m_leftJoystickY;
	double 	m_rightJoystickY;
	double 	m_leftTrigger;
	double 	m_rightTrigger;
	bool   	m_bLeftBumper;
	bool   	m_bRightBumper;
	bool	m_bButtonA;
	bool	m_bButtonB;
	bool	m_bButtonX;
	bool	m_bButtonY;

	frc::LiveWindow* lw = LiveWindow::GetInstance();

	std::shared_ptr<NetworkTable> axisCameraTable;


	// autonomous states
	//
	typedef enum
	{
		autoStart	 = 0,
		autoTraverse = 1,
		autoDropGear = 2,
		autoDone	 = 3
	} eAutonomousState;
	std::string m_strAutoState[4] = {"autoStart", "autoTraverse", "autoDropGear", "autoDone"};


	// traverse states
	//
	typedef enum
	{
		traverseNext = 0,
		traverseTurn = 1,
		traverseMove = 2,
		traverseDone  = 3
	} eTraverseState;
	std::string m_strTraverseState[4] = {"traverseNext", "traverseTurn", "traverseMove", "traverseDone"};


	eAutonomousState m_autoState = autoDone;
	eTraverseState m_traverseState = traverseDone;
	int m_traverseIndex = 0;

	double m_angle[AUTO_MOVE_MAX_SEGMENTS];
	double m_leftSpeed[AUTO_MOVE_MAX_SEGMENTS];
	double m_rightSpeed[AUTO_MOVE_MAX_SEGMENTS];
	double m_distance[AUTO_MOVE_MAX_SEGMENTS];

	double m_wheelCircumfrence = 0.00;

public:

	void RobotInit()
	{
		m_allianceColor    = DriverStation::GetInstance().GetAlliance();
		m_allianceLocation = DriverStation::GetInstance().GetLocation();
		std::cout << "Alliance Color   : " << m_allianceColor << std::endl;
		std::cout << "Alliance Location: " << m_allianceLocation << std::endl;

		// Jumper is installed on Practice Robot, which pulls DI low
		g_bPracticeRobot = !m_diPracticeRobot.Get();

		if (g_bPracticeRobot)
			m_wheelCircumfrence = PRACTICE_ROBOT_WHEEL_CIRCUMFRENCE;
		else
			m_wheelCircumfrence = COMPETITION_ROBOT_WHEEL_CIRCUMFRENCE;

		frc::CameraServer::GetInstance()->StartAutomaticCapture("Driving Camera", 0);

//.		axisCameraTable = NetworkTable::GetTable("GRIP/contoursReport");
//.		double gripNumbers[1];
//.		axisCameraTable->GetNumberArray("contoursReport", gripNumbers);
//.		frc::SmartDashboard::PutNumber("Center X: ", gripNumbers[0]);

	}


	//=================================================================================
	// Autonomous Initialize
	//
	//	Starting Location key:
	//			+-----------------+
	//		 	| 1				1 |
	//	Blue 	| x 			x | Red
	//	Alliance| 2				2 | Alliance
	//		 	| 3				3 |
	//			+-----------------+
	//
	void AutonomousInit() override
	{
		m_traverseIndex = 0;

		// clear all traverse segments
		//
		for (int i=0; i<AUTO_MOVE_MAX_SEGMENTS; i++)
		{
			m_distance[i] = 0.0;
			m_leftSpeed[i] = 0.0;
			m_rightSpeed[i] = 0.0;
			m_angle[i] = 0.0;
		}

		// The autonomous traverse path depends on the starting location
		//
		switch (m_allianceLocation)
		{
		case 1:
			m_angle[0] 		= kStart1Angle_0;
			m_distance[0] 	= kStart1Dist_0 / m_wheelCircumfrence;
			m_leftSpeed[0]	= kStart1SpeedLf_0;
			m_rightSpeed[0]	= kStart1SpeedRt_0;
			m_rightSpeed[0]	= kStart1SpeedRt_0;

			m_angle[1] 		= kStart1Angle_1;
			m_distance[1] 	= kStart1Dist_1 / m_wheelCircumfrence;
			m_leftSpeed[1]	= kStart1SpeedLf_1;
			m_rightSpeed[1]	= kStart1SpeedRt_1;
			break;
		case 2:
			m_angle[0] 		= kStart2Angle_0;
			m_distance[0] 	= kStart2Dist_0 / m_wheelCircumfrence;
			m_leftSpeed[0]	= kStart2SpeedLf_0;
			m_rightSpeed[0]	= kStart2SpeedRt_0;

			m_angle[1] 		= kStart2Angle_1;
			m_distance[1] 	= kStart2Dist_1 / m_wheelCircumfrence;
			m_leftSpeed[1]	= kStart2SpeedLf_1;
			m_rightSpeed[1]	= kStart2SpeedRt_1;

			m_angle[2] 		= kStart2Angle_2;
			m_distance[2] 	= kStart2Dist_2 / m_wheelCircumfrence;
			m_leftSpeed[2]	= kStart2SpeedLf_2;
			m_rightSpeed[2]	= kStart2SpeedRt_2;
			break;
		case 3:
			m_angle[0] 		= kStart3Angle_0;
			m_distance[0] 	= kStart3Dist_0 / m_wheelCircumfrence;
			m_leftSpeed[0]	= kStart3SpeedLf_0;
			m_rightSpeed[0]	= kStart3SpeedRt_0;

			m_angle[1] 		= kStart3Angle_1;
			m_distance[1] 	= kStart3Dist_1 / m_wheelCircumfrence;
			m_leftSpeed[1]	= kStart3SpeedLf_1;
			m_rightSpeed[1]	= kStart3SpeedRt_1;
			break;
		default:
			break;
		}

		// Autonomous traverse path was calculated from Red Alliance starting location.
		//	If the actual alliance is Red, the angles need to be flipped.
		//
		if (m_allianceColor == DriverStation::Alliance::kBlue)
		{
			for (int i=0; i<AUTO_MOVE_MAX_SEGMENTS; i++)
				m_angle[i] 	  = -m_angle[i];

		}

		m_autoState = autoStart;
		m_traverseState = traverseNext;
	}


	//=================================================================================
	// Autonomous Periodic
	//
	//	Step through Autonomous State Machine
	//
	void AutonomousPeriodic()
	{
		switch (m_autoState)
		{
		case autoStart:
			m_autoState = autoTraverse;
			break;
		case autoTraverse:
			// AutoTraverse() returns true after a segment completes and
			// the either isn't another segment allowed, or the next segment
			// has a speed of 0
			if (AutoTraverse())
				m_autoState = autoDropGear;
			break;
		case autoDropGear:
			m_autoState = autoDone;
			break;
		case autoDone:
			break;
		}

		UpdateDashboard();
	}


	bool AutoTraverse(void)
	{
		// if moving to the next segment (new turn and move values)
		if (m_traverseState == traverseNext)
		{
			// if reached max number of segments or segment specifies a speed of 0
			if (m_traverseIndex >= AUTO_MOVE_MAX_SEGMENTS ||
				m_leftSpeed[m_traverseIndex] == 0)
			{
				// Done moving. Return true, so m_autoState can move on to dropping gear
				m_traverseState = traverseDone;
				return true;
			}
			// if max number of segments not reached and the segment doesn't have a speed of 0
			else
			{
				// Start turning
				m_traverseState = traverseTurn;
				m_gyro.Reset();
				m_driveTrain.AutoCalculateTurn(m_angle[m_traverseIndex], kTurnSpeed);
				UpdateDashboard();
				// Commented out to test the other
				//m_driveTrain.AutoTurnStart(m_gyroAngle, m_angle[m_traverseIndex], kTurnSpeed);
			}
		}

		// if in the turning part of a segment
		if (m_traverseState == traverseTurn)
		{
			// AutoTurnUpdate() returns true when robot has turned the correct angle
			if (m_driveTrain.AutoTurn(m_angle[m_traverseIndex])
					/*m_driveTrain.AutoTurnUpdate(m_gyroAngle)*/)
			{
				// After turning is done, go to the moving part of the segment
				m_traverseState = traverseMove;
				m_driveTrain.resetEncoders();
				Wait(.5);
				UpdateDashboard();
				// Commented out to test the other
				//m_driveTrain.AutoMoveStart(m_distance[m_traverseIndex], m_leftSpeed[m_traverseIndex], m_rightSpeed[m_traverseIndex]);
			}
		}

		// if in the moving part of the segment
		if (m_traverseState == traverseMove)
		{
			// AutoMoveUpdate() returns true when the robot has moved the correct distance
			if (m_driveTrain.AutoMove(m_distance[m_traverseIndex], m_leftSpeed[m_traverseIndex], m_rightSpeed[m_traverseIndex])
					/*m_driveTrain.AutoMoveUpdate()*/)
			{
				// After moving is done, try to go to the next segment
				m_traverseState = traverseNext;
				// Increment the index
				m_traverseIndex++;
				UpdateDashboard();
			}

		}

		// if return true did not trigger because robot in the middle of a segment, return false
		return false;
	}

	void TeleopInit()
	{
		m_driveTrain.Stop();

		frc::SmartDashboard::PutString("Alliance Color    : ", (m_allianceColor == DriverStation::Alliance::kRed) ? "Red" : "Blue");
		frc::SmartDashboard::PutNumber("Alliance Location : ", m_allianceLocation);
	}

	void InitTraverse(void)
	{
		m_traverseState = traverseNext;
		m_traverseIndex = 0;

		for (int i=0; i<AUTO_MOVE_MAX_SEGMENTS; i++)
		{
			m_distance[i] = 0.0;
			m_leftSpeed[i] = 0.0;
			m_rightSpeed[i] = 0.0;
			m_angle[i] = 0.0;
		}

		m_angle[0] 		= 15;
		m_distance[0] 	  = (19.125 * 3)/ m_wheelCircumfrence;
		m_leftSpeed[0]	  = .15;
		m_rightSpeed[0]	  = .15;

		return;

		m_angle[1] 		= 0.0;
		m_distance[1] 	  = 19.125 / m_wheelCircumfrence;
		m_leftSpeed[1]	  = 0.15;
		m_rightSpeed[1]	  = 0.15;

	}

	void UpdateControlData()
	{
		m_gyroAngle = m_gyro.GetAngle();
		m_leftJoystickY  = m_controller.GetY(frc::GenericHID::kLeftHand);
		m_rightJoystickY = m_controller.GetY(frc::GenericHID::kRightHand);

		m_leftTrigger = m_controller.GetTriggerAxis(frc::GenericHID::kLeftHand);
		m_rightTrigger = m_controller.GetTriggerAxis(frc::GenericHID::kRightHand);

		m_bLeftBumper = m_controller.GetBumper(frc::GenericHID::kLeftHand);
		m_bRightBumper = m_controller.GetBumper(frc::GenericHID::kRightHand);

		m_bButtonA = m_controller.GetAButton();
		m_bButtonB = m_controller.GetBButton();
		m_bButtonX = m_controller.GetXButton();
		m_bButtonY = m_controller.GetYButton();
	}

	double m_leftPosOffset = 0.0;
	double m_rightPosOffset = 0.0;

	void TeleopPeriodic()
	{
		UpdateControlData();

		if (m_bButtonB)
			m_driveTrain.resetEncoders();
		else if (m_bButtonA)
		{
			if (m_autoState != autoDone)
				AutonomousPeriodic();

//			if (m_autoState == autoStart)
//			{
//				m_autoState = autoTraverse;
//			}
//			else if (m_autoState == autoTraverse)
//			{
//				if (AutoTraverse())
//					m_autoState = autoDone;
//			}
//			UpdateDashboard();
			return;
		}
		else
		{
			if (m_autoState != autoStart)
			{
				m_driveTrain.Stop();
				InitTraverse();
				m_autoState = autoStart;
			}
		}

//		UpdateControlData();

		// Commented out to test the auto drive straight in teleop
		//m_driveTrain.Update(m_leftJoystickY, m_rightJoystickY, m_bLeftBumper);
		m_driveTrain.AutoDriveStraight(m_leftJoystickY, m_rightJoystickY);

		m_gearLift.Update(m_bButtonX, m_bButtonY, m_bRightBumper, (m_rightTrigger > .7));
		m_winchMotor.Update(m_leftTrigger);

		UpdateDashboard();
	}

	void TestPeriodic()
	{
		//lw->Run();


	}

	void UpdateDashboard(void)
	{
		m_driveTrain.UpdateStats();
/*
		double centerX = 0;
		axisCameraTable->GetNumber("x", centerX);
		frc::SmartDashboard::PutNumber("Center X: ", centerX);
		axisCameraTable = NetworkTable::GetTable("GRIP/myBlobsReport");
 */

		frc::SmartDashboard::PutString("Robot : ", (g_bPracticeRobot) ? "Practice Robot" : "Competition Robot");
		frc::SmartDashboard::PutString("AutoState     : ", m_strAutoState[m_autoState]);
		frc::SmartDashboard::PutString("TraverseState : ", m_strTraverseState[m_traverseState]);

		frc::SmartDashboard::PutNumber("Encoder Velocity Difference : ", round(m_driveTrain.GetEncoderVelocityDifference(), 2));

		frc::SmartDashboard::PutNumber("Left Joystick  : ", round(m_leftJoystickY, 2));
		frc::SmartDashboard::PutNumber("Left Command   : ", round(m_driveTrain.GetLeftTarget(), 2));
		frc::SmartDashboard::PutNumber("Left Speed     : ", round(m_driveTrain.GetLeftSpeed(), 2));
		frc::SmartDashboard::PutNumber("Left Position  : ", round(m_driveTrain.GetLeftPosition(), 2));
		frc::SmartDashboard::PutNumber("Left Enc. Pos. : ", round(m_driveTrain.GetLeftEncoderPos(), 2));
		frc::SmartDashboard::PutNumber("Left Enc. Vel. : ", round(m_driveTrain.GetLeftEncoderVel(), 2));

		frc::SmartDashboard::PutNumber("Right Joystick : ", round(m_rightJoystickY, 2));
		frc::SmartDashboard::PutNumber("Right Command  : ", round(m_driveTrain.GetRightTarget(), 2));
		frc::SmartDashboard::PutNumber("Right Speed    : ", round(m_driveTrain.GetRightSpeed(), 2));
		frc::SmartDashboard::PutNumber("Right Position : ", round(m_driveTrain.GetRightPosition(), 2));
		frc::SmartDashboard::PutNumber("Right Enc. Pos.: ", round(m_driveTrain.GetRightEncoderPos(), 2));
		frc::SmartDashboard::PutNumber("Right Enc. Vel.: ", round(m_driveTrain.GetRightEncoderVel(), 2));

		frc::SmartDashboard::PutNumber("GearLift Up    : ", m_gearLift.IsUp());
		frc::SmartDashboard::PutNumber("GearLift Down  : ", m_gearLift.IsDown());
		frc::SmartDashboard::PutNumber("GearLift Clamp : ", m_gearLift.IsClamped());
		frc::SmartDashboard::PutNumber("Winch Trigger  : ", round(m_leftTrigger, 2));

		frc::SmartDashboard::PutNumber("Auto State     : ", m_autoState);
		frc::SmartDashboard::PutNumber("TraverseIndex  : ", m_traverseIndex);
		frc::SmartDashboard::PutNumber("TraverseState  : ", m_traverseState);


		frc::SmartDashboard::PutNumber("Start Position : ", round(m_driveTrain.GetStartPosition(), 2));
		frc::SmartDashboard::PutNumber("End Position   : ", round(m_driveTrain.GetEndPosition(), 2));
		frc::SmartDashboard::PutNumber("Delta Position : ", round(m_driveTrain.GetDeltaPosition(), 2));

		frc::SmartDashboard::PutNumber("Gyro Angle     : ", round(m_gyroAngle, 2));
		frc::SmartDashboard::PutNumber("Start Angle    : ", round(m_driveTrain.GetStartPosition(), 2));
		frc::SmartDashboard::PutNumber("End Angle      : ", round(m_driveTrain.GetEndPosition(), 2));
		frc::SmartDashboard::PutNumber("Delta Angle    : ", round(m_driveTrain.GetDeltaPosition(), 2));
	}

	double round(double value, int numDecimals)
	{
		return trunc(value * pow(10, numDecimals)) / pow(10, numDecimals);
	}



};


START_ROBOT_CLASS(Robot)
