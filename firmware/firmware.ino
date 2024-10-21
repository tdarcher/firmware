//#include <SoftwareSerial.h>


const int CO2_PWR = 2;     // the number of the pushbutton pin
const int GSM_PWR =  3;      // the number of the LED pin

//SoftwareSerial mySerial(2,3); //RX,TX

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(GSM_PWR, OUTPUT);
  pinMode(CO2_PWR, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(GSM_PWR, LOW);
  digitalWrite(CO2_PWR, LOW);
  digitalWrite(LED_BUILTIN,LOW);
  /*Serial.begin(115200);
  mySerial.begin(115200);
  delay(1000);
  mySerial.println("AT");
  delay(100);
  mySerial.println("AT");
  long b=mySerial.read();
  Serial.write(b);
  delay(100);
  b="";
  mySerial.println("AT+CIPMUX=1");
  b=mySerial.read();
  Serial.write(b);
  delay(100);
  b="";
  mySerial.println("AT+CIPSERVER=1,80");
  b=mySerial.read();
  Serial.write(b);
  delay(100);
  b="";
  mySerial.println("AT+CIFSR");
  b=mySerial.read();
  Serial.write(b);
  delay(100);
  b="";*/
}

void loop(){
  /*digitalWrite(GSM_PWR, LOW);
  digitalWrite(CO2_PWR, LOW);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);
  digitalWrite(GSM_PWR, HIGH);
  digitalWrite(CO2_PWR, HIGH);  // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(5000);  */
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }
}
