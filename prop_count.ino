#include <time.h>
int ldrIn1 = A0, ldrIn2 = A1; // LDR detector pins (Laser pins)
void setup() {
  // INPUT mode for LDR since we are "detecting"
  pinMode(ldrIn1, INPUT);
  pinMode(ldrIn2, INPUT);
  //Serial.begin(9600); // DEBUGGING
}
int x = 0, y, inCount = 0, outCount = 0, anomaly = 0, capacity = 60, flag = 0;
// LDR1 value, LDR2 value, folks coming in, folks going out, uncertain condtions in the system, room capacity(NOT USED YET)
void loop() {
  // put your main code here, to run repeatedly:
  x = analogRead(ldrIn1); // Read value of LDR1
  delay(500);
  y = analogRead(ldrIn2); // Read value of LDR2
  delay(500);
  //Serial.println(x); // DEBUGGING
  //Serial.println(y); // DEBUGGING

  // 700 is the empirical threshold, may vary
  if (x < 700 && y >= 700)
  { // If LDR1 detects a signal befote LDR2 i.e someone is coming in.
    unsigned int t = millis(); // Timer starts for the incoming person
    // Only for DEBUGGING
    // Serial.println(t);
    // Serial.println("Wait");
    // int y = (millis() - t);
    // Serial.println(y);
    while (((unsigned int)millis() - t) < 3000) // Timer is of 3 seconds
    {
      // If there is a trigger for a 2nd time under 3 seconds, someone has entered.
      y = analogRead(ldrIn2);
      delay(1000);
      if (y < 700) // Increment the count if there is a signal at the LDR2 indeed!
      {
        Serial.println(++inCount);
        flag = 1;
      }
    }
  }
  else if (y < 700 && x >= 700)
  { // If LDR2 detects a signal befote LDR1 i.e someone is going out.
    unsigned int t = millis(); // Timer starts for the outgoing person
    // Only for DEBUGGING
    // Serial.println(t);
    // Serial.println("Wait");
    // int y = (millis() - t);
    // Serial.println(y);
    while (((unsigned int)millis() - t) < 3000) // Timer is of 3 seconds
    {
      // If there is a trigger for a 2nd time under 3 seconds, someone has entered.
      x = analogRead(ldrIn2);
      delay(1000);
      if (x < 700) // Increment the count if there is a signal at the LDR1 indeed!
      {
        Serial.println(++outCount);
        flag = 0;
      }
    }
  }
  else
  {
    anomaly++; // Someone is screwing around.
  }
  Serial.println("--");

  // No of devices connected, devices to switch on
  int noOfRelays = 8, devices = 0;
  int relayPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
  double p = 0.0;

  if (inCount == outCount)
  {
    for (int i = 0, z; i < noOfRelays; i++)
      digitalWrite(relayPins[z], LOW);
  }
  else if (inCount > outCount)
  {
    p = (inCount - outCount)/ inCount;
    devices = noOfRelays * p;
    srand(time(NULL));
    int j = 0;

    if (flag) j = HIGH;
    else j = LOW;

    for (int i = 0, z; i < devices; i++)
    {
      z = rand() % 8;
      digitalWrite(relayPins[z], j);
    }
  }
  else
    anomaly++;
}
