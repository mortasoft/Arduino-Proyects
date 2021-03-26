/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Blynk.h>

//Token de autenticación de Blynk.cc
char auth[] = "21a257aed1dc4b229c6f2c6d6dffba72";

// Estos son los rangos para determinar si la planta esta seca o no
// Se pueden ajustar de acuerdo a las pruebas realizadas.
int humedadAlta = 550;
int humedadMedia = 700;

#define W5100_CS  10
#define SDCARD_CS 4

// Variable que almacena el valor de humedad del sensor.
// Va de 0 a 1000, donde 0 es completamente en agua, o 1000 en el aire.
int humedad = 0;
int estadoInt = 0;

BlynkTimer timer;
BlynkTimer timer2;

void setup() {
  // Inicializa la comunicación en el puerto 9600
  Serial.begin(9600);
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);
  Blynk.begin(auth);

  // Se va a llamar a esta funcion cada segundo para leer la humedad
  timer.setInterval(1000L, leerHumedad);
  // Se va a llamar a esta funcion cada dia x Milisegundos para enviar el correo
  timer2.setInterval(86400000L, enviarCorreo);

}

String obtenerHumedad() {

  String estado = "";
  humedad = analogRead(A0);

  // Significa que la planta esta suficientemente humeda.
  if (humedad <= humedadAlta) {
    estado += "La planta esta feliz, la tierra esta mojada";
    estadoInt = 0;
  }

  // Significa que la tierra esta en buenas condiciones
  if (humedad > humedadAlta && humedad < humedadMedia) {
    estado += "La tierra esta en buenas condiciones";
    estadoInt = 1;
  }

  // Significa que la tierra esta seca
  if (humedad >= humedadMedia) {
    estado += "La tierra esta seca hay que regar la planta";
    estadoInt = 2;
  }

  return estado;

}

// Funcion que se llama cada segundo
void leerHumedad()
{
  String valor = obtenerHumedad();
  int humedad = analogRead(A0);

  // Guarda los valores del estado y la humedad en los pines virtuales V1 y V2
  Blynk.virtualWrite(V1, humedad);
  Blynk.virtualWrite(V2, valor);
  Blynk.virtualWrite(V3, estadoInt);
  Serial.println(valor + humedad);

}

void enviarCorreo()
{
  // Si la tierra esta seca envia el correo
  if (estadoInt == 2) {
    String valor = obtenerHumedad();
    Blynk.email("mortasoft@gmail.com", "Debe regar la planta", valor);
  }

}

void loop() {

  Blynk.run();
  timer.run();
  timer2.run();

}
