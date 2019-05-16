#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;
#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8); // RX, TX  
// Connect HM10      Arduino Uno
//     TXD          Pin 7
//     RXD          Pin 8
int accValues[100];
int count=0;
char buf[4];
const char *msg;
void displaySensorDetails(void)
{
  sensor_t sensor;
  gyro.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
  mySerial.begin(9600);

}

void setup(void) 
{
  Serial.begin(9600);
  if (!driver.init())
     Serial.println("init failed");
  Serial.println("Gyroscope Test"); Serial.println("");
  
  /* Enable auto-ranging */
  gyro.enableAutoRange(true);
  
  /* Initialise the sensor */
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");

  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop(void) 
{
  /* Get a new sensor event */ 
 
  count=0;
  /* Display the results (speed is measured in rad/s) */
  for (int i=0;i<80;i++){ 
  sensors_event_t event; 
  gyro.getEvent(&event);
  Serial.print("X: "); Serial.print(event.gyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.gyro.z); Serial.print("  ");
  float tot=abs(event.gyro.x*10)+abs(event.gyro.y*10)+abs(event.gyro.z*10);
  Serial.print("Total: "); Serial.print(tot); Serial.println("  ");
  accValues[i]=tot;
  if (i>1) {
    if (accValues[i]>55&& accValues[i-1]<55&& accValues[i-2]<55&& accValues[i-3]<55) {
      count=count+1;
        const char *msg = itoa (count, buf, 10);
     Serial.println(msg);
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
      Serial.println("count");  
    }
  }
  delay(100);
  }
  Serial.print("Final count: "); Serial.print(count); Serial.println("  ");
    
    const char *msg = itoa (count, buf, 10);
     Serial.println(msg);
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(200);
     char c;
  if (Serial.available()) {
    c = Serial.read();
    mySerial.print(c);
  }
  if (mySerial.available()) {
    c = mySerial.read();
    Serial.print(c);    
  }
  delay(2000);
}



