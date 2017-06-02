
int ldrIn1 = A0, ldrIn2 = A1, r = A3; // LDR detector pins (Laser pins)
int relayPins[] = {10, 11, 12, 13}, relayStates[] = {0, 0, 0, 0}, noOfRelays = 4;
void setup() {
  // INPUT mode for LDR since we are "detecting"
  for (int i = 0; i < noOfRelays; i++)
  {
    pinMode(relayPins[i], OUTPUT);
  }
  pinMode(ldrIn1, INPUT);
  pinMode(ldrIn2, INPUT);
  pinMode(r, INPUT); // For randomSeed
  Serial.begin(9600); // DEBUGGING
}
double x = 0.0, y = 0.0, inCount = 0.0, outCount = 0.0, capacity = 8.0, count = 0; 
int devices = 0, devices1 = 0, thresh = 1000, flag = 0, anomaly = 0, change = 0, m;
// LDR1 value, LDR2 value, folks coming in, folks going out, uncertain condtions in the system, room capacity(NOT USED YET)
void loop() {
  // put your main code here, to run repeatedly:
  x = analogRead(ldrIn1); // Read value of LDR1
  delay(500);
  y = analogRead(ldrIn2); // Read value of LDR2
  delay(500);
  Serial.println(x); // DEBUGGING
  Serial.println(y); // DEBUGGING

  // thresh is the empirical threshold, may vary
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
        Serial.println(++inCount);
        flag = 1;
      }
    }
  }
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
        Serial.println(++outCount);
        flag = 0;
      }
    }
  }
  else if ((x < thresh) && (y < thresh))
  {
    anomaly++; // Someone is screwing around.
  }
  Serial.println("--");

  // No of devices connected, devices to switch on
  
  double p = 0.0;

  if (inCount == outCount)
  {
    for (int i = 0; i < noOfRelays; i++)
    {
      if (i == 0)
        digitalWrite(relayPins[i], HIGH);
      else
        digitalWrite(relayPins[i], LOW);
    }  
  }
  else if (inCount > outCount)
  {
    p = (inCount - outCount) / capacity;
    Serial.print("p = ");
    Serial.println(p);

    devices1 = noOfRelays * p;
    change = 0;
    // DEBUGGING
    //Serial.println(devices1 - devices);
    //delay(1000);
    if ((devices1 - devices) != 0)
    {
      Serial.println("si!!!!!");
      change = 1;
      //Serial.print("Devices = ");
      //Serial.println(devices);
    }  
    devices = noOfRelays * p;
    Serial.print("Devices = ");
    Serial.println(devices);
    // DEBUGGING
    int j = 0;  
    if (flag) j = HIGH;
    else j = LOW;
    Serial.print("j = ");
    Serial.println(j);
    randomSeed(analogRead(r));
    if (change)
    {
      
      if (j)
        m = devices - count;
      else
        m = count - devices;
      Serial.print("m = ");
      Serial.println(m);    
        
      for (int i = 0, z; i < m; i++)
      {

        do
        {
          z = random() % noOfRelays;
          Serial.print("z = ");
          Serial.println(z);
          delay(1000);
        }
        while (relayStates[z] == j);  
        relayStates[z] = j;
        if (z == 0)
        {
          if (j)
            digitalWrite(relayPins[z], LOW);
          else
            digitalWrite(relayPins[z], HIGH);
        } 
        else
          digitalWrite(relayPins[z], j);
        if (j)
          count++;
        else
          count--;  
        if (count > noOfRelays) break;
        Serial.println(count);  
        
      }
    }
    
  }
  else
    anomaly++;

  Serial.println(anomaly);  
}
