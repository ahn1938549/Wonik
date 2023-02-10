#include <SoftwareSerial.h>
#define IN_1 30
#define IN_2 31
#define Motor_EN 62
#define MOVE_1 66
#define MOVE_2 67
#define MOTOR_STOP 0

byte Received[6]; 
byte Master_Received[3]; //Master_PIO로 부터 받은 값
int i =0;
byte leng;
byte mode = 0; // 0 : Sleep, 1: IN, 2: OUT
unsigned long time;
unsigned long out_time;
bool out_flag = 0;
byte Process[3] = {0X10, 0x01, 0x11};
byte Done[3] = {0X20, 0x01, 0x21};
byte Master_IN[3] = {0X01, 0x01, 0x02}; 
byte Master_OUT[3] = {0X02, 0x01, 0x03};
byte Motor_status = MOTOR_STOP;
byte Pr_Motor_status = MOTOR_STOP;
bool front_sensor, rear_sensor, flag, stop;

void setup() 
{       
  pinMode(IN_1, INPUT); 
  pinMode(IN_2, INPUT); 
  pinMode(MOVE_1, OUTPUT); 
  pinMode(MOVE_2, OUTPUT); 
  pinMode(Motor_EN, OUTPUT); 
  digitalWrite(Motor_EN,MOTOR_STOP);
  Serial.begin(19200); 
  Serial1.begin(19200,SERIAL_8N1);
  Serial1.write(Process, sizeof(Process));
  time = millis();
}

void loop() {

  if(stop == 1) 
  {
    mode = 0;
    Motor_status =0;
    Pr_Motor_status = 0;
    digitalWrite(Motor_EN,LOW);
    stop = 0;
    Serial.println("Finished, Stop all!!!!");
    delay(5000);
  }

  else if(millis() - time > 300)
  {
    Serial1.flush();
    Serial1.write(Process, sizeof(Process));
    //Serial.print(Motor_status);
    front_sensor = digitalRead(IN_1);
    rear_sensor = digitalRead(IN_2);
    
    if(rear_sensor == 1)
    {
      if(mode == 1)
      {
        stop = 1;
      }
    }

    else if(front_sensor == 1)
    {
      if(mode == 2)
      {
        
        //Serial.println("Soon Stop to Wall");
        if(out_flag == 1 && millis() - out_time > 1500)
        {
          stop =1;
          out_flag = 0;
        }
        else if(out_flag == 0)
        {
          out_time = millis();
          out_flag =  1;
        }
      }      
    }
/////////////////////////////////////////////////////////////
    else if(mode == 0) // 대기중
    {
      digitalWrite(Motor_EN,LOW);
      Motor_status = 0;
      Serial.println("Looking For AGV");
    }

    else if(mode == 1) // 물체가 접촉했을 때
    {
      Pr_Motor_status = Motor_status;
      Motor_status = 1;
      digitalWrite(MOVE_1,HIGH);
      digitalWrite(MOVE_2,LOW);
      delay(100);
      digitalWrite(Motor_EN,HIGH);
      Serial.println("Running as Mode 1");
    }

    else if(mode == 2) // 물체가 접촉했을 때
    {
      Pr_Motor_status = Motor_status;
      Motor_status = 2;
      digitalWrite(MOVE_1,LOW);
      digitalWrite(MOVE_2,HIGH);
      delay(100);
      digitalWrite(Motor_EN,HIGH);
      Serial.println("Running as Mode 2");
    }
    
    if(Serial1.available() > 0)
    {
      leng = Serial1.readBytes(Received, 6);
      for(i = 3; i < 6; i++)
      {
        Master_Received[i - 3] = Received[i] & 0xFF;
        //Serial.print(Received[i],HEX);
      }
      if(memcmp(Master_Received,Master_OUT,3) == 0 ) mode =2;
      else if( memcmp(Master_Received,Master_IN,3) == 0 ) mode =1;
      //Serial.print("                             ");
      //Serial.println(mode);
    }
    time = millis();
    //Serial.println(Motor_status);
  }
}


    
