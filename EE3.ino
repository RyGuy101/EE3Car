#define LEFT_MOTOR 5
#define RIGHT_MOTOR 6

#define LEFT_SENSOR A7
#define RIGHT_SENSOR A5

#define LEFT_LED 2
#define STRAIGHT_LED 4
#define RIGHT_LED 7

#define LEFT_WHEEL A6
#define RIGHT_WHEEL A4

#define LEFT_FAST 110 //200 //100
#define RIGHT_FAST 140 //245 //130

float leftMaxLight;
float rightMaxLight;

bool leftWheelPrevState;
bool rightWheelPrevState;
unsigned long leftWheelPrevTime;
unsigned long rightWheelPrevTime;
float leftMeasSpeed = 0;
float rightMeasSpeed = 0;

unsigned long currTime;
unsigned long loopTime;

unsigned long flashTime = 0;

bool stopped = false;

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(STRAIGHT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  calibrate();
  leftWheelPrevState = analogRead(LEFT_WHEEL) > 150;
  rightWheelPrevState = analogRead(RIGHT_WHEEL) > 150;
  leftWheelPrevTime = micros();
  rightWheelPrevTime = micros();
  currTime = micros();
  analogWrite(RIGHT_MOTOR, RIGHT_FAST*1.2);
  analogWrite(LEFT_MOTOR, LEFT_FAST*1.2);
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(STRAIGHT_LED, HIGH);
  digitalWrite(RIGHT_LED, LOW);
  delay(250);
}

void loop() {
  run();

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
//  Serial.println(leftWheel);
//  if (leftWheelPrevState == LOW && leftWheel > 150) {
//    leftMeasSpeed = 1.0 / (currTime - leftWheelPrevTime);
//    leftWheelPrevState = HIGH;
//    Serial.println(leftMeasSpeed, 8);
//    leftWheelPrevTime = currTime;
//  } else if (leftWheel < 50) {
//    leftWheelPrevState = LOW;
//  }
//  int rightWheel = analogRead(RIGHT_WHEEL);
////  Serial.println(rightWheel);
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
  if (currTime - leftWheelPrevTime > 500000) {
    leftMeasSpeed = 0;
  }

  int rightWheel = analogRead(RIGHT_WHEEL);
  if (rightWheelPrevState == LOW && rightWheel > 150) {
    rightMeasSpeed = 1.0 / (currTime - rightWheelPrevTime);
    rightWheelPrevState = HIGH;
    rightWheelPrevTime = currTime;
  } else if (rightWheel < 50) {
    rightWheelPrevState = LOW;
  }
  if (currTime - rightWheelPrevTime > 500000) {
    rightMeasSpeed = 0;
  }
  
  float left = analogRead(LEFT_SENSOR) / leftMaxLight;
  float right = analogRead(RIGHT_SENSOR) / rightMaxLight;
  if ((left < 0.6 && right < 0.6) || stopped) {
    stopped = true;
    analogWrite(LEFT_MOTOR, 0);
    analogWrite(RIGHT_MOTOR, 0);
    digitalWrite(LEFT_LED, HIGH);
    digitalWrite(STRAIGHT_LED, LOW);
    digitalWrite(RIGHT_LED, HIGH);
  } else {
    int leftSpeed = LEFT_FAST;
    int rightSpeed = RIGHT_FAST;
    if (left < 0.92) {
      leftSpeed = 0;
    } else if (right < 0.92) {
      rightSpeed = 0;
    }

    analogWrite(LEFT_MOTOR, leftSpeed);
    analogWrite(RIGHT_MOTOR, rightSpeed);

    if (rightSpeed > 0 && rightMeasSpeed == 0) {
      analogWrite(RIGHT_MOTOR, rightSpeed*1.5);
      analogWrite(LEFT_MOTOR, leftSpeed*1.5);
      if (currTime - flashTime > 100000) {
        if (currTime - flashTime < 200000) {
          digitalWrite(LEFT_LED, HIGH);
          digitalWrite(STRAIGHT_LED, HIGH);
          digitalWrite(RIGHT_LED, HIGH);
        } else {
          flashTime = currTime;
        }
      } else {
        digitalWrite(LEFT_LED, LOW);
        digitalWrite(STRAIGHT_LED, LOW);
        digitalWrite(RIGHT_LED, LOW);
      }
    } else if (leftSpeed == 0) {
      digitalWrite(LEFT_LED, HIGH);
      digitalWrite(STRAIGHT_LED, LOW);
      digitalWrite(RIGHT_LED, LOW);
    } else if (rightSpeed == 0) {
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

