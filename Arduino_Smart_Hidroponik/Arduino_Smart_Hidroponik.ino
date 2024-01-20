
#include <SoftwareSerial.h>
SoftwareSerial linkSerial(10, 9); //tx,rx

#include <ArduinoJson.h> //JSON LIBRARY

//library pH
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
//#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;


//library tds
#include <EEPROM.h>
#include "GravityTDS.h"
#define TdsSensorPin A1
GravityTDS gravityTds;
float temperature = 25, tdsValue = 0;

//library hcsr04
#include <HCSR04.h>
HCSR04 hc(12, 11);

//inisialisasi relay
const int relay1 = 8;  //cairan pH UP
const int relay2 = 7;   //cairan pH DOWN
const int relay3 = 6;   //nutrisi A
const int relay4 = 5;   //nutrisi B
const int relay5 = 4;   //pompa kuras
const int relay6 = 3;   //valve
const int relay7 = 2;   //pompa reservoir
const int relay8 = 13;   //mixer

char get_autoSystem, get_mixer, get_nutrisiA, get_nutrisiB, get_phDown, get_phUp, get_pumpState, get_solenoid, get_waterDrain;
int i_autoSystem, i_mixer, i_nutrisiA, i_nutrisiB, i_phDown, i_phUp, i_pumpState, i_solenoid, i_waterDrain;


void setup() {

  //controlling relay
  Serial.begin(9600);
  linkSerial.begin(115200);
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization

  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(relay4,OUTPUT);
  pinMode(relay5,OUTPUT);
  pinMode(relay6,OUTPUT);
  pinMode(relay7,OUTPUT);
  pinMode(relay8,OUTPUT);

  digitalWrite(relay7, LOW);

  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,HIGH);
  digitalWrite(relay4,HIGH);
  digitalWrite(relay5,HIGH);
  digitalWrite(relay6,HIGH);
  digitalWrite(relay8,HIGH);

}

void loop() {
    if (linkSerial.available() > 0) {
    // read the incoming string:
    String incomingString = linkSerial.readStringUntil('\n');

    // prints the received data
    Serial.print("I received: ");
    Serial.println(incomingString);

    // Length (with one extra character for the null terminator)
    int str_len = incomingString.length();
    Serial.print("str_len: ");
    Serial.println(str_len);

    // Prepare the character array (the buffer)
    char char_array[str_len];

    // Copy it over
    incomingString.toCharArray(char_array, str_len);

    int strcmp1 = strcmp(char_array[0], '*');
    int strcmp2 = strcmp(char_array[10], '#');
    Serial.print("strcmp1 = ");
    Serial.println(strcmp1);
    Serial.print("strcmp2 = ");
    Serial.println(strcmp2);

    if (str_len == 12 && strcmp1 == 0 && strcmp2 == 0) {
      Serial.println("PARSING TO CHAR ARRAY");
      for (int i = 1; i < str_len - 2; i++) {
        Serial.println(char_array[i]);
      }
      get_autoSystem = char_array[1];
      get_mixer = char_array[2];
      get_nutrisiA = char_array[3];
      get_nutrisiB = char_array[4];
      get_phDown = char_array[5];
      get_phUp = char_array[6];
      get_pumpState = char_array[7];
      get_solenoid = char_array[8];
      get_waterDrain = char_array[9];
      Serial.print("get_autoSystem = ");
      Serial.println(get_autoSystem);
      Serial.print("get_mixer = ");
      Serial.println(get_mixer);
      Serial.print("get_nutrisiA = ");
      Serial.println(get_nutrisiA);
      Serial.print("get_nutrisiB = ");
      Serial.println(get_nutrisiB);
      Serial.print("get_phDown = ");
      Serial.println(get_phDown);
      Serial.print("get_phUp = ");
      Serial.println(get_phUp);
      Serial.print("get_pumpState = ");
      Serial.println(get_pumpState);
      Serial.print("get_solenoid = ");
      Serial.println(get_solenoid);
      Serial.print("get_waterDrain = ");
      Serial.println(get_waterDrain);
      
      i_autoSystem = get_autoSystem - '0';
      Serial.print("i_autoSystem = ");
      Serial.println(i_autoSystem);
      
      i_mixer = get_mixer - '0';
      Serial.print("i_mixer = ");
      Serial.println(i_mixer);
       if(i_mixer == true){
        digitalWrite(relay8, LOW);
      }
      else{
         digitalWrite(relay8, HIGH);
         }

      i_nutrisiA = get_nutrisiA - '0';
      Serial.print("i_nutrisiA = ");
      Serial.println(i_nutrisiA);
        if(i_nutrisiA == true){
        digitalWrite(relay3, LOW);
      }
      else{
         digitalWrite(relay3, HIGH);
         }


      i_nutrisiB = get_nutrisiB - '0';
      Serial.print("i_nutrisiB = ");
      Serial.println(i_nutrisiB);
       if(i_nutrisiB == true){
        digitalWrite(relay4, LOW);
      }
      else{
         digitalWrite(relay4, HIGH);
         }

      i_phDown = get_phDown - '0';
      Serial.print("i_phDown = ");
      Serial.println(i_phDown);
       if(i_phDown == true){
        digitalWrite(relay2, LOW);
      }
      else{
         digitalWrite(relay2, HIGH);
         }

      i_phUp = get_phUp - '0';
      Serial.print("i_phUp = ");
      Serial.println(i_phUp);

        if(i_phUp == true){
        digitalWrite(relay1, LOW);
      }
      else{
         digitalWrite(relay1, HIGH);
         }

      i_pumpState = get_pumpState - '0';
      Serial.print("i_pumpState= ");
      Serial.println(i_pumpState);
      if(i_pumpState == true){
        digitalWrite(relay7, HIGH);
      }
      else{
         digitalWrite(relay7, LOW);
         }

      i_solenoid = get_solenoid - '0';
      Serial.print("i_solenoid= ");
      Serial.println(i_solenoid);
       if(i_solenoid == true){
        digitalWrite(relay6, LOW);
      }
      else{
         digitalWrite(relay6, HIGH);
         }
      
      i_waterDrain = get_waterDrain - '0';
      Serial.print("i_waterDrain= ");
      Serial.println(i_waterDrain);
      if(i_waterDrain == true){
        digitalWrite(relay5, LOW);
      }
      else{
         digitalWrite(relay5, HIGH);
         }
    }
  }
  
  //ketinggian reservoir
  float hcsr = hc.dist();
  Serial.println( hcsr ); //return current distance (cm) in serial
  delay(60);

  
  //turbidity sensor
  int turbValue = analogRead(A2);// read the input on analog pin 0:
  float turbVoltage = turbValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(turbVoltage); // print out the value you read:
  delay(500);
 
  //deteksi pH
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset; 
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(pHValue,2);
        //digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
  }
 

  //TDS sensor
  //temperature = readTemperature();  //add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  delay(1000);


  // Create the JSON document
  StaticJsonDocument<200> doc;
  
  doc["ph"] = pHValue;
  doc["tds"] = tdsValue;
  doc["hcsr"] = hcsr;
  doc["turb"] = turbVoltage;

  //Mengirim JSON document melalui serial port "LINK"
  serializeJson(doc, Serial);
  serializeJson(doc, linkSerial);
  Serial.println();
  Serial.println("-----------------------------------------------------------------------------");
}


//array pH
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
