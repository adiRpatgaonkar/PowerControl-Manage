int ldrIn = A0, ldr2 = A1;
void setup() {
  // put your setup code here, to run once:
  pinMode(ldrIn_1, INPUT);
  pinMode(ldrIn_2, INPUT);
  Serial.begin(9600);
}
int x = 0, count = 0, thresh = 60;
void loop() {
  // put your main code here, to run repeatedly:
  x = analogRead(ldrIn1);
  delay(500);
  //Serial.println(x);

  if (x < 700)
  {
    unsigned int t = millis(); // Timer starts for the incoming person
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
      if (x < 700)
        Serial.println(++count);
    }
  }
  Serial.println("--");



}
