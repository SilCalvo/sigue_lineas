#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0

void setup() {
  Serial.begin(9600);//iniciailzamos la comunicación
  // put your setup code here, to run once:
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  float UNO = analogRead(PIN_ITR20001_LEFT);
  float DOS = analogRead(PIN_ITR20001_MIDDLE);
  float TRES = analogRead(PIN_ITR20001_RIGHT);

  Serial.println(UNO);
  Serial.println( DOS);
  //Serial.println( TRES);
//por encima de 800 es linea.
}
