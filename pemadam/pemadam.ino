#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "MyASUS";
const char* password = "hy12345678";

//----------------------------------------------------------------------------
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 25200;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds );

//----------------------------------------------------------------------------
#define BOT_TOKEN "5826393016:AAE-yvVDUve_SF1HllzM5GG6mD6ya-zH-PE"
#define CHAT_ID "1361104370"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
//----------------------------------------------------------------------------

const int api = 16;
const int api1 = 5;
const int asap = 4;
const int asap1 = 0;
const int buzz = 12;

// inialisasi variabel data
int data1,data2,data3,data4;

void setup()
{
    
    // inialisasi status I/O pin
    pinMode(api, INPUT);
    pinMode(asap, INPUT);
    pinMode(api1, INPUT);
    pinMode(asap1, INPUT);
    pinMode(buzz, OUTPUT);
    Serial.begin(9600);

    //-----------------------------------------
    WiFi.begin(ssid, password);
    Serial.print("Connecting.");
  
    while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("connected");
    timeClient.begin();
    timeClient.update();
    
    // Add root certificate for api.telegram.org
    configTime(0, 0, "pool.ntp.org");
    secured_client.setTrustAnchors(&cert);
    bot.sendMessage(CHAT_ID, "Sistem Berjalan", "");
}

void loop()
{
  //baca sensor---------------------------------------
  baca_sensor();
  //--------------------------------------------------

  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

void baca_sensor(){
  int hitung = 0;
  
  data1 = digitalRead(api);
  data2 = digitalRead(asap);
  data3 = digitalRead(api1);
  data4 = digitalRead(asap1);

  Serial.print("Pin Api1: ");Serial.println(data1);
  Serial.print("Pin Api2: ");Serial.println(data3);
  Serial.print("Pin Asap1: ");Serial.println(data2);
  Serial.print("Pin Asap2: ");Serial.println(data4);

  //kondisi------------------------------------------------
  if (data1 == 1 && data2 == 1 && data3 == 1 && data4 == 1)
  {
    Serial.println("Aman");
  }

  else
  {
    if(hitung < 1){
       bot.sendMessage(CHAT_ID, "Kebakaran Terdeteksi", "");
    }
    tone(buzz, 1000, 200);
    delay(250);
    tone(buzz, 1000, 200);
    delay(250);
    hitung+=1;
     
  }
  delay(100);
}

void handleNewMessages(int numNewMessages) {
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    
    String from_name = bot.messages[i].from_name;
    if (text == "/start") {
      String welcome = "Welcome , " + from_name + "\n";
      welcome += "Panduan Penggunaan \n";
      welcome += "/status : Mencek Status\n";
      bot.sendMessage(CHAT_ID, welcome, "");
    }
    if (text == "/status") {
      if (digitalRead(buzz)==LOW){
        bot.sendMessage(CHAT_ID, "Kondisi Aman", "");
      }else{
        bot.sendMessage(CHAT_ID, "Kebakaran Terdeteksi", "");
      }
    }    
  }
}
