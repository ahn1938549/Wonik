#include <SoftwareSerial.h>
#include <IRremote.hpp>
#define IN_1 30
#define IN_2 31
#define Motor_EN 62
#define MOVE_1 66
#define MOVE_2 67
#define IR_RECEIVE_PIN 2
#define MOTOR_STOP 0


long time = millis();
uint32_t ir = 777;
byte Motor_status = MOTOR_STOP;
byte Pr_Motor_status = MOTOR_STOP;

bool front_sensor, rear_sensor, flag;

void setup() {

  pinMode(IN_1, INPUT); 
  pinMode(IN_2, INPUT); 
  pinMode(MOVE_1, OUTPUT); 
  pinMode(MOVE_2, OUTPUT); 
  pinMode(Motor_EN, OUTPUT); 
  //Serial3.begin(57600);
  Serial.begin(115200);
  digitalWrite(Motor_EN,MOTOR_STOP);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

}

void loop() {

  if (IrReceiver.decode())
  {
    if(IrReceiver.decodedIRData.decodedRawData != (0XFFFF && 0X000))
    {

      if(IrReceiver.decodedIRData.decodedRawData == 3141860100)
      {
        Motor_status = 0;
        Serial.println("STOP");
      }
      else if(IrReceiver.decodedIRData.decodedRawData == 3208706820)
      {
        Pr_Motor_status = Motor_status;
        Motor_status = 1;
        Serial.println("GO IN");
      }
      else if(IrReceiver.decodedIRData.decodedRawData == 3191995140)
      {
        Pr_Motor_status = Motor_status;
        Motor_status = 2;
        Serial.println("GO OUT");
      }
    }
    ir = 0;
    IrReceiver.resume();
  }
  front_sensor = digitalRead(IN_1);
  rear_sensor = digitalRead(IN_2);

  if(Motor_status == 0)
  {
    digitalWrite(Motor_EN,LOW); 
  }
  /* 벽이 없을 경우
  if (millis() - time > 500 && flag == 2)
  {
    digitalWrite(Motor_EN,LOW);
    Serial.println("Soon Stop to Wall");
    flag = 0;
    Motor_status =0;
    Pr_Motor_status = 0;
  } 
  */
  if (millis() - time > 1000 && flag == 1)
  {
    digitalWrite(Motor_EN,LOW);
    Serial.println("Soon Stop to ground");
    flag = 0;
    Motor_status =0;
    Pr_Motor_status = 0;
  } 
  if(Motor_status == 1)
  {
    if(Pr_Motor_status ==2)
    {
      digitalWrite(Motor_EN,LOW); 
      Serial.print("Wait");
      time = millis();
      while (millis() - time < 1500)
      {
        digitalWrite(Motor_EN,LOW); 
      }
      Serial.println("Done");
      digitalWrite(MOVE_1,HIGH);
      digitalWrite(MOVE_2,LOW);
      delay(100);
      digitalWrite(Motor_EN,HIGH);
      Pr_Motor_status = 1;
    }
    else
    {
      digitalWrite(MOVE_1,HIGH);
      digitalWrite(MOVE_2,LOW);
      digitalWrite(Motor_EN,HIGH);
      Serial.println("Running In");
    }
    Pr_Motor_status = Motor_status;
    
    if(rear_sensor == 1)
    {
      digitalWrite(Motor_EN,LOW);
      Serial.println("Soon Stop to Wall");
      flag = 0;
      Motor_status =0;
      Pr_Motor_status = 0;
    }
  }

  if(Motor_status == 2)
  {
    if(Pr_Motor_status ==1)
    {
      digitalWrite(Motor_EN,LOW); 
      Serial.println("Wait");
      time = millis();
      while (millis() - time < 1500)
      {
        digitalWrite(Motor_EN,LOW); 
      }
      Serial.println("Done");
      digitalWrite(MOVE_1,LOW);
      digitalWrite(MOVE_2,HIGH);
      digitalWrite(Motor_EN,HIGH);
      Pr_Motor_status = 2;
    }
    else
    {
      digitalWrite(MOVE_1,LOW);
      digitalWrite(MOVE_2,HIGH);
      digitalWrite(Motor_EN,HIGH);
      Serial.println("Running Out");
    }

    Pr_Motor_status = Motor_status;

    if(front_sensor == 1)
    {
      Serial.println("Sensor Reached");
      time = millis();
      flag = 1;      
    }
  }
}
  