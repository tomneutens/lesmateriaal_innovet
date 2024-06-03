#include <Wire.h>
#include <Dwenguino.h>
#include <LiquidCrystal.h>
#include <DwenguinoMotor.h>

#define LINE_SENSORS 5

unsigned char sensorPins[LINE_SENSORS] = {A0, A1, A2, A3, A4};
unsigned int sensorValues[LINE_SENSORS] = {0, 0, 0, 0, 0};
float currentError = 0;

int itersBeforeGo = 2000;
int currentIters = 0;
long iters = 0;

DCMotor dcMotor1(MOTOR_1_0, MOTOR_1_1);
DCMotor dcMotor2(MOTOR_2_0, MOTOR_2_1);

float proportional = 0;
float Kp = 55;
float integral = 0;
float Ki = 0.0;
float derivative = 0;
float lastMeasuredError =  0;
float Kd = 75;

int baseMotorSpeed = 75;
float lastMotorSpeed = 0;

void setup()
{
  initDwenguino();
  // Set sensor pins as input and enable pullup.
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    pinMode(sensorPins[i], INPUT_PULLUP);
  }
  // Open Serial1 communication with the Bluetooth module at 9600 baud rate.
  Serial1.begin(9600);
  pinMode(SW_N, INPUT_PULLUP);
  while(digitalRead(SW_N)){
    Serial1.println("WAITING");
  }
  Serial1.println("START"); // Send start frame
}

void readSensorValues(unsigned int* sensorValues){
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    sensorValues[i] = analogRead(sensorPins[i]);
    // Invert
    sensorValues[i] = 1023 - sensorValues[i];
    //limit
    if (sensorValues[i] > 130){
      sensorValues[i] = 130;
    }
    if (sensorValues[i] <= 120){
      sensorValues[i] = 120; 
    }
    sensorValues[i] -= 120;
    
  }
}

void printSensorValues(unsigned int* sensorValues) {
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    dwenguinoLCD.setCursor(i,0);
    dwenguinoLCD.print(String(sensorValues[i]));
  }
}

void printLineError(int error) {
  dwenguinoLCD.setCursor(0,1);
  dwenguinoLCD.print(String(error));
}

float caclulateLineError(unsigned int* sensorValues){
  float error = 0;
  float cooif[5] = {-2.5, -1, 0.0, 1, 2.5};
  for (char i = -2 ; i <= 2 ; ++i){
    error += (float)cooif[i+2] * (float)sensorValues[i+2];
  }

  //normalize
  error = error/10.0;

  if (error < 0.1 && error > -0.1){
    return currentError;
  }
  currentError = error;
  //currentError = (float)error * 0.01 + (float)currentError * 0.99;
  return currentError;
}


void loop()
{
  // Read the sensor values
  readSensorValues(sensorValues);
  //printSensorValues(sensorValues);

  // Calculate P, I and D 
  float measuredError = caclulateLineError(sensorValues);
  proportional = (float)measuredError;
  integral = integral + (float)measuredError;
  derivative = measuredError - (float)lastMeasuredError;
  lastMeasuredError = measuredError;
  
  // calculate motor speeds
  float motorSpeed = Kp * proportional + Ki * integral + Kd * derivative;
  motorSpeed = 0.1 * motorSpeed + 0.9 * lastMotorSpeed;
  lastMotorSpeed = motorSpeed;
  int speedMotor1 = baseMotorSpeed + (int)motorSpeed;
  int speedMotor2 = baseMotorSpeed - (int)motorSpeed;
  // Only start after initial delay
  if (currentIters < itersBeforeGo){
    currentIters++;
  } else {
    dcMotor1.setSpeed(speedMotor1);
    dcMotor2.setSpeed(speedMotor2);
  }
  
  if (iters % 100 == 0){
    Serial1.println(String(motorSpeed) + ";" + String(Kp * proportional) + ";" + String(Ki * integral) + ";" + String(Kd * derivative) + ";" + String(sensorValues[2]) + ";" + String(baseMotorSpeed));
    //Serial1.println(String(currentError) + ";" + String(sensorValues[0]) + ";" + String(sensorValues[1]) + ";" + String(sensorValues[2]) + ";" + String(sensorValues[3]) + ";" + String(sensorValues[4]));
  }
    
  iters++;
}