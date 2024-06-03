#include <Wire.h>
#include <Dwenguino.h>
#include <LiquidCrystal.h>
#include <DwenguinoMotor.h>

#define LINE_SENSORS 6

unsigned char sensorPins[LINE_SENSORS] = {A0, A1, A2, A3, A4, A5};
float sensorValues[LINE_SENSORS] = {0, 0, 0, 0, 0, 0};
float sensorCalibrationTop[LINE_SENSORS] = {900, 300, 300, 450, 825, 617};
float sensorCalibrationBottom[LINE_SENSORS] = {250, 50, 50, 100, 150, 58};
float currentError = 0;

int itersBeforeGo = 2000;
int currentIters = 0;
long iters = 0;

DCMotor dcMotor1(MOTOR_1_0, MOTOR_1_1);
DCMotor dcMotor2(MOTOR_2_0, MOTOR_2_1);

float proportional = 0;
float Kp = 75;
float integral = 0;
float Ki = 10.0;
float derivative = 0;
float lastMeasuredError =  0;
float Kd = 300;

int baseMotorSpeed = 65;
float lastMotorSpeed = 0;

void printStringsToLCD(String line1, String line2){
  dwenguinoLCD.clear();
  dwenguinoLCD.setCursor(0,0);
  dwenguinoLCD.print(line1);
  dwenguinoLCD.setCursor(0,1);
  dwenguinoLCD.print(line2);
}

void calibrateSetpoint(float* calibrationSetPoints){
  // Read initial value
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    calibrationSetPoints[i] = (float)analogRead(sensorPins[i]);
  }
  // Read multiple values and take exponential running average.
  while (digitalRead(SW_E) == HIGH){
    for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
      calibrationSetPoints[i] = (float)analogRead(sensorPins[i]) * 0.95 + calibrationSetPoints[i] * 0.05;
    }
  }
  delay(500); // Debounce
}

void calibrate(){
  printStringsToLCD("Move to black", "East -> continue");
  while (digitalRead(SW_E) == HIGH){
    // Wait until east is pressed
  }
  delay(500); // Debounce

  printStringsToLCD("Calibrating black", "East -> continue");
  // Calibrate white value
  calibrateSetpoint(sensorCalibrationTop);

  printStringsToLCD("Move to white", "East -> continue");
  while (digitalRead(SW_E) == HIGH){
    // Wait for E button press.
  }
  delay(500); // Debounce

  printStringsToLCD("Calibrating white", "East -> continue");
  // Calibrate black value
  calibrateSetpoint(sensorCalibrationBottom);

  // Send calibration telemetry data.
  Serial1.println("CALIBRATION VALS");
  Serial1.println("TOP: " + String(sensorCalibrationTop[0]) + ";"  + String(sensorCalibrationTop[1]) + ";"  + String(sensorCalibrationTop[2]) + ";"  + String(sensorCalibrationTop[3]) + ";"  + String(sensorCalibrationTop[4]) + ";"  + String(sensorCalibrationTop[5]));
  Serial1.println("BOTTOM: " + String(sensorCalibrationBottom[0]) + ";"  + String(sensorCalibrationBottom[1]) + ";"  + String(sensorCalibrationBottom[2]) + ";"  + String(sensorCalibrationBottom[3]) + ";"  + String(sensorCalibrationBottom[4]) + ";"  + String(sensorCalibrationBottom[5]));
}

void setup()
{
  initDwenguino();
  // Set sensor pins as input and enable pullup.
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    pinMode(sensorPins[i], INPUT_PULLUP);
  }
  // Open Serial1 communication with the Bluetooth module at 9600 baud rate.
  Serial1.begin(9600);
  pinMode(SW_N, INPUT_PULLUP); // Set N as input and enable pull-up resistor.
  // Wait until N is pressed.
  while(digitalRead(SW_N)){
    Serial1.println("WAITING"); // Send update over BT.
    delay(250);
  }

  // Notify computer of calibration process.
  Serial1.println("CALIBRATING"); 

  // Start calibration
  calibrate();
  printStringsToLCD("Calibration done", "East -> START");
  while (digitalRead(SW_E) == HIGH){
    // Wait until east is pressed
  }
  delay(500); // Debounce
  Serial1.println("START"); // Send start frame
}

void readSensorValues(){
  for (unsigned char i = 0 ; i < LINE_SENSORS ; ++i){
    sensorValues[i] = (float)analogRead(sensorPins[i]);
    // Invert
    //limit
    if (sensorValues[i] > sensorCalibrationTop[i]){
      sensorValues[i] = sensorCalibrationTop[i];
    }
    if (sensorValues[i] < sensorCalibrationBottom[i]){
      sensorValues[i] = sensorCalibrationBottom[i]; 
    }
    // Normalize
    sensorValues[i] -= sensorCalibrationBottom[i];
    sensorValues[i] = (float)sensorValues[i]/(float)(sensorCalibrationTop[i]-sensorCalibrationBottom[i]);
    // Invert
    sensorValues[i] = 1.0 - sensorValues[i];
  }
}


float caclulateLineError(){
  float error = 0;
  float cooif[LINE_SENSORS] = {-3, -2, -1, 1, 2, 3};
  for (char i = 0 ; i <= LINE_SENSORS ; ++i){
    error += (float)cooif[i] * (float)sensorValues[i];
  }

  //normalize
  error = error/3.0;

  currentError = error;
  //currentError = (float)error * 0.01 + (float)currentError * 0.99;
  return currentError;
}




void loop()
{
  // Read the sensor values
  readSensorValues();

  // Calculate P, I and D 
  float measuredError = caclulateLineError();
  proportional = (float)measuredError;
  integral = integral + (float)measuredError;
  derivative = measuredError - (float)lastMeasuredError;
  lastMeasuredError = measuredError;
  
  // calculate motor speeds
  float motorSpeed = Kp * proportional + Ki * integral + Kd * derivative;
  motorSpeed = 0.1 * motorSpeed + 0.9 * lastMotorSpeed; // Smooth transition
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
    Serial1.println(String(motorSpeed) + ";" + String(Kp * proportional) + ";" + String(Ki * integral) + ";" + String(Kd * derivative) + ";" + String(measuredError) + ";" + String(baseMotorSpeed));
    //Serial1.println(String(sensorValues[0]) + ";" + String(sensorValues[1]) + ";" + String(sensorValues[2]) + ";" + String(sensorValues[3]) + ";" + String(sensorValues[4]) + ";" + String(sensorValues[5]));
  }
    
  iters++;
}