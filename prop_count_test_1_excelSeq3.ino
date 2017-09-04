/*
#include<.h>
long          idx = 0; 
char          value[16];              // written or read value
rExcel        myExcel;                // class for Excel data exchanging
*/

int ldrIn1 = A0, ldrIn2 = A1; // LDR detector pins (Laser pins)
int noOfRelays = 3, relayPins[] = {2, 3, 4}, relayStates[] = {0, 0, 0};

void setup() {
  // INPUT mode for LDR since we are "detecting"
  for (int i = 0; i < noOfRelays; i++)
    pinMode(relayPins[i], OUTPUT);
  pinMode(ldrIn1, INPUT);
  pinMode(ldrIn2, INPUT);
  
  Serial.begin(9600); // DEBUGGING

  //myExcel.clearInput();
}
double x = 0.0, y = 0.0, inCount = 0.0, inCtemp = 0.0, outCtemp = 0.0, outCount = 0.0, capacity = 6.0; 
int thresh = 900, anomaly = 0, interval = 2, devices = 0, people = 0, flag = 1;
// LDR1 value, LDR2 value, folks coming in, folks going out, uncertain condtions in the system, room capacity
void loop() {
  // put your main code here, to run repeatedly:
  x = analogRead(ldrIn1); // Read value of LDR1
  delay(500);
  y = analogRead(ldrIn2); // Read value of LDR2
  delay(500);
  Serial.println(x); // DEBUGGING
  Serial.println(y); // DEBUGGING

  // thresh is the empirical threshold, may vary
  
  //CALCULATE INCOMING PEOPLE
  
  if (x < thresh && y >= thresh)
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
      if (y < thresh) // Increment the count if there is a signal at the LDR2 indeed!
      {
        Serial.print("In : ");
        Serial.println(++inCount);
        flag = 1;
      }
    }
  }
  //CALCULATE OUTGOING PEOPLE
  else if (y < thresh && x >= thresh)
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
      x = analogRead(ldrIn1);
      delay(1000);
      if (x < thresh) // Increment the count if there is a signal at the LDR1 indeed!
      {
        Serial.print("Out : ");
        Serial.println(++outCount);
        flag = -1;
      }
    }
  }
  else if ((x < thresh) && (y < thresh))
  {
    anomaly++; // Someone is screwing around.
  }
  
  Serial.println("--");
  
  //PERFORM CALCULATIONS ON inCount and outCount
  people = inCount - outCount; // No. of people currently in the room
  Serial.print("Truth: "); Serial.println(people);

  if (people == 0)
  {
    devices = 0;
    inCount = 0;
    outCount = 0;
    for (int i = 0; i < noOfRelays; i++)
      digitalWrite(relayPins[i], HIGH); // Active LOW
  }
  else if (people > 0)
  {
    if (people > capacity)
    {
      for (int i = 0; i < noOfRelays; i++)
        digitalWrite(relayPins[i], LOW); // Active LOW
      devices = noOfRelays;
      Serial.println("Excess"); 
    }
    else if (people <= capacity)
    {
      if ((people % interval == 0) && (flag == 1))
      {
        Serial.print("devices : ");
        Serial.println(devices);
        digitalWrite(relayPins[devices++], LOW); // Acitve LOW
        flag = 0;
        Serial.println("(Y)");
      }
      else if ((people % interval == (interval - 1)) && (flag == -1))
      {
        digitalWrite(relayPins[--devices], HIGH); // Acitve LOW
        Serial.print("devices : ");
        Serial.println(devices);
        flag = 0;
        Serial.println("(W)");
      }
    }
    
  }
  else if (people < 0)
  {
    inCount = 0;
    outCount = 0;
    devices = 0;
    people = 0;
    anomaly++;
    for (int i = 0; i < noOfRelays; i++)
      digitalWrite(relayPins[i], HIGH); // Active LOW
    Serial.println(":'D");  
  }
  
  
  double Power = devices * 12.0; // Calculate the power
  
  //myExcel.writeIndexed("Example", idx+11, 1,"%date%");    // write %date% (that will be converted in current date) to worksheet 'Example' row 'idx+11' column '1' 
  //myExcel.writeIndexed("Example", idx+11, 2,"%time%");    // write %time% (that will be converted in current time) to worksheet 'Example' row 'idx+11' column '2' 
  //myExcel.writeIndexed("Example", idx+11, 3, devices, 2);      // write the value from A0 pin to worksheet 'Example' row 'idx+11' column '4' with two digits as decimals
  //myExcel.writeIndexed("Example", idx+11, 4, people, 2); 
  //myExcel.writeIndexed("Example", idx+11, 3, Power, 2); 
  
  //idx++;
  //myExcel.save(); 
}
