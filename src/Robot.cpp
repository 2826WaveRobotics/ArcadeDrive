#include "WPILib.h"
#include "Math.h"
using namespace std;

/**
 * This is a demo program showing the use of the RobotDrive class.
 * The SampleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're inexperienced,
 * don't. Unless you know what you are doing, complex code will be much more difficult under
 * this system. Use IterativeRobot or Command-Based instead if you're new.
 */
class Robot: public SampleRobot
{

	CANTalon Zero;//Initializing SRXs for drive train use
	CANTalon One;
	CANTalon Two;
	CANTalon Three;
	RobotDrive myRobot; // robot drive system
	Joystick driverJoystick; // driver's XBox controller
	Joystick arduino;
	Solenoid hands;//first stage of climbing mechanism
	Solenoid arms;//solenoid used to engage the PTO for second stage of climb
	Solenoid shifter;//duh
	BuiltInAccelerometer roboRIO;//kinda also duh, but if you don't really know, the RoboRIO has a built in accelerometer. This is it

	float oldMotorValueMove;//forward(or backward) value of the motors from the previous iteration of the code
	float oldMotorValueRotate;//rotational value of the motors from the previous iteration of the code
	int loopcounter;//counter used in our motor value comparing loop


public:
	Robot() :
			Zero(0),//initalizing everything to the proper USB, CAN, DI/O, or PWM channels
			One(1),
			Two(2),
			Three(3),
			myRobot(Zero, One, Two, Three),	// initialize the RobotDrive to use the SRXs
			driverJoystick(0),
			arduino(1),
			hands(2),
			arms(0),
			shifter(1),
			roboRIO()//only one, so no initalizing needed


	{
		myRobot.SetExpiration(0.1);//setting inital(and default) values for all of the systems
		hands.Set(0);
		arms.Set(0);
		shifter.Set(0);
		oldMotorValueMove=0;
		oldMotorValueRotate=0;
		loopcounter=0;
	}


