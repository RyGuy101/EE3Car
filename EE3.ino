#define LEFT_MOTOR 5
#define RIGHT_MOTOR 6

#define LEFT_SENSOR A7
#define RIGHT_SENSOR A5

#define LEFT_LED 2
#define STRAIGHT_LED 4
#define RIGHT_LED 7

#define LEFT_WHEEL A6
#define RIGHT_WHEEL A4

#define LEFT_FAST 100 //200
#define RIGHT_FAST 130 //245
#define MEAS_FAST 0.00002f

#define p 1.0f
#define accelTimeConst 500000.0f // microsecond-scale

float leftMaxLight;
float rightMaxLight;

float leftSpeed = 0;
float rightSpeed = 0;

bool leftWheelPrevState;
bool rightWheelPrevState;
unsigned long leftWheelPrevTime;
unsigned long rightWheelPrevTime;
float leftMeasSpeed = 0;
float rightMeasSpeed = 0;
float prevDesiredLeftSpeed = 0;
float prevDesiredRightSpeed = 0;


unsigned long currTime;
unsigned long loopTime;

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(STRAIGHT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  calibrate();
  leftWheelPrevState = analogRead(LEFT_WHEEL) > 100;
  rightWheelPrevState = analogRead(RIGHT_WHEEL) > 100;
  leftWheelPrevTime = micros();
  rightWheelPrevTime = micros();
  currTime = micros();
}

void loop() {
  run();
//  delay(10);

//  TEST GO STRAIGHT
//  analogWrite(LEFT_MOTOR, LEFT_FAST);
//  analogWrite(RIGHT_MOTOR, RIGHT_FAST);

//  TEST IR SENSORS 
//  delay(10);
//  float left = analogRead(LEFT_SENSOR) / leftMaxLight;
//  float right = analogRead(RIGHT_SENSOR) / rightMaxLight;
//  Serial.print("left=");
//  Serial.print(left);
//  Serial.print(" right=");
//  Serial.println(right);

//  TEST WHEEL SENSORS
//  unsigned long currTime = micros();
//  int leftWheel = analogRead(LEFT_WHEEL);
//  if (leftWheelPrevState == LOW && leftWheel > 150) {
//    leftMeasSpeed = 1.0 / (currTime - leftWheelPrevTime);
//    leftWheelPrevState = HIGH;
//    Serial.println(leftMeasSpeed, 8);
//    leftWheelPrevTime = currTime;
//  } else if (leftWheel < 50) {
//    leftWheelPrevState = LOW;
//  }
//  int rightWheel = analogRead(RIGHT_WHEEL);
//  if (rightWheelPrevState == LOW && rightWheel > 150) {
//    rightMeasSpeed = 1.0 / (currTime - rightWheelPrevTime);
//    rightWheelPrevState = HIGH;
////    Serial.println(rightMeasSpeed, 8);
//    rightWheelPrevTime = currTime;
//  } else if (rightWheel < 50) {
//    rightWheelPrevState = LOW;
//  }
}

void run() {
  unsigned long newTime = micros();
  loopTime = newTime - currTime;
  currTime = newTime;
  
  int leftWheel = analogRead(LEFT_WHEEL);
  if (leftWheelPrevState == LOW && leftWheel > 150) {
    leftMeasSpeed = 1.0 / (currTime - leftWheelPrevTime);
    leftWheelPrevState = HIGH;
    leftWheelPrevTime = currTime;
  } else if (leftWheel < 50) {
    leftWheelPrevState = LOW;
  }
  if (currTime - leftWheelPrevTime > 200000) {
    leftMeasSpeed = 0;
  }

  int rightWheel = analogRead(RIGHT_WHEEL);
  if (rightWheelPrevState == LOW && rightWheel > 150) {
    rightMeasSpeed = 1.0 / (currTime - rightWheelPrevTime);
//    Serial.println(rightMeasSpeed, 8);
    rightWheelPrevState = HIGH;
    rightWheelPrevTime = currTime;
  } else if (rightWheel < 50) {
    rightWheelPrevState = LOW;
  }
  if (currTime - rightWheelPrevTime > 200000) {
    rightMeasSpeed = 0;
  }
  
  float left = analogRead(LEFT_SENSOR) / leftMaxLight;
  float right = analogRead(RIGHT_SENSOR) / rightMaxLight;
  if (left < 0.5 && right < 0.5) {
    analogWrite(LEFT_MOTOR, 0);
    analogWrite(RIGHT_MOTOR, 0);
//    delay(1000);
  } else {
    float leftPercent = constrain(p*left + (1-p), 0, 1);
    float rightPercent = constrain(p*right + (1-p), 0, 1);

    float desiredLeftSpeed = MEAS_FAST * leftPercent;
    float desiredRightSpeed = MEAS_FAST * rightPercent;

    float leftSpeedChange = (desiredLeftSpeed - leftMeasSpeed) / MEAS_FAST;
    float rightSpeedChange = (desiredRightSpeed - rightMeasSpeed) / MEAS_FAST;
    leftSpeedChange *= abs(leftSpeedChange);
    rightSpeedChange *= abs(rightSpeedChange);
    leftSpeedChange *= LEFT_FAST * loopTime/accelTimeConst;
    rightSpeedChange *= RIGHT_FAST * loopTime/accelTimeConst;
    leftSpeedChange += (desiredLeftSpeed - prevDesiredLeftSpeed) * LEFT_FAST/MEAS_FAST / 2;
    rightSpeedChange += (desiredRightSpeed - prevDesiredRightSpeed) * LEFT_FAST/MEAS_FAST / 2;
    
    leftSpeed = constrain(leftSpeed + leftSpeedChange, 0, 255);
    rightSpeed = constrain(rightSpeed + rightSpeedChange, 0, 255);

    prevDesiredLeftSpeed = desiredLeftSpeed;
    prevDesiredRightSpeed = desiredRightSpeed;
    
    analogWrite(LEFT_MOTOR, (int) leftSpeed);
    analogWrite(RIGHT_MOTOR, (int) rightSpeed);

    if (leftPercent / rightPercent < 0.9) {
      digitalWrite(LEFT_LED, HIGH);
      digitalWrite(STRAIGHT_LED, LOW);
      digitalWrite(RIGHT_LED, LOW);
    } else if (rightPercent / leftPercent < 0.9) {
      digitalWrite(LEFT_LED, LOW);
      digitalWrite(STRAIGHT_LED, LOW);
      digitalWrite(RIGHT_LED, HIGH);
    } else {
      digitalWrite(LEFT_LED, LOW);
      digitalWrite(STRAIGHT_LED, HIGH);
      digitalWrite(RIGHT_LED, LOW);
    }
  }
}

void calibrate() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  float avgLeft = 0;
  float avgRight = 0;
  for (int i = 0; i < 100; i++) {
    avgLeft += analogRead(LEFT_SENSOR);
    avgRight += analogRead(RIGHT_SENSOR);
    delay(10);
  }
  avgLeft /= 100;
  avgRight /= 100;
  leftMaxLight = avgLeft; 
  rightMaxLight = avgRight; 
}

