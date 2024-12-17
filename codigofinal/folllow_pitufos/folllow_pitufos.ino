#include <Arduino_FreeRTOS.h>

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

// Follow line states
#define FORWARD 0
#define RIGHT 1
#define LEFT 2
#define RIGHT_X2 3
#define LEFT_X2 4
#define LOST 5

#define FAST 125 //175 //125 //75
#define SLOW 25 //35 //25 //15
#define VERY_LOW  15 //21//15 //10

#define LINE_UMBRAL 800

// Follow line state
int current_line = LOST;
int last_line;
// State flags
bool end_flag = false;
bool line_found = false;
bool line_lost_flag = false;
bool flanco_end = false;
// Percentage line variables
long int count_loops = 0;
long int count_line = 0;


uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

// Control of the RGB led
static void led (void* pvParameters) {
 int r = 255,g = 255,b =255;


  while(1) {

    if(current_line != LOST){
      r=0;
      g=255;
      b=0;
    } else {
      r=255;
      g=0;
      b=0;
    }

    FastLED.showColor(Color(r, g, b));
      
    vTaskDelay(pdMS_TO_TICKS(300)); // Espera 100 ms
  }
  
}

// Control of the states and actions of the follow line process
static void dect_line (void* pvParameters) {

  while (1) {

    float left_sensor = analogRead(PIN_ITR20001_LEFT);
    float middle_sensor = analogRead(PIN_ITR20001_MIDDLE);
    float right_sensor = analogRead(PIN_ITR20001_RIGHT);


    if (left_sensor < LINE_UMBRAL && middle_sensor > LINE_UMBRAL && right_sensor > LINE_UMBRAL) {
      //Serial.println("{GIRA DERECHA}");
      current_line = RIGHT;
    } else if (left_sensor > LINE_UMBRAL && middle_sensor > LINE_UMBRAL && right_sensor < LINE_UMBRAL) {
      //Serial.println("{GIRAA IZQUIERDA}");
      current_line = LEFT;
    } else if (left_sensor < LINE_UMBRAL && middle_sensor < LINE_UMBRAL && right_sensor > LINE_UMBRAL) {
      //Serial.println("{GIRA MUCHO DERECHA}");
      current_line = RIGHT_X2;

    } else if (left_sensor > LINE_UMBRAL && middle_sensor < LINE_UMBRAL && right_sensor< LINE_UMBRAL) {
      //Serial.println("{GIRA MUCHO IZQUIERDA}");
      current_line = LEFT_X2;

    }else if ( middle_sensor > LINE_UMBRAL ) {
      //Serial.println("{RECTO}");
      current_line = FORWARD;
    } else {
      current_line = LOST;

    }
    // Control the motors in based of the state
    if (!end_flag) {
      count_loops++;

      if(current_line != LOST) {
        control_motors(current_line);
        count_line++;
        line_lost_flag = true;

        if(line_found){
          Serial.println("{line_found}");
          line_found = false;
        }

      } else {
        
        control_lost_line();
        line_found = true;
        if (line_lost_flag){
          Serial.println("{line_lost}");
          line_lost_flag = false;
        }
        
      }
      
      
    } else {
      // Stop when finnish
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, 0);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, 0);

    }

    vTaskDelay(pdMS_TO_TICKS(30)); // Espera 10 ms
  
  }

}

static void messages (void* pvParameters) {
  while (1) {
   
    if(!end_flag){
      Serial.println("{ping}");
    }
    
    vTaskDelay(pdMS_TO_TICKS(3990)); // 5 segundos
  }
}

static void dect_obs (void* pvParameters) {
  while (1) {
    long t; //time echo
    long d; //distance in cm

    digitalWrite(TRIG_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1));
    
    digitalWrite(TRIG_PIN, LOW);
    
    t = pulseIn(ECHO_PIN, HIGH); 
    d = t/58;           

   if (end_flag && !flanco_end) { // In order to get the correct distance

      //Serial.print("Distancia: ");
      Serial.println("{" +String(d)+ "}");

      double div = (double)count_line/(double)count_loops * 100;
      Serial.println("{" + String(div) + "}");
      flanco_end = true;
    }

    // Stop when obstacle detected
    if(d < 12 && !end_flag){ 
      Serial.println("{end}");

      end_flag = true;

    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// When line is detected: 
void control_motors(int control_var) {
  switch (control_var) {
    case FORWARD:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, FAST);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, FAST);

      last_line = control_var;
      break;

    case RIGHT:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, VERY_LOW);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, FAST);

      last_line = control_var;
      break;

    case LEFT:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, FAST);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, VERY_LOW);

      last_line = control_var;
      break;

    case RIGHT_X2:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, VERY_LOW);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, FAST);

      last_line = control_var;
      break;

    case LEFT_X2:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, FAST);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, VERY_LOW);

      last_line = control_var;
      break;
  }
}

//In case line lost
void control_lost_line(){
 switch (last_line) {
    case FORWARD: // Go backwards
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, LOW);
      analogWrite(PIN_Motor_PWMA, 50);
      digitalWrite(PIN_Motor_BIN_1, LOW);
      analogWrite(PIN_Motor_PWMB, 50);

      break;

    case RIGHT:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, VERY_LOW);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, FAST);

      break;

    case LEFT:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, FAST);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, VERY_LOW);

      break;

    case RIGHT_X2:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, VERY_LOW);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, FAST);

      break;

    case LEFT_X2:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, FAST);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, VERY_LOW);

      break;
  }

}

void setup() {
  //leds
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);

  //DECTLINES
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);

  //OBSTACLES
  pinMode(TRIG_PIN, OUTPUT); //pin como salida
  pinMode(ECHO_PIN, INPUT);  //pin como entrada
  digitalWrite(TRIG_PIN, LOW);//Inicializamos el pin con 0

  // Wait for ESP32 connected to wifi
  String sendBuff;
  while(1) {

    if (Serial.available()) {

      char c = Serial.read();
      sendBuff += c;
    
      if (c == '}')  {            
        Serial.print("Received data in serial port from ESP32: ");
        Serial.println(sendBuff);

        sendBuff = "";
        break;
      } 

    } 

  }
 
  /// FREETROS Tasks 
  xTaskCreate(led, "Led", 50, NULL, 0, NULL); 
  xTaskCreate(dect_line, "Dect_Line", 100, NULL, 2, NULL);
  xTaskCreate(messages, "Messages", 100, NULL, 3, NULL);
  xTaskCreate(dect_obs, "Dect_Obs", 100, NULL, 4, NULL);


  //MOTORS
  pinMode(PIN_Motor_STBY, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);

  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, 0);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, 0);

  // Start circuit
  Serial.println("{init}");


}

void loop() {
  // put your main code here, to run repeatedly:


}
