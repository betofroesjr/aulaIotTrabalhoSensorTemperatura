#include <DHT.h>
#include <ESP8266WiFi.h>

const char *ssid = "nomeRede";
const char *pass = "senhaWifi";

#define DHTPIN 5
DHT dht(DHTPIN, DHT11);

const char* server = "api.thingspeak.com";
String apiKey = "VN8MFD6TGAPVD87X";
WiFiClient client;

String thingtweetAPIKey = "MOAQ3H7JGLOPXS9C";

void updateTwitterStatus(String tsData)
{
  if (client.connect(server, 80))
  {
    // Create HTTP POST Data
    tsData = "api_key=" + thingtweetAPIKey + "&status=" + tsData;
    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    digitalWrite(2, LOW); // Acende o Led
    Serial.print(".");
  }
  digitalWrite(2, HIGH); // Apaga o Led
  Serial.println("");
  Serial.println("WiFi connected");
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  Serial.print(t);
  Serial.print(" degrees Celcius, Humidity: ");
  Serial.print(h);
  Serial.println("%. Send to Thingspeak."); 

  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
    {
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(t);
      postStr += "&field2=";
      postStr += String(h);
      postStr += "\r\n\r\n";
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.print(" degrees Celcius, Humidity: ");
      Serial.print(h);
      Serial.println("%. Send to Thingspeak.");
    }
    client.stop();

    if (t > 27.0f) {
      updateTwitterStatus("Tweet enviado pela NodeMCU do José Humberto aula teste sobre a temperatura acima de 27ºC no Lab de Robótica da PÓS da Faculdade Delta");
       digitalWrite(2, LOW); // Acende o Led
       Serial.println(" Acende o Led...");
    } else {
       digitalWrite(2, HIGH); // Apaga o Led
       Serial.println("Apaga o Led...");
    }
  
    Serial.println("Waiting...");
    
  delay(500);
}
