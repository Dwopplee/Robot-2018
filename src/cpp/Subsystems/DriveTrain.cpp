// Copyright (c) 2016-2018 FRC Team 3512. All Rights Reserved.

#include "Subsystems/DriveTrain.hpp"

#include <cmath>
#include <iostream>
#include <limits>

#include "Robot.hpp"

DriveTrain::DriveTrain() {
    m_drive.SetDeadband(kJoystickDeadband);

    m_leftEncoder.SetDistancePerPulse(kDpP);
    m_rightEncoder.SetDistancePerPulse(kDpP);

    m_leftGrbx.Set(0.0);
    m_rightGrbx.Set(0.0);

    m_leftEncoder.SetReverseDirection(false);
    m_rightEncoder.SetReverseDirection(true);

    m_controller.GetPositionPID().SetPID(kPosP, kPosI, kPosD);
    m_controller.GetAnglePID().SetPID(kAngleP, kAngleI, kAngleD);

    m_controller.SetPositionTolerance(1.5, 0.5);
    m_controller.SetAngleTolerance(1.0, 1.75);
}

int32_t DriveTrain::GetLeftRaw() const { return m_leftEncoder.GetRaw(); }

int32_t DriveTrain::GetRightRaw() const { return m_rightEncoder.GetRaw(); }

void DriveTrain::Drive(double throttle, double turn, bool isQuickTurn) {
    m_drive.CurvatureDrive(throttle, -turn, isQuickTurn);
}

void DriveTrain::ResetEncoders() {
    m_leftEncoder.Reset();
    m_rightEncoder.Reset();
}

void DriveTrain::SetLeftManual(double value) { m_leftGrbx.Set(value); }

void DriveTrain::SetRightManual(double value) { m_rightGrbx.Set(value); }

double DriveTrain::GetLeftDisplacement() const {
    return m_leftEncoder.GetDistance();
}

double DriveTrain::GetRightDisplacement() const {
    return m_rightEncoder.GetDistance();
}

double DriveTrain::GetLeftRate() const { return m_leftEncoder.GetRate(); }

double DriveTrain::GetRightRate() const { return m_rightEncoder.GetRate(); }

double DriveTrain::GetPosition() { return m_controller.GetPosition(); }

double DriveTrain::GetAngle() { return m_controller.GetAngle(); }

double DriveTrain::GetAngularRate() const { return m_gyro.GetRate(); }

void DriveTrain::StartClosedLoop() {
    m_controller.Enable();
    m_drive.SetSafetyEnabled(false);
}

void DriveTrain::StopClosedLoop() {
    m_controller.Disable();
    m_drive.SetSafetyEnabled(true);
}

void DriveTrain::SetPositionGoal(double position) {
    m_posRef.SetGoal(position);
}

void DriveTrain::SetAngleGoal(double angle) { m_angleRef.SetGoal(angle); }

double DriveTrain::GetPosReference() {
    return m_posRef.GetPositionNode().GetOutput();
}

double DriveTrain::GetAngleReference() {
    return m_angleRef.GetPositionNode().GetOutput();
}

double DriveTrain::GetPositionGoal() const { return m_posRef.GetGoal(); }

double DriveTrain::GetAngleGoal() const { return m_angleRef.GetGoal(); }

bool DriveTrain::AtPositionGoal() const { return m_controller.AtPosition(); }

bool DriveTrain::AtAngleGoal() const { return m_controller.AtAngle(); }

double DriveTrain::PositionProfileTimeTotal() const {
    return m_posRef.ProfileTimeTotal();
}

double DriveTrain::AngleProfileTimeTotal() const {
    return m_angleRef.ProfileTimeTotal();
}

void DriveTrain::ResetGyro() { m_gyro.Reset(); }

void DriveTrain::CalibrateGyro() { m_gyro.Calibrate(); }

void DriveTrain::Debug() {
    std::cout << "Left Pos: " << m_leftEncoder.GetDistance()
              << " Right Pos: " << m_rightEncoder.GetDistance() << std::endl;
    m_controller.Debug();
}

void DriveTrain::HandleEvent(Event event) {
    if (Robot::driveStick1.GetRawButton(1)) {
        Drive(Robot::driveStick1.GetY() * 0.5, Robot::driveStick2.GetX() * 0.5,
              Robot::driveStick2.GetRawButton(2));
    } else {
        Drive(Robot::driveStick1.GetY(), Robot::driveStick2.GetX(),
              Robot::driveStick2.GetRawButton(2));
    }
}
