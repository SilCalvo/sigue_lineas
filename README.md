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

Durante la realización de esta práctica, hemos ido avanzando a traves de los siguientes pasos:

Al comienzo de esta praćtica, erimero decidimos como ibamos a diseñar el codigo. No nos costó mucho ponernos de acuerdo. Decidimos usar FreeRTos con el fin de aprovechar el código a traves de tareas y prioridades, ya que iba a ser mucho más reactivo y más eficiente. Además, en clase de teoria se habia hablado muy bien de este algoritmo, por lo que lo vimos muy claro a la hora de elegirlo. Antes de comenzar a desarrollar los codigos principales que usariamos en la práctica, primero optamos por probar cada sensor/componente con el fin de comprobar que todo estuviera en perfectas condiciones y aprender como iban algunos componentes. Debido a que algunos ya los habiamos usado en la practica anterior, no nos costó mucho comprobarlos. El resto, aunque con alguna dificultad de entre medias, conseguimos probarlo también. 

Una vez que tuvimos todas estas pruebas, empezamos con el codigo principal, el del sigue lineas. Tras plantear las diferentes funcionalidades y tareas, empezamos a desarrollar el codigo. Pero tras una serie de problemas, decidimos cambiar de ocupación y empezar a plantear tambien la prarte de lpos mensajes. Mientras avanzabamos en una parte, la otra se quedaba bloqueda por alguna duda que luego se preguntaba al profesor en clase. Estuvimos alteranado asi hasta que conseguimos la priemra versión del codigo. En la que implementamos completamente el sigue lineas. Tras ir al laboratorio de robotica y probrar el circuito montado alli, nos dimos cuenta de que el sigue lineas ya lo teniamos hehco.

Por lo tanto, nuestro siguiente objetivo fue implementar bien los mensajes y mejorar la velocidad y eficiencia del coche. TRas un par de dias mas de desarollo, conseguimos implemnentar las dos y por fin, terminarlo. 


Eleccion de cosas:
Selecionar FreeRTos para usar las prioridades para el coche

Comenzamos comprobando los distintos sensores y actuadores. Creando un archivo donde se encuentran todos estos codigos para su uso posteriorimetne. Empezamos con los mmotores

tuvimos porblmas al conectar arduinmo al ordenador por el driver del puerto y tuvimos que seguir las instrucciones para desahbilitarlo. Y luego no dejaba cargarlo poque decia que no habia permisos en el puerto por loque tuvimso que escribir el siguiente comando : sudo chmod 666 /dev/ttyUSB0


cREAMOS QUE RTOS LLAMABA A LAS FUNCIONES Y LAS EJECUTABA DESDE EL BPRINCIPION POR ESO NO TENIAMOS UN  while, luego supimos que continuaba la ejecucio por donde iba, no empeza de nuevo su ejecucion

No nos iba cargar datos en la esp32 y tuvuimos que instalar y descargar python un a version concreta de pserial, porque aunque tuvueisemos ya instaladi una, no valia esa version.
a la hora de comunicar el arduino con el serie, tuvimios problemas porque no se conectaban, pero se solucionaron con condiconales.


Solucionamos los whiles de la ultma vez, delays (comprendidoes y entendidos), arreglamos la variables globales, primera version del digue linseas que iba perfectaente bien pero muy lento. en ningun momento pierde la linea. si va muy lento si la pierde pero la pierde por un lado pero recapacita. pero subiendo un poco 
la velocidad, si lo hacia todo perdecto, y si la subiamos muchp, se salia de la linea de frenyte y no paraba.

Version 2: hemos puessto mas mensajes, incluidos algunos opcionales, porque hay 2 de perder linea. No vamos a implemntar el alrgoritmo de q si el coche se va a tomar por culo. Has cambiado los leds.


version 2.1: probando ahora el cambio de distribución de los mensajes del mensaje end.

## 3. Dificultades
## 4. Video
## Bloopers y fails :)


