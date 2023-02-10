#include <SoftwareSerial.h>

byte Received[6];
int i =0;
byte leng;
bool Connected = 0;
int sent = 0;
byte mode = 2;
int move = 0;
unsigned long time;
byte Start[3] = {0x00, 0x02, 0x02};
byte On[3] = {0x00, 0x00, 0x00};
byte IN[3] = {0X01, 0x00, 0x01};
byte OUT[3] = {0X02, 0x00, 0x02};
byte Slave[3] = {0X10, 0x01, 0x11};
byte Slave_Received[3];

void setup() {                 
 Serial.begin(19200); 
 Serial1.begin(19200,SERIAL_8N1);
 Serial1.write(Start, sizeof(Start));
 //time = millis();
 
 
}

void loop() 
{


  if(millis() - time > 500)
  {
    if(move == 1)
    {
      Connected = 0;
      mode = 0;
      sent = 3;
      Serial.println("Done Close System");
      Serial1.write(Start, sizeof(Start));
    }
    

    if(Connected == 1 && move == 0)
    {
      if(mode == 1)
      {
        Serial1.write(IN, sizeof(IN));
      }
      else if(mode == 2)
      {
        Serial1.write(OUT, sizeof(OUT));
      }
      move = 1;
    }

    else if(sent !=3)
    {
      Serial1.write(On, sizeof(On));  
      sent++;    
      Serial.print("Sending Order :       ");
      Serial.println(sent);
    }
   
    if(Serial1.available() > 0)
    {
    leng = Serial1.readBytes(Received, 6);
    for(i = 3; i < 6; i++)
    {
      //Serial.print(Received[i],HEX);
      Slave_Received[i -3] = Received[i] & 0xFF;
    }
    if(memcmp(Slave_Received,Slave,3) == 0 ) Connected =1;
    //Serial.print("                             ");
    //Serial.println(Connected);
    }
    

    time = millis();
  }
}



    
