int pompe_pin = 15;

void setup() {
  // put your setup code here, to run once:
  pinMode(pompe_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Reading");
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pompe_pin, HIGH);
}
