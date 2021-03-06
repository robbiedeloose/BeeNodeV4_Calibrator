#include <Arduino.h>

/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.

 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch

 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).

 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND

 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"

#define SCALE_1_CLOCK 7   // wit-blauw
#define SCALE_1_DATA  8   // blauw
#define SCALE_2_CLOCK 9   // wit-groen
#define SCALE_2_DATA  10  // groen
#define SCALE_3_CLOCK 11  // wit-bruin
#define SCALE_3_DATA  12  // bruin

#define SCALE_4_CLOCK 5   // wit-blauw
#define SCALE_4_DATA  6   // blauw
#define SCALE_5_CLOCK 14  // wit-groen
#define SCALE_5_DATA  15  // groen
#define SCALE_6_CLOCK 16  // wit-bruin
#define SCALE_6_DATA  17  // bruin

#define DOUT  8
#define CLK   7

#define Serial SerialUSB

HX711 scale(DOUT, CLK);

float calibration_factor = 24480.00; //-7050 worked for my 440lb max scale setup
float zero_factor = 8732445.00;

void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale();
  //scale.tare(); //Reset the scale to 0
  scale.set_offset(zero_factor); //old 8749213

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void findZeroPoint(){
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void findCalibrationFactor(){

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  Serial.print(scale.get_units(5), 3);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }

}

void testCalibration(){
  
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading: ");
  Serial.print(scale.get_units(10), 3);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  scale.power_down();
  delay(10000);
  scale.power_up();
  delay(100);
}

void loop() {

// uncomment one function below

//findZeroPoint();
//findCalibrationFactor();
testCalibration();

}
