#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "MyASUS";
const char* password = "hy12345678";

String data;
char c;

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

void setup()
{
    Serial.begin(115200);
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
  while(Serial.available()>0){
    delay(10);
    c = Serial.read();
    data+=c;
  }
  if (data.length()>0){
    Serial.println(data);
    if (data == "kebakaran"){
      bot.sendMessage(CHAT_ID, "Kebakaran Terdeteksi", "");
      data ="";
    }
  }else{
    Serial.println("aman");
  }
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
  delay(1000);
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
      if (data == "kebakaran"){
        bot.sendMessage(CHAT_ID, "Kebakaran Terdeteksi", "");
      }else if((data == "")){
        bot.sendMessage(CHAT_ID, "Aman", "");
      }
    }    
  }
}
