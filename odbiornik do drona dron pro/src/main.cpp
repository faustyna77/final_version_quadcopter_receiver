#include <Arduino.h>


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//Define widths
int pwm_width_2 = 0;
int pwm_width_3 = 0;
int pwm_width_4 = 0;
int pwm_width_5 = 0;
int pwm_width_6 = 0;
int pwm_width_7 = 0;

Servo PWM2;
Servo PWM3;
Servo PWM4;
Servo PWM5;
Servo PWM6;
Servo PWM7;

  
struct MyData {
byte throttle;      
byte yaw;
byte pitch;
byte roll;
byte AUX1;
byte AUX2;
};
MyData data;


const uint64_t pipeIn = 0xE8E8F0F0E1LL;     
RF24 radio(7, 8); 



void resetData()
{

data.throttle = 0;
data.yaw = 0;
data.pitch = 0;
data.roll = 0;
data.AUX1 = 0;
data.AUX2 = 0;
}



void setup()
{
  
   PWM2.attach(10);
  PWM3.attach(3);
  PWM4.attach(9);
  PWM5.attach(5);
  
  resetData();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);  
  radio.openReadingPipe(1,pipeIn);
  
  
  radio.startListening();
}



unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(MyData));
lastRecvTime = millis(); 
}
}



void loop()
{
recvData();
unsigned long now = millis();

if ( now - lastRecvTime > 1000 ) {

resetData();
}

pwm_width_2 = map(data.throttle, 0, 255, 1000, 2000);     
pwm_width_3 = map(data.yaw,      0, 255, 1000, 2000);     
pwm_width_4 = map(data.pitch,    0, 255, 1000, 2000);    
pwm_width_5 = map(data.roll,     0, 255, 1000, 2000);     
int analogreader=analogRead(A0);
int analogVal=map(analogreader,0,1023,1000,2000);
if(analogVal>1050)
{

 PWM2.writeMicroseconds(analogVal);
PWM3.writeMicroseconds(analogVal);
PWM4.writeMicroseconds(analogVal);
PWM5.writeMicroseconds(analogVal);
}else{




PWM2.writeMicroseconds(pwm_width_2);
PWM3.writeMicroseconds(pwm_width_3);
PWM4.writeMicroseconds(pwm_width_4);
PWM5.writeMicroseconds(pwm_width_5);

}
}
