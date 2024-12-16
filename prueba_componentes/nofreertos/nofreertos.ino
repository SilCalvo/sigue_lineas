//LEDS
#include "FastLED.h"
#define PIN_RBGLED 4
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

//DECT LINES
#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0

//ultrasonidoh
#define TRIG_PIN 13  
#define ECHO_PIN 12  

//motores
#define PIN_Motor_STBY 3

// Group A Motors (Right Side)
// PIN_Motor_AIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_AIN_1 7
// PIN_Motor_PWMA: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMA 5

// Group B Motors (Left Side)
// PIN_Motor_BIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_BIN_1 8
// PIN_Motor_PWMB: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMB 6

static int var = 0;
int last_line[3] = {0,0,0};

void DECT_OBS () {
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(TRIG_PIN, LOW);
  
  t = pulseIn(ECHO_PIN, HIGH); //obtenemos el ancho del pulso
  d = t/58;             //escalamos el tiempo a una distancia en cm
  
  Serial.print("Distancia: ");
  Serial.println(d);      //Enviamos serialmente el valor de la distancia
}


void DECT_LINE () {
  float LEFT = analogRead(PIN_ITR20001_LEFT);
  float MIDDLE = analogRead(PIN_ITR20001_MIDDLE);
  float RIGHT = analogRead(PIN_ITR20001_RIGHT);

  /*if (LEFT > 800 && MIDDLE > 800 && RIGHT > 800) {
    Serial.println("RECTO");
    last_line = {1, 1, 1};

  } else if (LEFT < 800 && MIDDLE > 800 && RIGHT > 800) {
    Serial.println("GIRA DERECHA");
    last_line = {0, 1, 1};
  } else if (LEFT > 800 && MIDDLE > 800 && RIGHT < 800) {
    Serial.println("GIRAA IZQUIERDA");
    last_line = {1, 1, 0};
  } else if (LEFT < 800 && MIDDLE < 800 && RIGHT > 800) {
    Serial.println("GIRA MUCHO DERECHA");
    last_line = {0, 0, 1};

  } else if (LEFT > 800 && MIDDLE < 800 && RIGHT < 800) {
    Serial.println("GIRA MUCHO IZQUIERDA");
    last_line = {1, 0, 0};
  } else {
    Serial.println("TAS PERDIÓ");
  }

  return last_line;*/

}



uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

// PREGUNTAR PARA QUE ES EL DELAY
void LED () {
  int r = 255,g = 255,b =255;
  Serial.println(var);

  if (var % 3 == 1) {
    r=255;
    g=0;
    b=0;
  } else if (var % 3 == 2) {
    r=0;
    g=255;
    b=0;
  } else if (var % 3 == 0) {
    r=0;
    g=0;
    b=255;
  }

  FastLED.showColor(Color(r, g, b));
    
  Serial.println("leds");

  var++;

  
}

void setup() {
  // put your setup code here, to run once:

  //leds
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);

  //EDCTLINES
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);

  //OBSTACLEH
  pinMode(TRIG_PIN, OUTPUT); //pin como salida
  pinMode(ECHO_PIN, INPUT);  //pin como entrada
  digitalWrite(TRIG_PIN, LOW);//Inicializamos el pin con 0

  //MOTOREH 
  pinMode(PIN_Motor_STBY, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);

  // APAGAMOH MOTOREH
  digitalWrite(PIN_Motor_STBY, LOW);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, 0);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, 0);

}

void loop() {
  // put your main code here, to run repeatedly:
  DECT_OBS();
  DECT_LINE();
  LED();

  delay(1000);

}
