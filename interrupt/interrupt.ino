const byte PIN_LED = 13;
const byte PIN_SET_INTERRUPT = 2;

volatile byte ledState = LOW;
volatile byte pinValue;

void setup()
{
  Serial.begin(9600);
  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_SET_INTERRUPT, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_SET_INTERRUPT), blink, HIGH);
}

void loop()
{
  digitalWrite(PIN_LED, ledState);

  pinValue = digitalRead(PIN_SET_INTERRUPT);

  Serial.print(pinValue);
  Serial.print(" : ");
  Serial.println(ledState);

  delay(100);
}

void blink()
{
  ledState = !ledState;
}
