#define LEFT_MOTOR 5
#define RIGHT_MOTOR 6

#define LEFT_SENSOR A7
#define RIGHT_SENSOR A5

#define MOTOR_SPEED 100

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);
}

void loop() {
  int left = analogRead(LEFT_SENSOR);
  int right = analogRead(RIGHT_SENSOR);
  if (left < 200 && right < 200) {
    analogWrite(LEFT_MOTOR, 0);
    analogWrite(RIGHT_MOTOR, 0);
  } else {
    if (left < 200) {
      analogWrite(LEFT_MOTOR, 0);
    } else {
      analogWrite(LEFT_MOTOR, 191);
    }
    if (right < 200) {
      analogWrite(RIGHT_MOTOR, 0);
    } else {
      analogWrite(RIGHT_MOTOR, 191);
    }
  }
  delay(100);
  Serial.print("left=");
  Serial.print(left);
  Serial.print(" right=");
  Serial.println(right);
}
