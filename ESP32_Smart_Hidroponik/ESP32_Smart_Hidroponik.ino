/**
   Created by K. Suwatchai (Mobizt)

   Email: k_suwatchai@hotmail.com

   Github: https://github.com/mobizt/Firebase-ESP-Client

   Copyright (c) 2022 mobizt

*/

//matiin serial.print buat ngurangin delay

// This example shows how to send data fast and continuously.
#include <ArduinoJson.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "SmartHidroponik"
#define WIFI_PASSWORD "qwertyui"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyAAR77JIXZGCPo_kVByNFz9FP-GsRyRpH4"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://smarthidroponic-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or \
  //<databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in
   your project */
#define USER_EMAIL "dikalwan@smarthidroponic.com"
#define USER_PASSWORD "dikalwan"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int count = 0;

void setup()
{

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);
#if defined(ESP8266)
  fbdo.setBSSLBufferSize(512, 2048);
#endif
}

void loop()
{

  //set data firebase
  while (Serial.available()) {
    StaticJsonDocument<200> doc;
    DeserializationError err = deserializeJson(doc, Serial);
    if (err == DeserializationError::Ok)
    {
      //const char* tanggal = doc["tanggal"];
      //const char* waktu = doc["waktu"];
      
      float ph = doc["ph"];
      float tds = doc["tds"];
      float hcsr = doc["hcsr"];
      float turb = doc["turb"];

      if (Firebase.ready())
      {
       
        Serial.printf("Set tds... %s\n", Firebase.RTDB.setFloat(&fbdo, "/transaction/tds", tds) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Set turb... %s\n", Firebase.RTDB.setFloat(&fbdo, "/transaction/turb", turb) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Set ph... %s\n", Firebase.RTDB.setFloat(&fbdo, "/transaction/ph", ph) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Set hcsr... %s\n", Firebase.RTDB.setFloat(&fbdo, "/transaction/hcsr", hcsr) ? "ok" : fbdo.errorReason().c_str());
        
      }
    }
    else
    {
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      while (Serial.available() > 0)
        Serial.read();
    } 
  }
  
  //Get Controlling
  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 300 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    bool autoSystem, mixer, nutrisiA, nutrisiB, phDown, phUp, pumpState, solenoid, waterDrain;

    if (Firebase.RTDB.getBool(&fbdo, "/control/autoSystem")) {
      if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        autoSystem = fbdo.to<bool>();
//        Serial.print("autoSystem: ");
//        Serial.println(autoSystem);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "/control/mixer")) {
      if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        mixer = fbdo.to<bool>();
//        Serial.print("mixer: ");
//        Serial.println(mixer);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "/control/nutrisiA")) {
      if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        nutrisiA = fbdo.to<bool>();
//        Serial.print("nutrisiA: ");
//        Serial.println(nutrisiA);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "/control/nutrisiB")) {
      if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        nutrisiB = fbdo.to<bool>();
//        Serial.print("nutrisiB: ");
//        Serial.println(nutrisiB);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "/control/phDown")) {
      if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        phDown = fbdo.to<bool>();
//        Serial.print("phDown: ");
//        Serial.println(phDown);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "/control/phUp")) {
      if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        phUp = fbdo.to<bool>();
//        Serial.print("phUp: ");
//        Serial.println(phUp);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "/control/pumpState")) {
      if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        pumpState = fbdo.to<bool>();
//        Serial.print("pumpState: ");
//        Serial.println(pumpState);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "/control/solenoid")) {
      if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        solenoid = fbdo.to<bool>();
//        Serial.print("solenoid: ");
//        Serial.println(solenoid);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "/control/waterDrain")) {
      if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        waterDrain = fbdo.to<bool>();
//        Serial.print("waterDrain: ");
//        Serial.println(waterDrain);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }

    String sendToIno = "*" + String(autoSystem) + String(mixer) + String(nutrisiA) + String(nutrisiB) + String(phDown) + String(phUp) + String(pumpState) + String(solenoid) + String(waterDrain) + "#";
    Serial.println(sendToIno);

    //    Serial.print("Set int async... ");
    //
    //    //    for (size_t i = 0; i < 10; i++)
    //    //    {
    //
    //    // The purpose for this async function is to set, push and update data instantly but...
    //
    //    // The async function is implemented by the library and not supported by Firebase.
    //
    //    // When use this async function, the response will be ignored, then sometimes server may reject the subsequence requests or
    //    // full of receive buffer can be interrupted the async continuous operation.
    //
    //    // You do not need to use these async functions in general usage.
    //
    //    // Use library in Test Mode (see examples/Authentication/TestMode) or legacy database secret authentication,
    //    // the speed of data transfer will be increased.
    //
    //    Firebase.RTDB.setBoolAsync(&fbdo, "/control/true", true);
    //    Firebase.RTDB.setBoolAsync(&fbdo, "/control/false", false);
    //    //      count++;
    //    //    }
    //    Serial.println("ok");
  }
}
