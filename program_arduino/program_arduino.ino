#include <SoftwareSerial.h>
SoftwareSerial serial (2,3);
//----------------------------------------------------------------------------
const int api = 4;
const int api1 = 5;
const int asap = 6;
const int asap1 = 7;
const int buzz = 8;

// inialisasi variabel data
int data1,data2,data3,data4;
int hitung;

void setup()
{
    
    // inialisasi status I/O pin
    pinMode(api, INPUT);
    pinMode(asap, INPUT);
    pinMode(api1, INPUT);
    pinMode(asap1, INPUT);
    pinMode(buzz, OUTPUT);
    Serial.begin(9600);
    serial.begin(115200);
}

void loop()
{
  //baca sensor---------------------------------------
  baca_sensor();
  //--------------------------------------------------
}

void baca_sensor(){
  data1 = digitalRead(api);
  data3 = digitalRead(api1);
  data2 = digitalRead(asap);
  data4 = digitalRead(asap1);

  Serial.print("Pin Api1: ");Serial.println(data1);
  Serial.print("Pin Api2: ");Serial.println(data3);
  Serial.print("Pin Asap1: ");Serial.println(data2);
  Serial.print("Pin Asap2: ");Serial.println(data4);

  //kondisi------------------------------------------------
  if (data1 == 0 or data2 == 0 or data3 == 0 or data4 == 0)
  {
    hitung ++;
    tone(buzz, 1000, 200);
    delay(250);
    tone(buzz, 1000, 200);
    delay(250);
    if (hitung < 2){
      serial.write("kebakaran");
      Serial.println("kirim");
      
    }
    Serial.println(hitung);
    
  }
  else
  {
    Serial.println("Aman");
    delay(2000);
    hitung=0;
  }
  delay(50);
}
