#include <SoftwareSerial.h>
#include <AFMotor.h>

AF_DCMotor motor_L(1);  // 왼쪽 바퀴 1번
AF_DCMotor motor_R(4);  // 오른쪽 바퀴 4번

#define BT_RXD A4
#define BT_TXD A5
SoftwareSerial bluetooth(BT_TXD, BT_RXD);

char rec_data;

int i;
int j;

// 초음파센서 출력핀(trig) & 입력핀(echo)
int TrigPin = A0;
int EchoPin = A1;
long duration, distance;    // 기간, 거리

// 초음파 센서 함수
void Obstacle_Check();
void Distance_Measurement();
void Forward();
void Backward();
void Right();
void Left();
void Stop();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);     // 시리얼모니터 통신
  bluetooth.begin(9600);  // 블루투스 통신
  
  pinMode(EchoPin, INPUT);
  pinMode(TrigPin, OUTPUT);


  motor_L.setSpeed(200);    // 왼쪽 바퀴 속도
  motor_L.run(RELEASE);     // 왼쪽 바퀴 대기 상태
  motor_R.setSpeed(200);    // 오른쪽 바퀴 속도
  motor_R.run(RELEASE);     // 오른쪽 바퀴 대기 상태
}

void loop() {
  // put your main code here, to run repeatedly:

// 블루투스로 방향 제어
  if(bluetooth.available()) {
    rec_data = bluetooth.read();
    Serial.write(rec_data);
    switch(rec_data) {
      case 'w' :
        Forward();
        Serial.write("직진");
        break;
      case 's' :
        Backward();
        Serial.write("후진");
        break;
      case 'd' :
        Right();
        Serial.write("우회전");
        break;
      case 'a' :
        Left();
        Serial.write("좌회전");
        break;
      case 'f' :
        Stop();
        break;
    }
    Obstacle_Check();
    
  }

  
  
   // Forward();
    //delay(100);
    //Stop();
    //Obstacle_Check();
}

// 장애물 확인 및 회피 방향 결정
void Obstacle_Check() {
  int val = random(2);
  Distance_Measurement();

 // Serial.println(distance);

  if (distance < 200) {
    if(distance < 180) {
      Backward();
      delay(1000);
      Stop();
      delay(50);
      Distance_Measurement();
    }
    else {
      if(val == 0) {
        Right();
        delay(400);
        Stop();
      }
      else if(val == 1) {
        Left();
        delay(400);
        Stop();
      }
      Distance_Measurement();
    }
  }
}

// 거리 감지 함수
void Distance_Measurement() {
  digitalWrite(TrigPin, LOW);
  delay(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);    // echoPin이  HIGH를 유지한 시간
  distance = ((float)(340 * duration) / 1000) / 2;
  delay(5);
}

// 방향 제어 함수
void Forward() {
  motor_L.run(BACKWARD); motor_R.run(FORWARD);
}

void Backward() {
  motor_L.run(FORWARD); motor_R.run(BACKWARD);
}

void Right() {
  motor_L.run(BACKWARD); motor_R.run(BACKWARD);
}

void Left() {
  motor_L.run(FORWARD); motor_R.run(FORWARD);
}

void Stop() {
  motor_L.run(RELEASE); motor_R.run(RELEASE);
}
