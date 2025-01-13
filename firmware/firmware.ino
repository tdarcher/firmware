
#include <SPI.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Wire.h>
#include <SensirionI2CScd4x.h>

// Define pins for SPI interface
#define CS_PIN 5
#define DC_PIN 1
#define RST_PIN 0
#define BUSY_PIN 9
#define CO2_PIN 2
#define GSM_PIN 3
#define GSM_RESET 0
#define GSM_BOOT 1

// Create display instance for 2.9" Waveshare e-Paper
GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(CS_PIN, DC_PIN, RST_PIN, BUSY_PIN));

// Create scd4x object
SensirionI2CScd4x scd4x;

void eink_display(uint16_t co2, float temperature, float humidity) {
  digitalWrite(CO2_PIN, LOW);
  digitalWrite(GSM_PIN, HIGH);  
  
  // Create strings inside this function
  String text = "CO             ppm"; // Separate CO and subscript
  String co2Value = ": " + String(co2);
  String text2 = "Temperature: " + String(temperature) + " C";
  String text3 = "Humidity: " + String(humidity) + " %";
  
  display.init();
  display.setRotation(3);
  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  
  display.setFullWindow();
  display.firstPage();
  
  do {
    display.fillScreen(GxEPD_WHITE);
    
    // Print CO and subscript
    display.setCursor(10, 30);
    display.print(text); 
    display.setCursor(37, 35); 
    display.print("2"); 
    display.setCursor(47, 30); 
    display.print(co2Value); 
    
    // Print temperature and humidity
    display.setCursor(10, 60);
    display.print(text2); 
    display.setCursor(10, 90);
    display.print(text3);    

    // Draw CO2 level bar
    int numBars = 0;
    
    if (co2 < 1000) {
      numBars = 1;
    } else if (co2 < 1500) {
      numBars = 2;
    } else if (co2 < 2000) {
      numBars = 3;
    } else {
      numBars = 4;
    }

    // Draw the bars at the bottom of the screen
    int barWidth = (display.width() - 20) / 4; // Width for each bar with some padding
    int barHeight = 10; // Height of each bar
    int yPosition = display.height() - barHeight - 10; // Position from bottom

    for (int i = 0; i < numBars; i++) {
      int xPosition = 10 + i * (barWidth + 5); // Position for each bar with spacing
      display.fillRect(xPosition, yPosition, barWidth, barHeight, GxEPD_BLACK); // Draw filled rectangle for each bar
    }

  } while (display.nextPage());
}




void EVERYTHING_OFF() {
  digitalWrite(GSM_PIN, HIGH); // high is off
  digitalWrite(CO2_PIN, HIGH); // high is off
  digitalWrite(LED_BUILTIN,HIGH); // low is on 
  digitalWrite(GSM_BOOT, LOW); // pullup to HIGH (As power is off this needs to be low)
  digitalWrite(GSM_RESET, LOW); // pullup to HIGH (As power is off this needs to be low)
}

void get_CO2() {
  uint16_t co2;
  float temperature;
  float humidity;
  uint16_t error;
  Wire.begin();
  scd4x.begin(Wire);
  scd4x.setAutomaticSelfCalibration(1);
  if (error) {
    Serial.println("Error enabling automatic self-calibration");
    return;
  }

  //error = scd4x.performForcedRecalibration(referenceCO2);
  //if (error) {
  //  Serial.println("Error performing forced recalibration");
  //  return;
  //}
  //sleep(600000)


  char errorMessage[256];
  digitalWrite(CO2_PIN, LOW);
  digitalWrite(GSM_PIN, HIGH);
  delay(500);
  error = scd4x.stopPeriodicMeasurement();
  if (error) {
    Serial.println("Error stopping periodic measurement");
    return;
  }
  delay(500);
  error = scd4x.measureSingleShot();
  if (error) {
    Serial.println("Error performing single shot measurement");
    return;
  }
  delay(5000);
  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error) {
    Serial.print("Error reading measurement: ");
    errorToString(error, errorMessage, sizeof(errorMessage));
    Serial.println(errorMessage);
    return; // Exit if there's an error reading the measurement.
  } else if (co2 == 0) {
    Serial.println("Invalid sample detected, skipping.");
    return; // Exit if the sample is invalid.
  } else {
    eink_display(co2, temperature, humidity); 
    Wire.end(); 
    return; 
  }
}

void setup() {
  Serial.begin(115200);
  
  EVERYTHING_OFF();
}

void loop() {
  get_CO2();
  EVERYTHING_OFF();
  delay(15000); // Delay between readings.
}


