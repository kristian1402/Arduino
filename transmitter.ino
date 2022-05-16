#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h> // Library containing math functions, used to calculate the angles from the magnetometer
#include <Wire.h> //Library for I2C communication, used in the magnetometer
#include <DFRobot_QMC5883.h> //Magnetometer library

DFRobot_QMC5883 compass; //Define the magnetometer as compass

#define motor 2 // define the motor port

RF24 radio(9, 10); // CE, CSN
const byte address[5] = {'R', 'X', 'A', 'A', 'A'};


void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  while (!compass.begin()) // If magnetometer is not connected propperly, print error message
  {
    Serial.println("Could not find a valid QMC5883 sensor, check wiring!"); 
    delay(500);
  }
  // Next part checks if our magnetometer is the HMC or QMC model, and initializes the correct one, by setting the range, measurement mode, and data and sample rate 
    if(compass.isHMC()){
        Serial.println("Initialize HMC5883");
        compass.setRange(HMC5883L_RANGE_1_3GA);
        compass.setMeasurementMode(HMC5883L_CONTINOUS);
        compass.setDataRate(HMC5883L_DATARATE_15HZ);
        compass.setSamples(HMC5883L_SAMPLES_8);
    }
   else if(compass.isQMC()){
        Serial.println("Initialize QMC5883");
        compass.setRange(QMC5883_RANGE_2GA);
        compass.setMeasurementMode(QMC5883_CONTINOUS); 
        compass.setDataRate(QMC5883_DATARATE_50HZ);
        compass.setSamples(QMC5883_SAMPLES_8);
   }

}

void loop() {
 Vector norm = compass.readNormalize(); // Normalize the readings from the magnetometer
  float heading = atan2(norm.YAxis, norm.XAxis); // Calculate the heading by taking the arc tangent of the 2 normalized x and y values from the magnetometer
  float declinationAngle = (3.0 + (47.0 / 60.0)) / (180 / PI); //Magnetic declenation angle of the current location, in our case set to Aalborg, Denmark
  // Next part calculates the heading in degrees, first in radians and then in degrees
  heading += declinationAngle;
  if (heading < 0){
    heading += 2 * PI;
  }
  if (heading > 2 * PI){
    heading -= 2 * PI;
  }
  float headingDegrees = heading * 180/M_PI; 
  Serial.println(headingDegrees);
 radio.write(&headingDegrees, sizeof(headingDegrees));
 delay(100);
}
