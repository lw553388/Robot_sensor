import processing.serial.*;

Serial myPort;
String myString = null;
int angle, distance;

void setup() {
  myPort = new Serial(this, "COM7", 9600);
  size(1200, 700);
  background(0, 0, 0);
}

void draw() {
  // 먼저 그려진 레이더를 흐리게 표현
  noStroke();
  fill(0, 10);
  rect(0, 0, width, height);

  radarBoard();
  drawLine();
  drawObject();
}

// 초음파센서에 값에 따라 빨간선으로 장애물 표현
void drawObject() {
  pushMatrix();

  translate(width/2, height);
  strokeWeight(4);
  stroke(255, 10, 10);  // red color
  float d = (width/2.0/30.0) * (float)distance;
  if (d < width/2) {
    line(d*cos(radians(angle)), -d*sin(radians(angle)), width/2*cos(radians(angle)), 
    -width/2*sin(radians(angle)));
  }
  popMatrix();
}

// 초음파센서가 서보모터에 움직임에 따라 회전하는 모습 표현
void drawLine() {
  pushMatrix();

  translate(width/2, height);
  strokeWeight(4);
  stroke(98, 245, 31);

  line(0, 0, width/2 * cos(radians(angle)), -width/2 * sin(radians(angle)));

  popMatrix();
}

// 레이더 창 디자인
void radarBoard() {
  pushMatrix();    // 기준점이 다른 draw와 겹치지 않도록 해주기 위한 함수
  translate(width/2, height);
  noFill();
  stroke(98, 245, 31);
  strokeWeight(2);
  arc(0, 0, width, width, PI, TWO_PI);
  arc(0, 0, width*2/3, width*2/3, PI, TWO_PI);
  arc(0, 0, width*1/3, width*1/3, PI, TWO_PI);
  line(0, 0, width/2 * cos(radians(30)), -width/2 * sin(radians(30)));
  line(0, 0, width/2 * cos(radians(60)), -width/2 * sin(radians(60)));
  line(0, 0, width/2 * cos(radians(90)), -width/2 * sin(radians(90)));
  line(0, 0, width/2 * cos(radians(120)), -width/2 * sin(radians(120)));
  line(0, 0, width/2 * cos(radians(150)), -width/2 * sin(radians(150)));

  textSize(16);
  fill(98, 245, 31);
  textAlign(RIGHT);  
  text("10Cm", width*1/6, 0);
  text("20Cm", width*2/6, 0);
  text("30Cm", width*3/6, 0);
  textAlign(LEFT);  
  text("10Cm", -width*1/6, 0);
  text("20Cm", -width*2/6, 0);
  text("30Cm", -width*3/6, 0); 
  popMatrix();
}

// 시리얼포트로 들어오는 값을 처리
void serialEvent(Serial p) {
  try {
    if (p.available() > 0) {
      myString = p.readStringUntil('.'); //agl, dis.
      if (myString != null) {
        String[] strArr = split(myString, ',');
        angle = int(strArr[0]);
        distance = int(strArr[1].replace(".", ""));
        println(angle);
        println(distance);
      }
    }
  }
  catch(Exception e) {
  }
}
