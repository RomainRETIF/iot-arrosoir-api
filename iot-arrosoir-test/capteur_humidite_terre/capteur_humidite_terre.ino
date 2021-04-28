int sensor_dirt_humidity_pin = 34;
int ledPin = 14;
int dirt_humidity;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Reading");
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  dirt_humidity = analogRead(sensor_dirt_humidity_pin);
  dirt_humidity= map(dirt_humidity,550,0,0,100);
  if(dirt_humidity > 50){
      digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
  Serial.print("Moisture : ");
  Serial.print(dirt_humidity);
  Serial.println("%");
  delay(1000);
}
