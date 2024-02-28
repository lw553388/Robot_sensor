#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Servo.h>
#include <Wire.h>

Servo servo;

int trig = 4, echo = 5;             //초음파센서 핀 설정
int servoPin = 3, ang = 90, i = 1;  //서보 핀, 각도, 증가량 설정
long distance, duration;            //거리 및 펄스 지속 시간 변수 선언

#define SCREEN_WIDTH 128
#define SCREEN_HIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 oled(128, 64, &Wire, -1);  // OLED 초기화

struct Obstacle{
  int x, y;
  bool detected;
};
 
void setup() {
  //시리얼 통신, 핀모드, 서보모터 핀, 초기 각도 설정
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  servo.attach(servoPin); 
  servo.write(ang);

  //OLED 초기화
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  oled.clearDisplay();
  oled.display();
}
 
void loop() {
  oled.clearDisplay();
  drawRadar();              // 레이더 그리는 함수
  oled.setTextSize(1);      // 텍스트 크기
  oled.setTextColor(SSD1306_WHITE); // 텍스트 색상
  oled.setCursor(0, 0);     // 각도 출력 텍스트 위치
  oled.print("Ang: ");
  oled.print(ang);
  oled.print("   Dis: ");
  oled.print(distance);
  oled.display();           // oled에 출력

  // 초음파 발생
  digitalWrite(trig, LOW);
  delayMicroseconds(2);  //2uS 동안 멈춤
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);  //10uS 동안 동작
  digitalWrite(trig, LOW);
  
  // 거리 계산
  duration = pulseIn(echo, HIGH);     // 시간 측정
  distance = (duration * 0.034) / 2;  // 음속:초속 340m

  // 서보모터의 움직임을 위한 각도 설정 
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

// 레이더 그리기 함수
void  drawRadar(){
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  // 레이더 원 그리기
  for(int i = 1; i <= 3; ++i)
    oled.drawCircle(SCREEN_WIDTH / 2, SCREEN_HIGHT, i*(SCREEN_HIGHT - 10) / 4, SSD1306_WHITE);

  // 레이더 각도 라인 그리기
  for(int i=30; i<=150; i=i+30){
    float angleRad = radians(i);
    int x1 = SCREEN_WIDTH/2+(3*(SCREEN_HIGHT-10)/4)*cos(angleRad);
    int y1 = SCREEN_HIGHT-(3*(SCREEN_HIGHT-10)/4)*sin(angleRad);
    int x2 = SCREEN_WIDTH/2+(SCREEN_HIGHT-10)/4*cos(angleRad);
    int y2 = SCREEN_HIGHT-(SCREEN_HIGHT-10)/4*sin(angleRad);
    oled.drawLine(x1, y1, x2, y2, SSD1306_WHITE);        
  }

  // 현재 각도에 대한 라인 그리기
  float angleRad = radians(ang);
  int lineLength = SCREEN_HIGHT-10;
  int x2 = SCREEN_WIDTH/2+lineLength*cos(angleRad);
  int y2 = SCREEN_HIGHT-lineLength*sin(angleRad);
  oled.drawLine(SCREEN_WIDTH/2, SCREEN_HIGHT, x2, y2, SSD1306_WHITE);

  // 외부 원의 반지름 설정
  int outerCircleRadius = 3*(SCREEN_HIGHT-10)/4;
  int obstacleDistance = distance;
  if(obstacleDistance < outerCircleRadius){
    drawSmallCircleInArc(ang, obstacleDistance);
  }
}

// 호 내의 작은 원 그리기 함수
void drawSmallCircleInArc(int angle, int dist){
  float radianAngle = radians(angle);

  int centerX = SCREEN_WIDTH/2+dist*cos(radianAngle);
  int centerY = SCREEN_HIGHT-dist*sin(radianAngle);

  oled.fillCircle(centerX, centerY, 3, SSD1306_WHITE);
}
