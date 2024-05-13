#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <FS.h> // Include the SPIFFS library
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
// unsigned long sendData PrevMillis=0;
bool signupOK = false;
int ldrData = 0;

const char* ssid = "Hotspot lelo";
const char* password = "password";

#define DHTPIN 4
#define DHTTYPE DHT11
#define api "AIzaSyChuxFIjmjICbbTGT1XOFtxRmrlkWw53TM"
#define url "https://iot69-f4b58-default-rtdb.firebaseio.com/"

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  delay(100);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = api;
  config.database_url = url;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("signUp OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  dht.begin();
}

void loop() {
  delay(5000); // Adjust delay according to your requirement

  if (WiFi.status() == WL_CONNECTED) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Get epoch timestamp
    unsigned long epochTime = time(nullptr);

    // Create unique node names with timestamp
    String tempPath = "Sensor/" + String(epochTime) + "/temp";
    String humidityPath = "Sensor/" + String(epochTime) + "/hum";

    if (Firebase.RTDB.setInt(&fbdo, tempPath.c_str(), temperature)) {
      Serial.println();
      Serial.print(temperature);
      Serial.print(" - successfully saved to: ");
      Serial.println(fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, humidityPath.c_str(), humidity)) {
      Serial.println();
      Serial.print(humidity);
      Serial.print(" - successfully saved to: ");
      Serial.println(fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
}
