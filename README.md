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

Optamos por usar **FreeRTOS** porque nos parecía la solución más adecuada para gestionar de manera eficiente las prioridades entre las diferentes tareas del robot. Este sistema operativo nos permite dividir la funcionalidad del robot en tareas independientes con niveles de prioridad, garantizando una ejecución fluida y una adecuada respuesta a eventos críticos. En nuestro diseño, hemos distribuido las funcionalidades en cuatro tareas principales, organizadas según su importancia y frecuencia de ejecución:

### **Tareas y sus prioridades**  
1. **Prioridad 0 (la más baja): Control de LEDs**  
   Esta tarea gestiona el LED RGB integrado en el Arduino Uno. Su función principal es indicar el estado del robot en tiempo real:  
   - **Verde:** Cuando el robot detecta correctamente la línea.  
   - **Rojo:** Cuando el robot pierde la línea.  
   Dado que no es una funcionalidad crítica para el funcionamiento principal del robot, se le asigna la prioridad más baja.

2. **Prioridad 2: Motores y detección de línea**  
   En esta tarea, el robot utiliza los sensores infrarrojos para detectar la línea negra del circuito y controla los motores en consecuencia. La lógica aquí permite que el robot tome decisiones rápidas sobre su movimiento, como avanzar, girar o corregir la dirección. Esta funcionalidad es esencial para que el robot siga la línea de forma efectiva, por lo que se le otorga una prioridad media.

3. **Prioridad 3: Envío de mensajes**  
   Esta tarea es responsable de la comunicación serie entre el Arduino Uno y la ESP32. Cada 4 segundos, el Arduino envía un *ping* a la ESP32, que a su vez publica esta información en un servidor MQTT. Para garantizar la precisión temporal de los mensajes, esta tarea tiene una prioridad más alta que la de los motores.

4. **Prioridad 4 (la más alta): Detección de obstáculos**  
   Esta es la tarea crítica del sistema, ya que implica la detección de obstáculos mediante el sensor de ultrasonidos. Si el sensor detecta un objeto a una distancia entre 5 y 8 cm, el robot detiene inmediatamente su movimiento para evitar colisiones y marca el fin de la vuelta. Esta tarea tiene la prioridad más alta para asegurar que la detección y respuesta sean inmediatas, minimizando cualquier riesgo de choque.

---

### **Configuración inicial del sistema**  
Antes de ejecutar el algoritmo, la ESP32 debe conectarse correctamente a una red Wi-Fi. Para ello:  
1. La ESP32 permanece en espera hasta establecer la conexión.  
2. Una vez conectada, se cambia el modo de *upload* a *CAM* para garantizar que la comunicación serie entre la ESP32 y el Arduino funcione correctamente.  
3. Posteriormente, se espera 5 segundos para confirmar la conexión, ya que no hay una forma visual de verificar este paso en tiempo real.  

---

### **Modos de movimiento del robot**  
El robot cuenta con cinco modos distintos para ajustar su dirección según los datos recibidos de los sensores infrarrojos:  
1. **Recto:** Todos los sensores detectan la línea.  
2. **Giro a la izquierda:** Solo los sensores del lado izquierdo detectan la línea.  
3. **Giro pronunciado a la izquierda:** Los sensores más cercanos al borde izquierdo detectan la línea.  
4. **Giro a la derecha:** Solo los sensores del lado derecho detectan la línea.  
5. **Giro pronunciado a la derecha:** Los sensores más cercanos al borde derecho detectan la línea.  

El umbral mínimo para detectar la línea se establece en un valor de **800**. Si el robot pierde la línea, recuerda su estado anterior para corregir la dirección y volver al trayecto. Si el robot pierde la línea mientras avanzaba recto, retrocede ligeramente para intentar recuperarla.

---

### **Comunicación serie y formato de mensajes**  
En cuanto a la comunicación serie entre el Arduino y la ESP32, los mensajes deben seguir un formato específico para garantizar una lectura correcta. En nuestro diseño, los mensajes se envían encapsulados entre corchetes (`[]`). Este formato asegura que los datos sean interpretados de manera precisa por ambos dispositivos.

