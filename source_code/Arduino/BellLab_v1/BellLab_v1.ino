unsigned long t_old = 0;

int d_raw = 0;
int d_val = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("\nBell Lab Start..");

  analogReadResolution(12);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);

  d_raw = analogRead(A0);
  d_val = map(d_raw, 0, 4095, 0, 3300);
  plotVolt();
  
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);

  while ((micros() - t_old) < 1000000);
  t_old = micros(); 
}

void plotRaw() {
  Serial.println(d_raw);
}

void plotVolt() {
  Serial.println((float)d_val / 1000);
}

void serialTest() {
  Serial.print(d_raw);
  Serial.print(" ");
  Serial.print(d_val);
  Serial.println();
}