	void OperatorControl()
	{
		//initalizing values used primarialy in Tele-Op
		bool armCheck = 0;//value used to check to see if both nessicary buttons are pressed
		bool armPrevious = 0;//value of armCheck set in previous iteration
		bool armState = 0;//position of arm(engaged or not)
		float xAxis;//acceleration(in g's) of the X Axis of the RoboRIO's onboard accelerometer
		float yAxis;//acceleration(in g's) of the Y Axis of the RoboRIO's onboard accelerometer
		float zAxis;//acceleration(in g's) of the Z Axis of the RoboRIO's onboard accelerometer
		bool checkB3;//stores the value of button 3 from the previous iteration
		bool b3Good;//the state of b3 changed from not pressed to pressed
		bool b3;//button 3

		while (IsOperatorControl() && IsEnabled())
		{

			double temp2 = arduino.GetRawAxis(2);
			double temp3 = arduino.GetRawAxis(3);
			double temp4 = arduino.GetRawAxis(4);


			float newMotorValueMove=driverJoystick.GetRawAxis(1);//Makes it go forward
			float newMotorValueRotate=driverJoystick.GetRawAxis(4);// Rotates the robot
			b3 = driverJoystick.GetRawButton(3);//store the state of button 3


			loopcounter += 1;// This loop changes the speed
			if (loopcounter >= 3)
			{
				float limiter=.1;// Maximum change in speed

				if(newMotorValueMove>oldMotorValueMove) //trying to move forward
				{
					if (fabs(newMotorValueMove-oldMotorValueMove)>limiter)// Change higher then the limiter
					{
						newMotorValueMove=oldMotorValueMove+limiter; //set change to limit
					}

				}
				else if (newMotorValueMove<oldMotorValueMove) //Trying to slow down
				{
					if (fabs(oldMotorValueMove-newMotorValueMove)>limiter)//Change greater then limiter
					{
						newMotorValueMove=oldMotorValueMove-limiter;//Set change to limiter
					}
				}

				if (newMotorValueRotate>oldMotorValueRotate)//Trying to rotate
				{
					if (fabs(newMotorValueRotate-oldMotorValueRotate)>limiter)//Change greater then limiter
					{
						newMotorValueRotate=oldMotorValueRotate+limiter;//Set change to limiter
					}
				}
				else if (newMotorValueRotate<oldMotorValueRotate)//Trying to rotate
				{
					if (fabs(oldMotorValueRotate-newMotorValueRotate)>limiter)//Change greater then limiter
					{
						newMotorValueRotate=oldMotorValueRotate-limiter;//Set change to limiter
					}
				}
				loopcounter = 0; //resetting loopcounter
				oldMotorValueMove=newMotorValueMove; //saving new value to compare for next time
				oldMotorValueRotate=newMotorValueRotate;
			}

			if(0 == driverJoystick.GetRawAxis(1))//Drivers forward axis is 0
			{
				oldMotorValueMove= 0;//Set move value to 0
				newMotorValueMove=0;
			}
			if(0 == driverJoystick.GetRawAxis(4))//Drivers rotation axis is 0
			{
				oldMotorValueRotate=0;//Set move value to 0
				newMotorValueRotate=0;
			}




			myRobot.ArcadeDrive(oldMotorValueMove, oldMotorValueRotate); // drive with arcade style (use right stick)
			hands.Set(driverJoystick.GetRawButton(1));//Extend hands when button 1 is pressed
			shifter.Set(driverJoystick.GetRawButton(6));//Shift when button 6 is pressed

			if((b3 == 1)&& (b3 != checkB3))//button 3 has changed from not pressed to pressed
			{
				b3Good = 1;
			}
			else
			{
				b3Good = 0;
			}

			if(driverJoystick.GetRawButton(5)&&b3Good)//Check if button 5 and 3 is pressed
			{
				armState = ! armState;//Change state of arm (In to out/out to in)
			}

			arms.Set(armState);//Engage or disengage the arms

			xAxis = roboRIO.GetX();
			yAxis = roboRIO.GetY();
			zAxis = roboRIO.GetZ();

			SmartDashboard::PutNumber("Fwd/Rev MotorOutput", oldMotorValueMove);//Write forward motor value to the dashboard
			SmartDashboard::PutNumber("Rotate MotorOutput", oldMotorValueRotate);//Write rotational motor value to the dashboard
			SmartDashboard::PutNumber("X Axis", xAxis);//Write x-axis acceleration to the dashboard
			SmartDashboard::PutNumber("Y Axis", yAxis);//Write y-axis acceleration to the dashboard
			SmartDashboard::PutNumber("Z Axis", zAxis);//Write z-axis acceleration to the dashboard

			SmartDashboard::PutNumber("Arduino 1", temp1);
			SmartDashboard::PutNumber("Arduino 2", temp2);
			SmartDashboard::PutNumber("Arduino 3", temp3);
			SmartDashboard::PutNumber("Arduino 4", temp4);
			SmartDashboard::PutNumber("Arduino 5", temp5);

			checkB3 = b3;

			Wait(0.01);				// wait for a motor update time
		}
	} //There once was a man named steven who enjoyed giving chris his "Jobs". Then connor came along and exclaimed to sir steven "Please! I would like to give chris one job of my own!" But when steven heard what mister connor has just said, he attacks. He took off his shirt, pulled the spray paint out of his shiny golden wagon which has the slightest bit of rust near the rear left wheel and starts to spray paint a star pattern onto his nipples. But when sir steven began to spray paint his left nipple, he ran out of paint. This infuriated sir steven, which made mister connor laugh. Then Sir Steven noticed something out of the corner of his eye and screamed for his life, "Oh my good god sir steven! The mighty Bryce Dragon, the fattest, most dick of a dragon of all!" Sir Steven unsheathed his sword and pointed the beautifully crafted piece of craftsmanship to the sky in the direction of the obese dragon. The Bryce Dragon then proceded to attack the gay couple. But when the 900,000 pound dragon swooped in to pick up Sir Steven with his sharp talons, he forgot sir steven's sword was still raised in the air! Sir Steven accidently sliced the belly of the obese dragon! The dragon whispered near death to the couple, "I did not mean to frighten thou steven, but you have killed me. Please complete my task in life and kill the faggot Jonas. He doesn't deserve to live, nor eat the food of the Obese Dragon!" The dragon then died in vein. The couple, determined to make it up to the dead dragon, then made a 36 hour quest, making frequent stops to kiss, to the cave of the flaming faggot. When they were outside of the cave, Sir Steven yelled out of pain, "Holy Cats mister Connor! This cave smells like rotten mutton chops blended in ass!" They proceded to enter the cave and meet the faggot Jonas. "FEAR ME YOUNG COUPLE! I HAVE THE POWERS OF SMELLY PEANUT BUTTER AND BG ON MY SIDE" The faggot shouted. Mister Connor then said, "But velocity is represented by a 'v'" The faggot died. The ghost of the obese dragon appeared and said to the couple "Thanks you gay guys, you killed him. I shall give you the keys to my buick. I bid you good day. Stay gay. Horses go 'Nay'" FIN."-Chris
};

START_ROBOT_CLASS(Robot);
