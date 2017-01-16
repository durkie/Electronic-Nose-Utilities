#include <Time.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress tempSensor = { 0x28, 0x94, 0xE2, 0xDF, 0x02, 0x00, 0x00, 0xFE };

void setup() {
  Serial.begin(9600);
  sensors.begin();
  sensors.setResolution(tempSensor, 10);
}

bool state = false;

void loop() {
  state = !state;
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();
  }
  if (timeStatus() == timeSet) {
    double temperature = GetTemp();
    digitalWrite(13, HIGH); // LED on if synced
    Serial.print("A0: ");
    Serial.print(analogRead(A0));
    Serial.print(", A1: ");
    Serial.print(analogRead(A1));
    Serial.print(", A2: ");
    Serial.print(analogRead(A2));
    Serial.print(", A3: ");
    Serial.print(analogRead(A3));
    Serial.print(", A4: ");
    Serial.print(analogRead(A4));
    Serial.print(", A5: ");
    Serial.print(analogRead(A5));
    Serial.print(", A6: ");
    Serial.print(analogRead(A6));
    Serial.print(", A7: ");
    Serial.print(analogRead(A7));
    Serial.print(", Temp: ");
    Serial.print(temperature);
    Serial.print(", DBTemp: ");
    Serial.println(sensors.getTempC(tempSensor));
    digitalWrite(13, state);
    delay(30000);
  }
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.print(": ");
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void recordDigits(int digits) {
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

/* Get internal Atmega328 Temperature */
double GetTemp(void)
{
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.
  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celsius.
  return (t);
}

