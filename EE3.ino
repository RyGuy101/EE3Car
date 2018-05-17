#define LEFT_MOTOR 5
#define RIGHT_MOTOR 6

#define LEFT_SENSOR A7
#define RIGHT_SENSOR A5

#define LEFT_LED 2
#define STRAIGHT_LED 3
#define RIGHT_LED 4

#define LEFT_WHEEL
#define RIGHT_WHEEL

#define LEFT_FAST 100
#define RIGHT_FAST 130

#define p 0.8f

float leftMaxLight;
float rightMaxLight;

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);
  calibrate();
}

void loop() {
  float left = analogRead(LEFT_SENSOR) / leftMaxLight;
  float right = analogRead(RIGHT_SENSOR) / rightMaxLight;
  if (left < 0.5 && right < 0.5) {
    analogWrite(LEFT_MOTOR, 0);
    analogWrite(RIGHT_MOTOR, 0);
  } else {
    int leftSpeed = LEFT_FAST * (p*left + (1-p));
    int rightSpeed = RIGHT_FAST * (p*right + (1-p));
    
    analogWrite(LEFT_MOTOR, leftSpeed);
    analogWrite(RIGHT_MOTOR, rightSpeed);
  }
  
//  delay(10);
//  Serial.print("left=");
//  Serial.print(left);
//  Serial.print(" right=");
//  Serial.println(right);

//  int leftwheel = analogRead(LEFT_WHEEL);
//  int rightwheel = analogRead(RIGHT_WHEEL);
//  delay(100);
//  Serial.print("left wheel=");
//  Serial.print(leftwheel);
//  Serial.print(" right wheel=");
//  Serial.println(rightwheel);
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

