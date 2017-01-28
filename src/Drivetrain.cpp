/*
 * Drivetrain.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: Developer
 */

#include <Drivetrain.h>

Drivetrain::Drivetrain()
{
	gyro.Calibrate();
}

Drivetrain::~Drivetrain()
{
}

void Drivetrain::Stop()
{
	m_rightMotor1.Set(0.0);
	m_leftMotor1.Set(0.0);
	m_rightMotor2.Set(0.0);
	m_leftMotor2.Set(0.0);
}

void Drivetrain::Update(const double speedLimit)
{
	double leftValue = controller.GetY(frc::GenericHID::kLeftHand);
	double rightValue = controller.GetY(frc::GenericHID::kRightHand);

	m_rightMotor1.Set(rightValue * speedLimit);
	m_leftMotor1.Set(-leftValue * speedLimit);

	m_rightMotor2.Set(rightValue * speedLimit);
	m_leftMotor2.Set(-leftValue * speedLimit);
}

double Drivetrain::GetControllerValue(frc::GenericHID::JoystickHand hand)
{
	return controller.GetY(hand);
}

double Drivetrain::GetGyroAngle()
{
	return gyro.GetAngle();
}
