// Author : Bob Yang (bob.yang.dev@gmail.com)
//
// This is a sketch to read NTC thermistor values and print it in serial port.
// Reference: https://en.wikipedia.org/wiki/Thermistor
//
// Usage: first you need to modify some B paramters of your NTC thermistor mainly Ro,To and B.
//        Ro is the resistance value of your thermistor at To temperature in Celsius.
//        B is the B value of your thermistor.
#include <math.h>


// Schematic
// Ground |--- 10k --- NTC(100k) --- 5v
//                  |--- A0 pin

// Your NTC Thermistor B parameters:
double Ro = 100.0*1000.0;  // Need to be double literal
double ToInCelsius = 25.0; // To = 25C
double B = 3950;           // B paramter of my NTC Thermistor


// converto analog voltage
double DAC(int digitalValue){
  double voltPerUnit = 5.0/1024.0;
  return voltPerUnit * digitalValue;
}

// compute the Resistenc of NTC thermistor.
// r = v / 5 * 110k;
double Resistance(int analogIn){
  double v = DAC(analogIn);
  double r = (5.0-v)/v * 10000;
  return r;
}

double CelsToKelvin(double cels){
  return cels + 273.15;
}

double KelvinToCels(double kelvin){
  return kelvin - 273.15;
}

// B parameter equation.
// 1/T = 1/To + 1/B*ln(R/Ro)
// T = To*B / (B+To*ln(R/Ro))
// Reference: https://en.wikipedia.org/wiki/Thermistor
double BParameterNtcThermistor(int RawADC) {
  double To = CelsToKelvin(ToInCelsius); // 100K@25C
  double R = Resistance(RawADC);
//  Serial.print("R: ");
//  Serial.println(R);
  double lnRRo = log(R/Ro);
  double T = To*B / (B + To*lnRRo);
  return KelvinToCels(T);
}

void setup() {
 Serial.begin(115200);
}

void loop() {
//  Serial.print("Raw Input: ");
  int rawInput = analogRead(0);
//  Serial.println(rawInput);

  double temperature = BParameterNtcThermistor(rawInput);
//  Serial.print("Temperature: ");
  Serial.println(temperature);  // display Celsius Degree Temperature.
  delay(1000);
}
