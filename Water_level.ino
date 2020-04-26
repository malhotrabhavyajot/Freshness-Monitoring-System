int flag;
void setup() {
  Serial.begin(9600);//Begin Serial Monitor
  pinMode(4,INPUT_PULLUP);//Pull Up pin 2 and set it as Input
  pinMode(5,INPUT_PULLUP);//Pull Up pin 3 and set it as Input
  pinMode(3,OUTPUT);
  digitalWrite(3,HIGH);
}

void loop() {
  if(digitalRead(5)==LOW && digitalRead(4)==LOW)
  {
    digitalWrite(3,LOW);
  }
  if(digitalRead(5)==HIGH && digitalRead(4)==HIGH)
  {
    digitalWrite(3,HIGH);
  }
}
