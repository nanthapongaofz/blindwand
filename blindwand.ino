//โมดูล MP3
const uint8_t ARDUINO_RX = 7; //ขารับสัญญาณโมดูล MP3
const uint8_t ARDUINO_TX = 6; //ขาส่งสัญญาณโมดูล MP3

//เซนเซอร์วัดระยะ
const int EchoPin = 8; //ขาส่งคลื่นสัญญาณอัลตร้าโซนิค
int TrigPin = 9; //ขารับคลื่นสัญญาณอัลตร้าโซนิค

//เงื่อนไขตัวแปลข้อมูลแบบ Serial
#ifndef USE_SOFTWARESERIAL
#define USE_SOFTWARESERIAL 1
#endif

//ประกาศใช้งาน Libralies โมดูล MP3
#include <MD_YX5300.h>

//ประกาศใช้งาน Libralies โมดูล ประมวลผล MP3
#include <MD_cmdProcessor.h>

//ประกาศเงื่อนไขการใช้ตัวแปร
#if USE_SOFTWARESERIAL

//ประกาศใช้งาน Libralies ซอฟแวร์ซีเรียล
#include <SoftwareSerial.h>

//ประกาศให้ซอฟแวร์ซีเรียลเป็น MP3
SoftwareSerial  MP3Stream(ARDUINO_RX, ARDUINO_TX);
#define Console Serial
#else
#define MP3Stream Serial2
#define Console   Serial
#endif
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
MD_YX5300 mp3(MP3Stream);

 
void setup() {
  //ให้โมดูล MP3 เริ่มทำงาน
  MP3Stream.begin(MD_YX5300::SERIAL_BPS); 
  mp3.begin();
  
  //เริ่มใช้คำสั่งแบบซีเรียล
  Serial.begin(9600);
}
 
void loop() {
  mp3.check(); //ให้โมดูล MP3 เช็คสถานะ
  long duration, cm; //ประกาษตัวแปรชนิด long เป็น duration และ cm
  pinMode(EchoPin, OUTPUT); //ตั้งค่าขา EchoPin เป็น Output
  digitalWrite(EchoPin, LOW); //สั่งให้ขา EchoPin = LOW หรือ 0
  delayMicroseconds(2); // ดีเลย์คำสั่ง 2 millisecond
  digitalWrite(EchoPin, HIGH); //สั่งให้ขา EchoPin = HIGH หรือ 1
  delayMicroseconds(5); // ดีเลย์คำสั่ง 5 millisecond
  digitalWrite(EchoPin, LOW); //สั่งให้ขา EchoPin = LOW หรือ 0
  pinMode(TrigPin, INPUT); //ตั้งค่าขา EchoPin เป็น Input
  duration = pulseIn(TrigPin, HIGH); //ประกาศให้ตัวแปร duration ดึงค่าขา TrigPin เป็น 1
  cm = microsecondsToCentimeters(duration); //ประกาศให้ตัวแปร cm คำนวณค่าซีเรียลให้เป็นเซนติเมตร

  //ตั้งเงื่อนไขถ้าระยะของวัตถุเข้าใกล้ 30 เซนติเมตรสั่งให้เล่นเสียงเตือน
  if (cm < 30){
    mp3.check();
    mp3.playTrack(1);
    delay(3000);
  }
  //ถ้าไม้เข้าเงื่อนไขจะไม่มีอะไรเกิดขึ้้นแต่จะส่งค่าไปที่ตัวควบคุม
  else {
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  }  
}
//ประกาศตัวแปรชนิด long เพื่อสร้างความเสถียรให้ระบบ
long microsecondsToCentimeters(long microseconds) {
return microseconds / 29 / 2;
}