

/*IEDC Project Ultimate Vehicle Security By Team HDR.
Coded BY  ***** HARDIQ VERMA ****
Project Started at Date 11 March 2018
Find me at www.hardiqverma.com

check our team Website for more
www.mechnorobs.com
*/

#include <FPS_GT511C3.h>
#include <SoftwareSerial.h>

#define EnrollButton A1
#define SpecialFunctionButton A0

#define Wireless_Signal 12

#define LED 13

int MasterDataID[50] = {};
int TemporaryEnrollID[150] = {};
int Button_case;
int enrollMasterid = 1;
int enrollTemporaryid = 50;
int check_entry;
bool status = true;
unsigned long CurrentMills;

FPS_GT511C3 fps(4, 5);

void setup()
{
	pinMode(EnrollButton, INPUT_PULLUP);
	pinMode(SpecialFunctionButton, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
	pinMode(Wireless_Signal, OUTPUT);
	digitalWrite(Wireless_Signal, HIGH);
	Serial.begin(9600);
	fps.Open();
	fps.SetLED(false);
	delay(100);
}

void loop()
{
	CurrentMills = millis();
	if (CurrentMills < 10000)
	{
		if (status == true)
		{
			fps.SetLED(true);
			if (fps.IsPressFinger())
			{
				fps.CaptureFinger(false);
				check_entry = fps.Identify1_N();
				if (check_entry < 200)
				{
					digitalWrite(Wireless_Signal, LOW);
					fps.SetLED(false);
					status = false;
					//Serial.println("OK");
				}
				else
				{
					digitalWrite(Wireless_Signal, HIGH);
					//Serial.println("Failed");
				}
			}
		}
		Button_case = Check_Button();
		switch (Button_case)
		{
		case 0:
		{
			EnrollTemporary();
			break;
		}
		case 1:
		{
			DeleteAllTemporary();
			break;
		}
		case 2:
		{
			EnrollMaster();
			break;
		}
		}
	}
	else if (CurrentMills >= 10000)
	{
		fps.SetLED(false);
	}
}

int Check_Button()
{
	int data;
	if (digitalRead(EnrollButton) == 0 && digitalRead(SpecialFunctionButton) == 1)
	{
		delay(100);
		if (digitalRead(EnrollButton) == 0 && digitalRead(SpecialFunctionButton) == 1)
		{
			return data = 0;// temporary enrollment
		}

	}
	else if (digitalRead(EnrollButton) == 0 && digitalRead(SpecialFunctionButton) == 0)
	{
		delay(100);
		if (digitalRead(EnrollButton) == 0 && digitalRead(SpecialFunctionButton) == 0)
		{
			return data = 1;// delete all temporary
		}
	}
	else if (digitalRead(EnrollButton) == 1 && digitalRead(SpecialFunctionButton) == 0)
	{
		delay(100);
		if (digitalRead(EnrollButton) == 1 && digitalRead(SpecialFunctionButton) == 0)
		{
			return data = 2;// Master Enrollment
		}
	}
	else
	{
		return data = 3;
	}
}

void EnrollTemporary()
{
	int verification_id;
	Serial.println("Temporary Enrollment");
	fps.SetLED(true);
	bool MasterStatus = false;
	bool MasterData = false;
	Serial.println("Enter Master Finger");
	int i = 0;
	while (i < 100)
	{
		if (fps.IsPressFinger())
		{
			fps.CaptureFinger(false);
			verification_id = fps.Identify1_N();
			if (verification_id < 50)
			{
				Serial.println("Master Verified");
				MasterData = true;
				break;
			}
			else
			{
				Serial.println("Master is not Verified");
				break;
			}
			i = 105;
		}
		else
		{
			i++;
		}
	}
	fps.SetLED(false);
	delay(2000);
	if (MasterData)
	{
		fps.SetLED(true);
		bool usedid = true;
		while (usedid == true)
		{
			usedid = fps.CheckEnrolled(enrollTemporaryid);
			if (usedid == true)
			{
				enrollTemporaryid++;
			}
		}
		fps.EnrollStart(enrollTemporaryid);
		// enroll
		Serial.println("Temporary Enrollment");
		Serial.print("Press finger to Enroll #");
		Serial.println(enrollTemporaryid);
		int j = 0;
		while (j < 100)
		{
			if (fps.IsPressFinger())
			{
				while (fps.IsPressFinger() == false) delay(100);
				bool bret = fps.CaptureFinger(true);
				int iret = 0;
				if (bret != false)
				{
					Serial.println("Remove finger");
					fps.Enroll1();
					while (fps.IsPressFinger() == true) delay(100);
					Serial.println("Press same finger again");
					while (fps.IsPressFinger() == false) delay(100);
					bret = fps.CaptureFinger(true);
					if (bret != false)
					{
						Serial.println("Remove finger");
						fps.Enroll2();
						while (fps.IsPressFinger() == true) delay(100);
						Serial.println("Press same finger yet again");
						while (fps.IsPressFinger() == false) delay(100);
						bret = fps.CaptureFinger(true);
						if (bret != false)
						{
							Serial.println("Remove finger");
							iret = fps.Enroll3();
							if (iret == 0)
							{
								Serial.println("Enrolling Successful");
							}
							else
							{
								Serial.print("Enrolling Failed with error code:");
								Serial.println(iret);
							}
						}
						else
						{
							Serial.println("Failed to capture third finger");
						}
					}
					else
					{
						Serial.println("Failed to capture second finger");
					}
				}
				else
				{
					Serial.println("Failed to capture first finger");
				}

				j = 105;
			}
			else
			{
				j++;
			}
		}
	}
	fps.SetLED(false);
}
void DeleteAllTemporary()
{
	Serial.println("delete mode On");
	int verification_id;
	fps.SetLED(true);
	bool MasterStatus = false;
	bool MasterData = false;
	Serial.println("Enter Master Finger");
	int i = 0;
	while (i < 100)
	{
		if (fps.IsPressFinger())
		{
			fps.CaptureFinger(false);
			verification_id = fps.Identify1_N();
			if (verification_id < 50)
			{
				MasterData = true;
				Serial.println("Master Verified");
				break;
			}
			else
			{
				Serial.println("Master is not Verified");
				break;
			}
			i = 105;
		}
		else
		{
			i++;
		}
	}
	bool data = 1;
	if (MasterData)
	{
		for (int i = 50; data != 0; i++)
		{
			data = fps.DeleteID(i);
			Serial.println(data); // Delete all temporary enrollments and print their status;
		}
		fps.SetLED(false);
	}
	else
	{
		fps.SetLED(false);
	}
}
void EnrollMaster()
{
	int verification_id;
	Serial.println("Master Enrollment");
	fps.SetLED(true);
	bool MasterStatus = false;
	bool MasterData = false;
	Serial.println("Enter Master Finger");
	int i = 0;
	while (i < 100)
	{
		if (fps.IsPressFinger())
		{
			fps.CaptureFinger(false);
			verification_id = fps.Identify1_N();
			if (verification_id < 50)
			{
				MasterData = true;
				Serial.println("Master Verified");
				break;
			}
			else
			{
				Serial.println("Master is not Verified");
				break;
			}
			i = 105;
		}
		else
		{
			i++;
		}
	}
	fps.SetLED(false);
	delay(2000);
	if (MasterData)
	{
		fps.SetLED(true);
		bool usedid = true;
		while (usedid == true)
		{
			if (enrollMasterid < 50)
			{
				usedid = fps.CheckEnrolled(enrollMasterid);
				if (usedid == true)
				{
					enrollMasterid++;
				}
			}
			else
			{
				usedid == false;
			}
		}
		fps.EnrollStart(enrollMasterid);
		// enroll
		Serial.println("Master Enrollment");
		Serial.print("Press finger to Enroll #");
		Serial.println(enrollMasterid);
		int j = 0;
		while (j < 100)
		{
			if (fps.IsPressFinger())
			{
				while (fps.IsPressFinger() == false) delay(100);
				bool bret = fps.CaptureFinger(true);
				int iret = 0;
				if (bret != false)
				{
					Serial.println("Remove finger");
					fps.Enroll1();
					while (fps.IsPressFinger() == true) delay(100);
					Serial.println("Press same finger again");
					while (fps.IsPressFinger() == false) delay(100);
					bret = fps.CaptureFinger(true);
					if (bret != false)
					{
						Serial.println("Remove finger");
						fps.Enroll2();
						while (fps.IsPressFinger() == true) delay(100);
						Serial.println("Press same finger yet again");
						while (fps.IsPressFinger() == false) delay(100);
						bret = fps.CaptureFinger(true);
						if (bret != false)
						{
							Serial.println("Remove finger");
							iret = fps.Enroll3();
							if (iret == 0)
							{
								Serial.println("Enrolling Successful");
							}
							else
							{
								Serial.print("Enrolling Failed with error code:");
								Serial.println(iret);
							}
						}
						else
						{
							Serial.println("Failed to capture third finger");
						}
					}
					else
					{
						Serial.println("Failed to capture second finger");
					}
				}
				else
				{
					Serial.println("Failed to capture first finger");
				}

				j = 105;
			}
			else
			{
				j++;
			}
		}
	}
	fps.SetLED(false);
}
