
/*IEDC Project Ultimate Vehicle Security By Team HDR.
Coded BY  ***** HARDIQ VERMA ****
Project Started at Date 02 April 2018
Find me at www.hardiqverma.com

check our team Website for more
www.mechnorobs.com
*/

// TO add delay of 5 Seconds in the Code of rfid matching for enabling the user to place the key inside the key assembly. Check the notes for information in the comments of the Rfid matching code.
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include "call.h"
#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>

#define RFID_SS_PIN  10
#define RFID_RST_PIN 9

#define ECU_Relay A0
#define Lock_Servo 6

#define Wireless_Signal_Receive A1
#define Wireless_VT_PIN 7

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

MFRC522::MIFARE_Key;

CallGSM call; // objects
SMSGSM sms;

Servo LockServo;

int code[] = { 4, 108, 222, 66, 236, 76, 128 }; //This is the stored UID
int codeRead = 0;
String uidString;

char number[20];  // To store the phone number of the person 
byte stat = 0;

boolean Lock_status = true;  // Intially assume that the car is locked.
//boolean Wireless_Verification_Status = false;
//boolean Check_call_Status = false;

int count = 0;

void setup() 
{
	Serial.begin(9600);
	gsm.begin(19200);
	SPI.begin();
	rfid.PCD_Init();

	pinMode(Wireless_Signal_Receive, INPUT);
	pinMode(Wireless_VT_PIN, INPUT);

	pinMode(ECU_Relay, OUTPUT);

	digitalWrite(ECU_Relay, LOW);

	LockServo.attach(Lock_Servo);

	LockServo.write(0);  // Change as per the written angles on the Door of the car
}

void loop()
{
	while (Lock_status)
	{
		Serial.println(count);
		check_Wireless_Verification();
		check_call();

		count++;
	}
	Serial.println("Car is unlocked");
}
void check_Wireless_Verification()
{
	if (digitalRead(Wireless_VT_PIN) == HIGH && digitalRead(Wireless_Signal_Receive) == LOW)
	{
		// Add delay Here to enable user to place the key inside the assembly of generally 5 Seconds.
		Serial.println("Wireless verification Success");
		if (rfid.PICC_IsNewCardPresent())
		{
			rfid.PICC_ReadCardSerial();
			MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
			//printDec(rfid.uid.uidByte, rfid.uid.size);

			uidString = String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]) + " " + String(rfid.uid.uidByte[4]) + " " + String(rfid.uid.uidByte[5]) + " " + String(rfid.uid.uidByte[6]);

			int i = 0;
			boolean match = true;
			while (i < rfid.uid.size)
			{
				if (!(rfid.uid.uidByte[i] == code[i]))
				{
					match = false;
				}
				i++;
			}

			if (match)
			{
				Serial.println("\nI know this card!");// Serial Debugging
				digitalWrite(ECU_Relay, HIGH);
				LockServo.write(180);
				delay(5000);
				LockServo.write(0);
				Lock_status = false;
			}
			else
			{
				Serial.println("\nUnknown Card"); //Serial debugging
				digitalWrite(ECU_Relay, LOW);
				LockServo.write(0); // Door Lock Closed And Car is immobolized.

			}

			// Halt PICC
			rfid.PICC_HaltA();

			// Stop encryption on PCD
			rfid.PCD_StopCrypto1();
		}
		else
		{
			Serial.println("No RFID TAG Detected"); //Serial debugging
			digitalWrite(ECU_Relay, LOW);
			LockServo.write(0); // Door Lock Closed And Car is immobolized.
		}
	}
	else
	{
		Serial.println("Wireless Verification Failed");
		digitalWrite(ECU_Relay, LOW);
		LockServo.write(0); // Door Lock Closed And Car is immobolized.
	}
}
void check_call()
{
	    stat = call.CallStatusWithAuth(number, 17, 20);
		if (stat == CALL_INCOM_VOICE_AUTH)
		{
			call.HangUp();
			delay(2000);
			sms.SendSMS(number, "You are authorised and Car is opened for You.");

			Serial.println("Opened"); //uncomment for serial debugging.

			digitalWrite(ECU_Relay, HIGH);
			LockServo.write(180);
			delay(5000);
			LockServo.write(0);
			Lock_status = false;
		}
		if (stat == CALL_INCOM_VOICE_NOT_AUTH)
		{
			Serial.println("Locked");
			digitalWrite(ECU_Relay, LOW);
			LockServo.write(0);
			sms.SendSMS(number, "You are not authorised and Car is locked for You.");
			sms.SendSMS("+917087969184", "Someone tried to unlock the car. The car is locked for security.");
		}
		delay(1000);
}


