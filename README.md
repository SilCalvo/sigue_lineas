# Práctica 4: Follow line by 13_pitufos

https://silcalvo.github.io/sigue_lineas/

Sistemas Empotrados y de Tiempo real - Diciembre 2024

Silvia calvo e Irene Diez

## 0. Introducción

En esta práctica, el objetivo principal es desarrollar un robot seguidor de líneas que pueda alcanzar la meta de manera eficiente y rápida. La meta está delimitada por una caja que actúa como obstáculo. Durante el recorrido, el robot debe mantener comunicación en tiempo real a través de una conexión IoT utilizando el protocolo MQTT sobre Wi-Fi. Esta comunicación es gestionada por una placa ESP32 que publica información en un *topic* 
basado en los datos recibidos mediante la comunicación serie desde una placa Arduino Uno.  El diseño permite cierta flexibilidad: el robot puede perder la línea momentáneamente, siempre y cuando sea capaz de recuperarla y continuar su camino.
Además, se debe implementar un algoritmo para detectar obstáculos de manera precisa y tomar decisiones que eviten colisiones.

Para cumplir con estos objetivos, el robot incorpora los siguientes componentes:

- **Arduino Uno**: Esta placa centraliza el procesamiento de los datos provenientes de los sensores, controla el movimiento de los motores y establece la comunicación serie con la ESP32.
- **ESP32**: Responsable de la comunicación IoT mediante Wi-Fi. Esta placa recibe información desde el Arduino Uno y la publica en el servidor MQTT, permitiendo monitorear el comportamiento del robot en tiempo real.
- **Sensor de ultrasonidos**: Permite la detección de obstáculos a través de ondas sonoras. Estas ondas rebotan en los objetos y, al medir el tiempo que tardan en regresar al sensor, se calcula la distancia al obstáculo.
- **Sensor infrarrojo (IR)**: Actúa como detector de líneas. Es capaz de diferenciar el color del suelo, distinguiendo entre superficies negras (línea) y no negras (fondo), lo que guía al robot en su trayecto.
- **Motores**: Controlan las ruedas y permiten que el robot se desplace en función de las órdenes del Arduino Uno, adaptándose a las curvas y obstáculos del trayecto.
- **LED RGB**: Incorporado en el Arduino Uno, sirve como indicador visual del estado del robot. Se enciende en rojo cuando el robot está fuera de la línea y en verde cuando está correctamente alineado con ella.

Este diseño integra hardware y software de manera eficiente, combinando control de sensores, algoritmos de navegación y comunicación en red. La implementación exitosa de esta práctica no solo requiere lograr que el robot siga
la línea y evite obstáculos, sino también garantizar una comunicación confiable y un comportamiento robusto en tiempo real.

## 1. Nuestra implementación y detalles
## 2. Proceso
## 3. Dificultades
## 4. Video
## Bloopers y fails :)