---

Con esta estructura de tareas y configuraciones, logramos un sistema eficiente y robusto, donde el robot puede seguir la línea, evitar obstáculos y mantener una comunicación constante mediante IoT, cumpliendo con todos los objetivos de la práctica.


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

Durante la realización de esta práctica, sus majestades se encontraron con varios desafíos técnicos que lograron superar con esfuerzo y aprendizaje. Entre los problemas más destacados están los siguientes:

### **1. Problemas con la conexión entre Arduino, ESP32 y el ordenador**  
La conexión entre las placas y el ordenador presentó varios inconvenientes:  
- **Drivers no reconocidos:** Al conectar el Arduino al ordenador, este no era reconocido debido a que mi portátil tenía unos drivers activados que interferían con el reconocimiento del puerto USB. Después de consultar al profesor, me indicó que debía desactivar esos drivers. Tras hacerlo, el problema parecía resuelto, pero surgieron nuevos inconvenientes.  
- **Permisos del USB:** Aunque el Arduino ya era reconocido, cada vez que lo conectábamos aparecía un error de permisos en el puerto USB. Investigando en internet, encontramos el comando `sudo chmod 666 /dev/ttyUSB0`, que solucionaba temporalmente el problema al dar permisos de lectura y escritura al puerto. Sin embargo, esta solución tenía dos limitaciones:  
  1. Los permisos se perdían cada vez que desconectábamos el USB, por lo que había que ejecutar el comando de nuevo.  
  2. Si cambiábamos el puerto USB, el identificador del dispositivo también cambiaba, obligándonos a repetir el proceso.  
- **Datos no visibles en la terminal de la ESP32:** El último obstáculo relacionado con la conexión fue que no se cargaban correctamente los datos enviados desde la ESP32 en la terminal de Python. Para solucionarlo, tuvimos que instalar una versión más reciente de la librería `pyserial`, lo cual finalmente permitió que los datos se mostraran sin inconvenientes.

---

### **2. Dificultad para entender el funcionamiento de las tareas en FreeRTOS**  
Al principio, tuvimos problemas al implementar las tareas en FreeRTOS, ya que pensábamos que cada vez que se ejecutaba una tarea, esta empezaba desde el principio. Sin embargo, después de discutirlo con el profesor, nos explicó que las tareas no se reinician con cada ejecución, sino que retoman desde el punto donde se habían pausado. Además, aprendimos que si no incluíamos un bucle dentro de la tarea, esta solo se ejecutaba una vez y no volvía a ejecutarse, lo cual no era el comportamiento deseado en nuestro diseño. Este aprendizaje fue clave para estructurar correctamente nuestras tareas y hacer un uso eficiente de FreeRTOS.

---

### **3. Configuración de los *delays* en las tareas**  
Otro desafío importante fue ajustar los tiempos de los *delays* en las tareas. Inicialmente, los teníamos configurados con valores demasiado altos, lo que hacía que el robot reaccionara de manera lenta. Esto provocaba que se saliera frecuentemente de la línea antes de que pudiera corregir su trayectoria. Por otro lado, al reducir demasiado los *delays*, las tareas no tenían suficiente tiempo para ejecutarse completamente, causando conflictos y un funcionamiento errático.  

Finalmente, encontramos un equilibrio ajustando los tiempos de espera de cada tarea según su prioridad e importancia:  
- Las tareas críticas, como la detección de obstáculos, tenían *delays* muy bajos para garantizar una rápida reacción.  
- Las tareas menos prioritarias, como el control de LEDs, tenían tiempos de espera más altos para no interferir con el rendimiento de las tareas principales.  

---

Estos problemas nos permitieron aprender mucho sobre el funcionamiento de las placas, la configuración de FreeRTOS y la importancia de los tiempos en sistemas de tiempo real. Superarlos no solo mejoró nuestra práctica, sino también nuestra comprensión de los desafíos técnicos que pueden surgir en proyectos de este tipo.


## 4. Video

Click on the link to see it! -> [FOLLOW LINE]() :)

## Bloopers y fails :)


