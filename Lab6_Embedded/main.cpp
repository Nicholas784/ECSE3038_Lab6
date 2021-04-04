#include <Arduino.h>
#include <SoftwareSerial.h>

#define DEBUG true

SoftwareSerial esp (10,11);

String sendData(String command, const int timeout, boolean debug);
float getWaterLevel();
int tankID();

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    esp.begin(115200);

    sendData("AT+RST\r\n", 10000, DEBUG);
    sendData("AT+CWLAP\r\n", 10000, DEBUG);
    sendData("AT+CWLAP=\"Coolie wifi\",\"randomPW\"\r\n", 5000, DEBUG);
}

void loop() {
  // put your main code here, to run repeatedly:
    int tank_id = tankID();

    float water_level = getWaterLevel();

    String jsonBody = "{\"tank_id\":" +String(tank_id)+  ",\"percentage_full\":" +String(water_level)+ "\r\n\r\n}";

    int length = jsonBody.length() - 4;

    sendData("AT+CIPSTART=\"TCP\",\"10.22.0.35\",5000\r\n", 1000, DEBUG);

    String post = "POST /tank HTTP/1.1\r\nHost: 10.22.0.35:5000\r\nContent-Type: application/json\r\nContent-Length:"+String(length)+"\r\n\r\n"+jsonBody;

    int postLength = post.length();

    sendData("AT+CIPSEND="+String(postLength)+"\r\n", 3000, DEBUG);

    sendData(post, 5000, DEBUG);

}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";

    esp.print(command);

    long int time = millis();

    while ((time + timeout) > millis())
    {
        while(esp.available())
        {

            char c = esp.read();
            response += c;

        }
    }

    if (debug)
    {
        Serial.print(response);
    }

    return response;
}

float getWaterLevel()
{
    int decimal = random(0, 100)/100;
    float water_level = random(10, 199) + decimal;

    return water_level;
}

int tankID()
{
    int tank_id = random(1, 5);

    return tank_id;
}