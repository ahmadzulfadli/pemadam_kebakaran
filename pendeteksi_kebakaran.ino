#include <Servo.h>
Servo servo;

const int api = 8;
const int buzz = 9;
const int led = 10;
const int asap = A0;

// inialisasi variabel data
int data , inasap;

// Nilai threshold
int sensorThres = 100;

void setup()
{
    servo.attach(3);
    // inialisasi status I/O pin
    pinMode(api, INPUT);
    pinMode(asap, INPUT);
    pinMode(buzz, OUTPUT);
    pinMode(led, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    data = digitalRead(api);
    inasap = analogRead(asap);

    Serial.print("Pin A0: ");
    Serial.println(inasap);

    if (data == LOW)
    {
        digitalWrite(buzz, HIGH);
        digitalWrite(led, HIGH);
        servo.write(180);
        delay(100);
    }

    else if (analogSensor > sensorThres)
    {
        digitalWrite(led, HIGH);
        tone(buzz, 1000, 200);
        servo.write(100);
        delay(100);
        
    }

    else
    {

        digitalWrite(led, LOW);
        digitalWrite(buzz, LOW);
        servo.write(0);
        delay(100);
    }
}
