#include <Servo.h>  //서보 라이브러리를 불러온다
Servo servo;

int trig = 4, echo = 5;    //트리거는 4번, 에코는 5번
int servoPin = 3, ang = 90, i = 1;  // rad는 각도를 의미합니다.
long distance, duration;
 
void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  servo.attach(servoPin); 
  servo.write(ang);
}
 
void loop() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);  //2uS 동안 멈춤
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);  //10uS 동안 동작
  digitalWrite(trig, LOW);
 
  duration = pulseIn(echo, HIGH); 
  distance = (duration * 0.034) / 2; // 음의 속도:초속 340m

  ang += i;
  if (ang > 180) {
    ang = 179;
    i = -1;
  }
  else if (ang < 0) {
    ang = 1;
    i = 1;
  }
  servo.write(ang);
   
  // 시리얼모니터에 출력(processing에서 데이터 처리를 하기 위해 구분자 사용)
  Serial.print(ang);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
  delay(70); //서보모터가 움직이는 시간을 줍니다.
}
