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

Durante el desarrollo de esta práctica, seguimos una metodología estructurada, avanzando paso a paso para alcanzar los objetivos planteados. Estos fueron los principales pasos que seguimos:

1. **Diseño inicial del código**  
   Desde el comienzo de la práctica, nos enfocamos en diseñar el código de manera eficiente. La decisión de utilizar FreeRTOS fue unánime, ya que queríamos aprovechar las ventajas que ofrece en términos de gestión de tareas y prioridades. Este sistema operativo nos permitiría desarrollar un código más reactivo y eficiente, capaz de manejar múltiples procesos de forma simultánea. Además, en las clases teóricas se había destacado la utilidad de FreeRTOS, lo que reforzó nuestra decisión. Con esta base conceptual clara, procedimos a estructurar el desarrollo.

2. **Pruebas iniciales de componentes**  
   Antes de abordar el desarrollo del código principal, realizamos pruebas exhaustivas de todos los sensores y componentes del robot. El objetivo era asegurarnos de que todo funcionara correctamente y, al mismo tiempo, familiarizarnos con su funcionamiento. Algunos componentes, como los motores y sensores infrarrojos, ya los habíamos utilizado en prácticas anteriores, por lo que estas pruebas fueron rápidas. Sin embargo, otros componentes, como el sensor de ultrasonidos, nos presentaron ciertos desafíos iniciales. A pesar de estas dificultades, logramos que todos los elementos funcionaran adecuadamente.

3. **Desarrollo del código principal: el seguidor de líneas**  
   Con las pruebas completadas, nos centramos en el desarrollo del código principal, comenzando con el algoritmo del seguidor de líneas. Planteamos las diferentes funcionalidades necesarias y asignamos prioridades a las tareas según su importancia. Sin embargo, durante este proceso nos encontramos con varios problemas técnicos que retrasaron el avance. Ante esto, decidimos alternar el trabajo entre distintas partes del proyecto. Por ejemplo, mientras resolvíamos dudas sobre el seguidor de líneas, avanzábamos en la implementación de la comunicación mediante mensajes. Esta estrategia nos permitió progresar de forma continua, aunque de manera no lineal.

4. **Primera versión funcional**  
   Tras resolver las dudas y ajustar el código, conseguimos desarrollar una primera versión completa del algoritmo seguidor de líneas. Esta versión fue probada en el laboratorio de robótica, donde montamos el circuito para realizar pruebas en condiciones reales. Los resultados fueron positivos, y confirmamos que el seguidor de líneas funcionaba correctamente, guiando al robot de manera estable a lo largo del trayecto.

5. **Optimización y finalización**  
   Una vez validado el seguidor de líneas, nos enfocamos en mejorar la velocidad y eficiencia del coche. Paralelamente, trabajamos en la implementación completa de la comunicación mediante mensajes MQTT, asegurándonos de que el ESP32 transmitiera correctamente la información recibida desde el Arduino Uno. Después de varios días de ajustes y pruebas adicionales, logramos implementar ambas mejoras con éxito.  

Finalmente, alcanzamos todos los objetivos de la práctica: el robot no solo seguía la línea y detectaba obstáculos, sino que también transmitía información en tiempo real a través de IoT. Este enfoque metódico y flexible fue clave para superar los desafíos y completar el proyecto con éxito.

## 3. Dificultades

---
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


## 4. Video

Click on the link to see it! -> [FOLLOW LINE]() :)

## Bloopers y fails :)


