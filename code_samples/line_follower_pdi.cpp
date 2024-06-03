#include <Wire.h>
#include <Dwenguino.h>
#include <LiquidCrystal.h>
#include <DwenguinoMotor.h>

#define LINE_SENSORS 6

unsigned char sensorPins[LINE_SENSORS] = {0, 1, 2, 3, 4, 5};
unsigned char sensorValues[LINE_SENSORS] = {0, 0, 0, 0, 0, 0};
char currentError = 0;
int error = 0;

DCMotor dcMotor1(MOTOR_1_0, MOTOR_1_1);
DCMotor dcMotor2(MOTOR_2_0, MOTOR_2_1);

float proportional = 0;
float Kp = 15;
float integral = 0;
float Ki = 0.125;
float derivative = 0;
int lastMeasuredError =  0;
float Kd = 20;

int baseMotorSpeed = 100;

void setup()
{
  initDwenguino();
  // Set sensor pins as input and enable pullup.
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    pinMode(i, INPUT_PULLUP);
  }
  // Open Serial1 communication with the Bluetooth module at 9600 baud rate.
  Serial1.begin(9600);
  Serial1.println("START"); // Send start frame
}

void readSensorValues(unsigned char* sensorValues){
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    sensorValues[i] = digitalRead(i);
  }
}

void printSensorValues(unsigned char* sensorValues) {
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    dwenguinoLCD.setCursor(i,0);
    dwenguinoLCD.print(String(sensorValues[i]));
  }
}

void printLineError(int error) {
  dwenguinoLCD.setCursor(0,1);
  dwenguinoLCD.print(String(error));
}

char caclulateLineError(unsigned char* sensorValues){
  char error = 0;
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    if (sensorValues[i] && i <= 2 && !error){
      error = -3 + i;
    }
    else if (sensorValues[i] && i > 2){
      error = i - 2;
    }
  }
  if (error == 0){
    error = currentError;
  }
  currentError = error;
  return error;
}


void loop()
{
  // Read the sensor values
  readSensorValues(sensorValues);
  //printSensorValues(sensorValues);

  // Calculate P, I and D 
  int measuredError = caclulateLineError(sensorValues);
  proportional = (float)measuredError;
  integral = integral + (float)measuredError;
  derivative = measuredError - (float)lastMeasuredError;
  lastMeasuredError = measuredError;
  
  // calculate motor speeds
  float motorSpeed = Kp * proportional + Ki * integral + Kd * derivative;
  int speedMotor1 = baseMotorSpeed + motorSpeed;
  int speedMotor2 = baseMotorSpeed - motorSpeed;
  dcMotor1.setSpeed(speedMotor1);
  dcMotor2.setSpeed(speedMotor2);

  // Print debug info 
  dwenguinoLCD.setCursor(0,0);
  dwenguinoLCD.print(String(proportional));
  dwenguinoLCD.setCursor(4,0);
  dwenguinoLCD.print(String(integral));
  dwenguinoLCD.setCursor(8,0);
  dwenguinoLCD.print(String(derivative));
  dwenguinoLCD.setCursor(12,0);
  dwenguinoLCD.print(String(motorSpeed));


  // Print motor speeds
  dwenguinoLCD.setCursor(4,1);
  dwenguinoLCD.print(String(speedMotor1));
  dwenguinoLCD.setCursor(8,1);
  dwenguinoLCD.print(String(speedMotor2));
  
  // Send data over serial
  // measuredError;proportional;integral;derivative
  Serial1.println(measuredError + ";" + proportional + ";" + integral + ";" + derivative);
  
  delay(1);
  dwenguinoLCD.clear();
}