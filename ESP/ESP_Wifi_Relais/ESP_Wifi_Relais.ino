#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
// Set these to run example.
#define WIFI_SSID "knispel"
#define WIFI_PASSWORD "mann!heim-06"
#define FIREBASE_DB_URL "homeespautomation.firebaseio.com"
#define FIREBASE_DB_SECRET_KEY "PTMceNXyvLk3m5OFFSfz02SPxkyof50AE0nFFUE4"
#define RelaisPin 0
#define TasterPin D1


void setup() {
  Serial.begin(9600);

  pinMode(0, OUTPUT);
  //pinMode(TasterPin,INPUT_PULLUP);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("Wifi ok");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_DB_URL, FIREBASE_DB_SECRET_KEY);
  Firebase.stream("/automation");


}


void loop() {

  if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());
  }
 


  if (Firebase.available()) {
    Serial.println("Firebase available");
    FirebaseObject event = Firebase.readEvent();
    String eventType = event.getString("type");
    eventType.toLowerCase();
    Serial.print(eventType);
    if (eventType == "put") {
      String path = event.getString("path");
      String data = event.getString("data");
      Serial.print(path);
      Serial.print(path);
      
      if (path.equals("/fan/value")) {
        if (data.equals("off")) {
          Serial.println("fan off");
          digitalWrite(RelaisPin,0);
          byte close[]={0xA0,0x01,0x01,0xA2};
          Serial.write(close,sizeof(close));
        } else {
          Serial.println("fan on");
          digitalWrite(RelaisPin,1);
          byte open[]={0xA0,0x01,0x00,0xA1};
          Serial.write(open,sizeof(open));
        }
      } else if (path.equals("/light/value")) {
        if (data.equals("off")) {
          Serial.println("light off");
          //digitalWrite(D2, HIGH);
        } else {
          Serial.println("light on");
          //digitalWrite(D2, LOW);
        }
      }
    }
  }
}