/*
//#include <SoftwareSerial.h>

#define GSM_RESET D0
#define GSM_BOOT D1
#define CO2_PWR  D2     // the number of the pushbutton pin
#define GSM_PWR  D3      // the number of the LED pin




#define APN iot.1nce.net



bool n = true;

void setup(){
  GSM_ON();
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(GSM_PWR, OUTPUT);
  pinMode(CO2_PWR, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);  
  pinMode(GSM_BOOT, OUTPUT);
  pinMode(GSM_RESET, OUTPUT);
  RST_GSM();
  //GSM_INIT();
}

void EVERYTHING_OFF(){
  digitalWrite(GSM_PWR, HIGH); // high is off
  digitalWrite(CO2_PWR, HIGH); // high is off
  digitalWrite(LED_BUILTIN,HIGH); //low is on 
  digitalWrite(GSM_BOOT, LOW); //pullup to HIGH (As power is off this needs to be low)
  digitalWrite(GSM_RESET, LOW); //pullup to HIGH (As power is off this needs to be low)
}




bool wait_connect(){
  Serial.print("Connecting.....");
  int k=0;
  bool connected =false;
  while (!connected){ 
    char responseBuffer[30] = {""};
    const char* substring ="IP";
    int i = 0;
    while( !Serial1.available() ) {
      delay(10);
      i++;
      if (i>5000){
        Serial.write("No serial1");
        break;
      }
    } 
    int j=0;
    while( Serial1.available() ) {
      responseBuffer[j] +=Serial1.read();
      j++;
    }
    responseBuffer[j] +='\0';
    String tmp = String(responseBuffer);
    if(tmp.indexOf(substring) >=0) {
      connected =true;
      Serial.write(responseBuffer);
    }
    delay(10);
    if (k>5000){
      Serial.write("Not connected");
      break;
    }
  }
  Serial.println("Done");
  return connected;
}


void wait_response(){
    char responseBuffer[30] = {""};
    int i = 0;
    while( !Serial1.available() ) {
      delay(10);
      i++;
      //if (i>5000){
      //  Serial.write("No serial data");
      //  break;
      //}
    } 
    int j=0;
    while( Serial1.available() ) {
      responseBuffer[j] +=Serial1.read();
      j++;
    }
    responseBuffer[j] +='\0';
    Serial.write(responseBuffer);
}


void GSM_INIT(){
  GSM_ON();
  delay(1000);
  Serial.print("GSM initalization.....");
  int i = 0;
  while( !Serial1.available() ) {
    delay(100);
    i++;
    if (i>10000){
      Serial.write("No serial");
      break;
    }
  } 
  Serial1.println("AT");
  wait_response();
  //Serial.println("1");
  //Serial1.println("AT+QSCLK=0");  // disable sleep
  //wait_response();
  //Serial.println("2");
  //Serial1.println("AT+CFUN=0"); //turn off radio
  //wait_response();
  //Serial.println("3");
  Serial1.println("AT+QBAND=1,20"); //select band 
  wait_response();
  //Serial.println("4");
  //Serial1.println("AT+QCGDEFCONT=\"IP\",\"iot.1nce.net\"");  // set APN
  //wait_response();
  //Serial.println("5");
  //Serial1.println("AT+QRST=1");  // restart module
  //wait_response();
  //Serial.println("Done");
  //delay(5000);
  GSM_OFF();
}

void GSM_OFF(){
  // Turn OFF GSM
  digitalWrite(GSM_PWR, HIGH); // low is on
  digitalWrite(CO2_PWR, HIGH); // high is off
  digitalWrite(LED_BUILTIN,HIGH); //low is on 
  //digitalWrite(GSM_BOOT, LOW); //pullup to HIGH
  //digitalWrite(GSM_RESET, LOW); //pullup to HIGH
}

void GSM_ON(){
  digitalWrite(GSM_PWR, LOW); // low is on
  digitalWrite(CO2_PWR, HIGH); // high is off
  digitalWrite(LED_BUILTIN,LOW); //low is on 
  digitalWrite(GSM_BOOT, HIGH); //pullup to HIGH
  digitalWrite(GSM_RESET, HIGH); //pullup to HIGH
}


void RST_GSM(){ 
  digitalWrite(GSM_RESET, LOW); //pullup to HIGH
  delay(600);
  digitalWrite(GSM_RESET, HIGH); //pullup to HIGH
  delay(600);
}


void GSM(){
// turn on GSM
  int timer = millis();
  GSM_ON();
  bool connected = wait_connect();
  if (connected){
    Serial.println("Connected");
  } else {
    //GSM_INIT();
    RST_GSM();
    bool connected = wait_connect();
    Serial.println("Not connected");
  }
/// Do stuff
    //Serial1.println("AT+QPING=0,\"8.8.8.8\",4,1,32");
// POWEROFF
  GSM_OFF();
  Serial.print("On time = ");
  Serial.println( (millis()-timer)/1000 );
}


void loop(){

  if (n){
    GSM();
    n=false;
  };
  wait_response();
  delay(10000);
  GSM();

}
*/