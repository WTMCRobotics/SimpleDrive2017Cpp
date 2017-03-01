#ifndef ROBOT_DEFS_H_
#define ROBOT_DEFS_H_

// Power Distribution Panel
//
#define PDP_CHANNEL_RIGHTMASTER	0
#define PDP_CHANNEL_RIGHTSLAVE	1
#define PDP_CHANNEL_LEFTSLAVE	2
#define PDP_CHANNEL_LEFTMASTER	3
#define PDP_CHANNEL_WINCH		15
#define PDP_CHANNEL_GEARLIFT	14

// CAN Bus IDs
//
#define CAN_ID_RIGHTMASTER	4
#define CAN_ID_RIGHTSLAVE	3
#define CAN_ID_LEFTSLAVE	2
#define CAN_ID_LEFTMASTER	1

// Motor Parameters
//
#define STALL_CURRENT_WINCH 		4.0
#define STALL_CURRENT_GEAR_LIFT		4.0

// ToDo: test deadband value
#define DRIVE_COMMAND_DEADBAND		0.1
#define DRIVE_COMMAND_DEADBAND		0.1
#define DRIVE_RAMP_VoltsPerSec		12.0
#define DRIVE_PID_P_GAIN			0.1
#define DRIVE_PID_I_GAIN			0.0
#define DRIVE_PID_D_GAIN			0.005
#define DRIVE_ENCDR_STEPS			2048
#define DRIVE_GEAR_RATIO			12.75

#define GEARLIFT_COMMAND_DEADBAND	0.2

// Digital Inputs
//
#define	DIO_SWITCH_GEARLIFT_DOWN	0
#define	DIO_SWITCH_GEARLIFT_UP		1
#define	DIO_PRACTICE_ROBOT			9

// Pneumatic Control Module
//
#define PCM_ID					0
#define PCM_CHANNEL_GEAR_CLAMP 		0
#define PCM_CHANNEL_GEAR_RELEASE 	1
#define PCM_CHANNEL_CAMERA_LIGNTS 	2

// Autonomous mode constants
//
#define AUTO_MOVE_MAX_SEGMENTS	3

// starting location 1 (LEFT lane for Red Alliance, RIGHT for Blue Alliance)
#define kStart1Angle_0		+0.0
#define kStart1Dist_0		  4.00
#define kStart1SpeedLf_0	  50.0
#define kStart1SpeedRt_0	  50.0
#define kStart1Angle_1		60.0
#define kStart1Dist_1		  1.00
#define kStart1SpeedLf_1	  50.0
#define kStart1SpeedRt_1	  50.0

// starting location 1 (CENTER lane for both Alliances)
#define kStart2Angle_0		+0.0
#define kStart2Dist_0		  0.5
#define kStart2SpeedLf_0	  250.0
#define kStart2SpeedRt_0	  250.0
#define kStart2Angle_1		+45.0
#define kStart2Dist_1		  0.5
#define kStart2SpeedLf_1	  250.0
#define kStart2SpeedRt_1	  250.0
#define kStart2Angle_2		-45.0
#define kStart2Dist_2		  3.0
#define kStart2SpeedLf_2	  250.0
#define kStart2SpeedRt_2	  250.0

// starting location 3 (RIGHT lane for Red Alliance, LEFT for Blue Alliance)
#define kStart3Angle_0		+0.0
#define kStart3Dist_0		  4.75
#define kStart3SpeedLf_0	  200.0
#define kStart3SpeedRt_0	  200.0
#define kStart3Angle_1		-60.0
#define kStart3Dist_1		  2.8
#define kStart3SpeedLf_1	  200.0
#define kStart3SpeedRt_1	  200.0


#endif	/* ROBOT_DEFS_H_ */
