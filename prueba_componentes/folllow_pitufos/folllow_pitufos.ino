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

#define FORWARD 0
#define RIGHT 1
#define LEFT 2
#define RIGHT_X2 3
#define LEFT_X2 4
#define LOST 5

#define FAST 125 //125 //75
#define SLOW 25 //25 //15
#define VERY_LOW 15 //10


int current_line = LOST;
int last_line;
bool end_flag = false;
bool line_found = false;
long int count_loops = 0;
long int count_line = 0;

//TaskHandle_t messagesHandle = NULL; // Declarar handle global para la tarea messages


uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

// PREGUNTAR PARA QUE ES EL DELAY
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

static void dect_line (void* pvParameters) {

  while (1) {

    float left_sensor = analogRead(PIN_ITR20001_LEFT);
    float middle_sensor = analogRead(PIN_ITR20001_MIDDLE);
    float right_sensor = analogRead(PIN_ITR20001_RIGHT);

    

    if (left_sensor < 800 && middle_sensor > 800 && right_sensor > 800) {
      //Serial.println("{GIRA DERECHA}");
      current_line = RIGHT;
    } else if (left_sensor > 800 && middle_sensor > 800 && right_sensor < 800) {
      //Serial.println("{GIRAA IZQUIERDA}");
      current_line = LEFT;
    } else if (left_sensor < 800 && middle_sensor < 800 && right_sensor > 800) {
      //Serial.println("{GIRA MUCHO DERECHA}");
      current_line = RIGHT_X2;

    } else if (left_sensor > 800 && middle_sensor < 800 && right_sensor< 800) {
      //Serial.println("{GIRA MUCHO IZQUIERDA}");
      current_line = LEFT_X2;

    }else if ( middle_sensor > 800 ) {
      //Serial.println("{RECTO}");
      current_line = FORWARD;
    } else {
      current_line = LOST;

    }
     if (!end_flag) {
      count_loops++;

      if(current_line != LOST) {
        control_motors(current_line);
        count_line++;

        if(line_found){
          Serial.println("{line_found}");
          line_found = false;
        }

      } else {
        
        control_lost_line();
        line_found = true;
        Serial.println("{line_lost}");
      }
      
      
    } else {
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, 0);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, 0);

    }

    vTaskDelay(pdMS_TO_TICKS(50)); // Espera 100 ms
  
  }

}

static void messages (void* pvParameters) {
  while (1) {
   
    //vTaskDelay(pdMS_TO_TICKS(5000)); // 5 segundos

    if(!end_flag){
      Serial.println("{ping}");
    }
    
    vTaskDelay(pdMS_TO_TICKS(4900)); // 5 segundos
  }
}

static void dect_obs (void* pvParameters) {
  while (1) {
    long t; //timepo que demora en llegar el eco
    long d; //distancia en centimetros

    digitalWrite(TRIG_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1));
    //delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(TRIG_PIN, LOW);
    
    t = pulseIn(ECHO_PIN, HIGH); //obtenemos el ancho del pulso
    d = t/58;             //escalamos el tiempo a una distancia en cm
    
    if(d < 9 && !end_flag){ ///////////////////////////////////////////////////////////////////// DEPURAR AL MILIMETRO
      Serial.println("{end}");

      end_flag = true;
      //Serial.print("Distancia: ");
      Serial.println("{" +String(d)+ "}");

      double div = (double)count_line/(double)count_loops * 100;
      Serial.println("{" + String(div) + "}");
      
      
      /*if (messagesHandle != NULL) {
        vTaskDelete(messagesHandle);
        Serial.println("Messages task deleted.");
      }*/

    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

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
      analogWrite(PIN_Motor_PWMA, 15);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, FAST);

      last_line = control_var;
      break;

    case LEFT:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, FAST);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, 15);

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
void control_lost_line(){
 switch (last_line) {
    case FORWARD:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, LOW);
      analogWrite(PIN_Motor_PWMA, 50);
      digitalWrite(PIN_Motor_BIN_1, LOW);
      analogWrite(PIN_Motor_PWMB, 50);

      break;

    case RIGHT:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, 15);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, FAST);

      break;

    case LEFT:
      digitalWrite(PIN_Motor_STBY, HIGH);
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, FAST);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, 15);

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

  //EDCTLINES
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);

  //OBSTACLEH
  pinMode(TRIG_PIN, OUTPUT); //pin como salida
  pinMode(ECHO_PIN, INPUT);  //pin como entrada
  digitalWrite(TRIG_PIN, LOW);//Inicializamos el pin con 0


  // inicializar comunicaciones
  String sendBuff;

  // To make this code works, remember that the switch S1 should be set to "CAM"
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
 
  // put your setup code here, to run once:
  xTaskCreate(led, "Led", 50, NULL, 0, NULL); //////////////////////////////////NULL O VAR
  xTaskCreate(dect_line, "Dect_Line", 100, NULL, 2, NULL);
  xTaskCreate(messages, "Messages", 100, NULL, 3, NULL);
  xTaskCreate(dect_obs, "Dect_Obs", 100, NULL, 4, NULL);


  //MOTOREH 
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

  Serial.println("{init}");


}

void loop() {
  // put your main code here, to run repeatedly:


}
