// Automatic Watering System
// Created on: March,17,2020
// Last Modified:  March,20,2020

#include <LiquidCrystal.h>

// Ports
const int Water_Pump = 2;
const int Moisture_Sensor = A0;
const int Watering_Potentiometer = A1;
const int LED_Drysoil = 11;                             // red LED
const int LED_Fill_Water = 12;                          // blue LED

// Important Values
const int Required_Alert_Time_Before_Watering = 20;     // 300 sec or 5 min
double    Watering_Duration = 5;                        // exactly 8OZ / 236.6CC   per 10 sec
const int Dry_Soil_Moisture_Percentage = 35 ;           // 70% is good soil  20% and below is bad
const int Air_Value = 670;                              
const int Water_Value = 350;

// Other Variables
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool Alert_On = false;
int Moisture_Value;
int Moisture_Percentage;
double Time;
double Main_Time ;
double Alert_Time;
double MotorOn_Time;


void setup() {
  Serial.begin(9600);
  pinMode(Water_Pump, OUTPUT);
  pinMode(LED_Drysoil, OUTPUT);
  digitalWrite(Water_Pump, LOW);  
  lcd.begin(16,2);
}

void loop() {
  Time_Update();
  Main_Time = Time;

  digitalWrite(Water_Pump, LOW);

  Watering_Duration = analogRead(Watering_Potentiometer);
  Watering_Duration = map(Watering_Duration, 1,1024, 1, 48);
  Watering_Duration = (Watering_Duration)/2 ;

  Calculate_Moisture();
  SerialPrint();
  LCDPrint();
  
  if (Moisture_Percentage <= Dry_Soil_Moisture_Percentage){
    Alert_On = true;}
  
  while (Alert_On == true){
    
    digitalWrite(Water_Pump, LOW);
    Calculate_Moisture();
    SerialPrint();
    LCDPrint();

    if (Moisture_Percentage > Dry_Soil_Moisture_Percentage){
      Alert_On = false;}

    Time_Update();
    Alert_Time = Time;
    Alert_Time = Alert_Time - Main_Time;
    
    if (Alert_Time >= Required_Alert_Time_Before_Watering){
      Alert_On = false;
      Time_Update();
      Main_Time = Time;  
      Alert_Time = 0;
      MotorOn_Time = 0;
      
      while (MotorOn_Time <= Watering_Duration){
        Time_Update();
        MotorOn_Time = Time;
        MotorOn_Time = MotorOn_Time - Main_Time;

        Calculate_Moisture();
        Serial.print("Watering   ");
        Serial.print(MotorOn_Time);
        SerialPrint();
        LCDPrint();
        
        digitalWrite(Water_Pump, HIGH); 
       
      }}}
  


  delay(500);
}

/**************************************/

void Time_Update(){
  Time = millis();
  Time = round(Time/100);
  Time = Time/10 ;}

void Calculate_Moisture(){
  Moisture_Value = analogRead(Moisture_Sensor);
  Moisture_Percentage = map(Moisture_Value, Air_Value, Water_Value, 0, 100);

  if (Moisture_Percentage <= Dry_Soil_Moisture_Percentage) {
   digitalWrite(LED_Drysoil, HIGH);}
  else {
   digitalWrite(LED_Drysoil, LOW); }}
    
void SerialPrint(){
  Serial.print("\t Moisture Level = ");
  Serial.print(Moisture_Value);
  Serial.print("\t Moisture Percentage = %");
  Serial.print(Moisture_Percentage);
  Serial.print("\t Time: ");
  Serial.print(Time);
  Serial.print("\t Watering Duration = ");
  Serial.print(Watering_Duration);
  if (Alert_Time > 0){
   Serial.print("\t Alert Time: ");
   Serial.print(Alert_Time);
   Serial.println("\t :O");}
  else{
    Serial.println("\t :D");}}

void LCDPrint(){
  lcd.clear();
  lcd.print("Moisture");
  lcd.setCursor(0, 1);
  lcd.print("%");
  lcd.print(Moisture_Percentage);
  lcd.setCursor(9,1);
  lcd.print(Time);
}
