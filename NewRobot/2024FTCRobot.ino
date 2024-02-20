#define CH1 8
#define CH2 9
#define CH4 12
#define CH7 A4
#define CH8 A3  
#define CH9 A2
#include <Servo.h>

Servo pivot;

int pivotPin = A5;


int ch1Value;
int ch2Value;
int ch4Value;
int ch7Value;
int ch8Value;
int ch9Value;

int leftForward = 2; 
int leftBack = 3; 
int rightForward = 4; 
int rightBack = 5; 
int wristIn = 7; 
int wristOut = 11; 
int extendOut = 13;
int extendIn = 6;


int desiredAngle = 45; // Set the desired angle
int currentAngle = 45; // Initialize current angle
int step = 10; // Set the step size for movement

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the switch channel and return a boolean value
int readSwitchTriple(byte channelInput, int defaultValue){
  int ch = readChannel(channelInput, 0, 100, defaultValue);
  
  if (ch < 30) {
    return 0;
  } else if (ch > 70) {
    return 2;
  } else {
    return 1;
  }
}

bool readSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void driveForward() {
  analogWrite(leftForward, 255);
  analogWrite(rightForward, 255);
  analogWrite(leftBack, 0);
  analogWrite(rightBack, 0);
}

void driveBack() {
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(leftBack, 255);
  analogWrite(rightBack, 255);
}

void turnLeft() {
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 255);
  analogWrite(leftBack, 255);
  analogWrite(rightBack, 0);
}

void turnRight() {
  analogWrite(leftForward, 255);
  analogWrite(rightForward, 0);
  analogWrite(leftBack, 0);
  analogWrite(rightBack, 255);
}

void stop() {
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(leftBack, 0);
  analogWrite(rightBack, 0);
}

void moveWristIn() {
  analogWrite(wristIn, 200);
  analogWrite(wristOut, 0);
}

void moveWristOut() {
  analogWrite(wristIn, 0);
  analogWrite(wristOut, 200);
}

void wristStop() {
  analogWrite(wristIn, 0);
  analogWrite(wristOut, 0);
}

void moveExtendIn() {
  analogWrite(extendIn, 255);
  analogWrite(extendOut, 0);
  Serial.println("Extend In");
}

void moveExtendOut() {
  analogWrite(extendIn, 0);
  analogWrite(extendOut, 255);
  Serial.println("Extend Out");
}

void extendStop() {
  analogWrite(extendIn, 0);
  analogWrite(extendOut, 0);
  Serial.println("Extend Stop");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH7, INPUT);
  pinMode(CH8, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH9, INPUT);

  pivot.attach(pivotPin);

  pinMode(leftForward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(leftBack, OUTPUT);
  pinMode(rightBack, OUTPUT);
  pinMode(extendIn, OUTPUT);
  pinMode(extendOut, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ch1Value = readChannel(CH1, -255, 255, 0);
  ch2Value = readChannel(CH2, -255, 255, 0);
  ch4Value = readChannel(CH4, -255, 255, 0);
  ch9Value = readChannel(CH9, 0, 200, 100);
  ch7Value = readSwitchTriple(CH7, 1);
  ch8Value = readSwitch(CH8, false);

  Serial.println(ch9Value);

  if (ch1Value < -35) {
    turnRight();
  } else if (ch1Value > 35) {
    turnLeft();
  } else if (ch2Value > 35) {
    driveBack();
  } else if (ch2Value < -35) {
    driveForward();
  } else {
    stop();
  }

  /*if (ch8Value == 0) {
    moveExtendIn();
  } else {
    moveExtendOut();
  }*/

  if (ch9Value > 120) {
    moveExtendIn();
  } else if (ch9Value > 20 && ch9Value < 120) {
    moveExtendOut();
  } else {
    extendStop();
  }


  if (ch7Value == 0) {
    desiredAngle = 45;
    if (currentAngle != desiredAngle) {
      if (currentAngle < desiredAngle) {
        currentAngle += step; // Increment angle
      } else if (currentAngle > desiredAngle){
        currentAngle -= step; // Decrement angle
      } 
      pivot.write(currentAngle); // Set the servo position
      delay(7); // Adjust delay to change speed (smaller values make it faster)
    }
  } else if (ch7Value == 2) {
    desiredAngle = 125;
    if (currentAngle != desiredAngle) {
      if (currentAngle < desiredAngle) {
        currentAngle += step; // Increment angle
      } else {
        currentAngle -= step; // Decrement angle
      }
      pivot.write(currentAngle); // Set the servo position
      delay(7); // Adjust delay to change speed (smaller values make it faster)
    }
    moveWristOut();
  } else {
    desiredAngle = 55;
    if (currentAngle != desiredAngle) {
      if (currentAngle < desiredAngle) {
        currentAngle += step; // Increment angle
      } else {
        currentAngle -= step; // Decrement angle
      }
      pivot.write(currentAngle); // Set the servo position
      (7); // Adjust delay to change speed (smaller values make it faster)
    }
  }

  if (ch7Value != 2) {
     if (ch4Value <= -100) {
        moveWristIn();
      } else if (ch4Value >= 100) {
        moveWristOut();
      } else {
        wristStop();
      }
  }


}
