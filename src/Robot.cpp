#include "WPILib.h"
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

	CANTalon Zero;
	CANTalon One;
	CANTalon Two;
	CANTalon Three;
	RobotDrive myRobot; // robot drive system
	Joystick driverJoystick; // only joystick
	Solenoid hands;
	Solenoid arms;
	Solenoid shifter;
	BuiltInAccelerometer roboRIO;




public:
	Robot() :
			Zero(0),
			One(1),
			Two(2),
			Three(3),
			myRobot(Zero, One, Two, Three),	// initialize the RobotDrive to use motor controllers on ports 0 and 1
			driverJoystick(0),
			hands(0),
			arms(1),
			shifter(2),
			roboRIO()


	{
		myRobot.SetExpiration(0.1);
		hands.Set(0);
		arms.Set(0);
		shifter.Set(0);
	}

	/**
	 * Runs the motors with arcade steering.
	 */
	void OperatorControl()
	{
		bool armCheck = 0;
		bool armPrevious = 0;
		bool armState = 0;
		float xAxis;
		float yAxis;
		float zAxis;
		float xAxisScaler = 0;
		float xAxisMax = 0;
		float xAxisMin = 0;


		while (IsOperatorControl() && IsEnabled())
		{
			myRobot.ArcadeDrive(driverJoystick.GetRawAxis(1), driverJoystick.GetRawAxis(4)); // drive with arcade style (use right stick)
			hands.Set(driverJoystick.GetRawButton(1));
			shifter.Set(driverJoystick.GetRawButton(6));

			if(driverJoystick.GetRawButton(5)&&driverJoystick.GetRawButton(3))
			{
				armCheck = 1;
			}
			else
			{
				armCheck = 0;
			}

			if((armCheck == 1)&&(armCheck != armPrevious))
			{
				armState = ! armState;
			}

			arms.Set(armState);

			xAxis = roboRIO.GetX();
			yAxis = roboRIO.GetY();
			zAxis = roboRIO.GetZ();

			if(xAxis>=xAxisMax)
			{
				xAxisMax = xAxis;
			}
			if(xAxis<=xAxisMax)
			{
				xAxisMin = xAxis;
			}

			if(xAxisMax >= !xAxisMin)
			{
				xAxisScaler = (1/xAxisMax) ;
			}
			else
			{
				xAxisScaler = (1/!xAxisMin);
			}

			//test.Set((xAxisScaler*xAxis));

			//cout<<"X Axis"<<xAxis<<"    Y Axis"<<yAxis<<"      Z Axis"<<zAxis<<endl;
			//printf("test");

			SmartDashboard::PutNumber("X Axis", xAxis);
			SmartDashboard::PutNumber("Y Axis", yAxis);
			SmartDashboard::PutNumber("Z Axis", zAxis);

			Wait(0.005);				// wait for a motor update time
		}
	} //There once was a man named steven who enjoyed giving chris his "Jobs". Then connor came along and exclaimed to sir steven "Please! I would like to give chris one job of my own!" But when steven heard what mister connor has just said, he attacks. He took off his shirt, pulled the spray paint out of his shiny golden wagon which has the slightest bit of rust near the rear left wheel and starts to spray paint a star pattern onto his nipples. But when sir steven began to spray paint his left nipple, he ran out of paint. This infuriated sir steven, which made mister connor laugh. Then Sir Steven noticed something out of the corner of his eye and screamed for his life, "Oh my good god sir steven! The mighty Bryce Dragon, the fattest, most dick of a dragon of all!" Sir Steven unsheathed his sword and pointed the beautifully crafted piece of craftsmanship to the sky in the direction of the obese dragon. The Bryce Dragon then proceded to attack the gay couple. But when the 900,000 pound dragon swooped in to pick up Sir Steven with his sharp talons, he forgot sir steven's sword was still raised in the air! Sir Steven accidently sliced the belly of the obese dragon! The dragon whispered near death to the couple, "I did not mean to frighten thou steven, but you have killed me. Please complete my task in life and kill the faggot Jonas. He doesn't deserve to live, nor eat the food of the Obese Dragon!" The dragon then died in vein. The couple, determined to make it up to the dead dragon, then made a 36 hour quest, making frequent stops to kiss, to the cave of the flaming faggot. When they were outside of the cave, Sir Steven yelled out of pain, "Holy Cats mister Connor! This cave smells like rotten mutton chops blended in ass!" They proceded to enter the cave and meet the faggot Jonas. "FEAR ME YOUNG COUPLE! I HAVE THE POWERS OF SMELLY PEANUT BUTTER AND BG ON MY SIDE" The faggot shouted. Mister Connor then said, "But velocity is represented by a 'v'" The faggot died. The ghost of the obese dragon appeared and said to the couple "Thanks you gay guys, you killed him. I shall give you the keys to my buick. I bid you good day. Stay gay. Horses go 'Nay'" FIN."-Chris
};

START_ROBOT_CLASS(Robot);
