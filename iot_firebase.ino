#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>                         
#define FIREBASE_HOST "YOUR_FIREBASE_HOST"
#define FIREBASE_AUTH "YOUR_AUTH_KEY"
#define WIFI_SSID "YOUR_WIFI" 
#define WIFI_PASSWORD "YOUR_PASSWORD"
FirebaseData firebaseData;
int val=0;
int gval=0;
const int buzzer = D5;
void setup() {
    pinMode(D5, OUTPUT);
    pinMode(A0, INPUT);
    Serial.begin(115200);                                                      
  Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                    
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   
  Firebase.reconnectWiFi(true);
  delay(1000);
}

void loop() {
  gval = analogRead(A0);
 Firebase.setInt(firebaseData, "/gasdata", gval);
if (Firebase.getInt(firebaseData, "/data")) {                          
    if (firebaseData.dataType() == "int") {                            
      val = firebaseData.intData();
      if(val==1){
      Serial.print("Value of Pir is:");
      Serial.print(val);
      Serial.println("(person detected)");
      Serial.println(gval);
        tone(buzzer, 1000); // Send 1KHz sound signal...
        delay(1000);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        delay(1000);   
      }
      else if(val==0){
         Serial.print("Value of Pir is:");
      Serial.print(val);
      Serial.println("(person not detected)");
        //digitalWrite(D5, LOW);
        noTone(buzzer); 
       delay(1000);
      }
}
if(gval>=800){
      Serial.print("Value of MQ-5 is:");
      Serial.print(gval);
      Serial.println("(gas leaked....!)");
        tone(buzzer, 1000); // Send 1KHz sound signal...
        delay(1000);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        delay(1000);   
      }
      else if(gval<800){
         Serial.print("Value of MQ-5 is:");
      Serial.print(gval);
      Serial.println("(Normal level)");
        //digitalWrite(D5, LOW);
        noTone(buzzer); 
       delay(1000);
      }
  } else {
    Serial.println(firebaseData.errorReason());
  }
 }
