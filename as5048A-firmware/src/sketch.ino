/*
 *				ATMEL ATTINY44 / ARDUINO
 *
 *                          +-\/-+
 *                    VCC  1|    |14  GND
 *            (D 10)  PB0  2|    |13  AREF (D  0)
 *            (D  9)  PB1  3|    |12  PA1  (D  1) 
 *                    PB3  4|    |11  PA2  (D  2) 
 * PWM  INT0  (D  8)  PB2  5|    |10  PA3  (D  3) 
 * PWM        (D  7)  PA7  6|    |9   PA4  (D  4) 
 * PWM        (D  6)  PA6  7|    |8   PA5  (D  5)        PWM*
 *
 * Mappings for Angle Sensor Board (referenced against arduino pins)
 * 0 : LED NEAR
 * 1 : LED_FAR
 * 2 : LED FIELD
 * 3 : CSn
 * 4 : CLK
 * 5 : MISO
 * 6 : MOSI
 * 7 : NC
 * 8 : NC
 * 9 : NC
 * 10 : Internal/External Switch
 *
 */

#include <AS5048A.h>

#define LED_POWER 7
#define LED_NEAR 1
#define LED_FAR 0
#define LED_GOOD 2
#define ANGLE_CSn 3
#define ANGLE_CLK 4
#define ANGLE_MISO 5
#define ANGLE_MOSI 6
#define INT_EXT_SW 10

AS5048A angleSensor(ANGLE_CSn, ANGLE_MOSI, ANGLE_MISO, ANGLE_CLK);
boolean diagnosticMode = false;
boolean errorState = false;
boolean switch_state = digitalRead(INT_EXT_SW);


void setup() {                
	//Initialize the LEDs as an output.
	pinMode(LED_NEAR, OUTPUT);     
	pinMode(LED_FAR, OUTPUT);     
	pinMode(LED_GOOD, OUTPUT);     
	pinMode(LED_POWER, OUTPUT);
	pinMode(INT_EXT_SW, INPUT);
	digitalWrite(LED_POWER, 1);
	delay(300);
	digitalWrite(LED_FAR, 1);
	delay(300);
	digitalWrite(LED_NEAR, 1);
	delay(300);
	digitalWrite(LED_GOOD, 1);
	delay(300);
	digitalWrite(LED_NEAR, 0);
	digitalWrite(LED_GOOD, 0);
	digitalWrite(LED_FAR, 0);
	
	
	switch_state = !digitalRead(INT_EXT_SW);
	diagnosticMode = !switch_state;
	if (!diagnosticMode) {
		digitalWrite(LED_POWER,0);
		angleSensor.pause();
	}
		
}

void loop() {
	
	 switch_state = !digitalRead(INT_EXT_SW);

	if ((diagnosticMode) && (switch_state == true)) {
		digitalWrite(LED_NEAR, 0);
		digitalWrite(LED_GOOD, 0);
		digitalWrite(LED_FAR, 0);
		diagnosticMode = false;
		digitalWrite(LED_POWER,0);
		angleSensor.pause();
	}
	
	if ((!diagnosticMode) && (switch_state == false)) {
		diagnosticMode = true;
		digitalWrite(LED_POWER,1);
		angleSensor.restart();
	}

	if (diagnosticMode) {

		byte gain = angleSensor.getGain();

		errorState = false;//angleSensor.error();


		word state = angleSensor.getState();

		if (state & 0x400 || errorState) {
			digitalWrite(LED_FAR, 1);
		}
		else {
			digitalWrite(LED_FAR, 0);
		}

		if (state & 0x800 || errorState) {
			digitalWrite(LED_NEAR, 1);
		}
		else {
			digitalWrite(LED_NEAR, 0);
		}

		if (gain < 255 || errorState) {
			digitalWrite(LED_GOOD, 1);
		}
		else {
			digitalWrite(LED_GOOD, 0);
		}


	}

	delay(100);

}


