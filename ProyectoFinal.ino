// UPIIZ - IPN
// INSTRUMENTACIÓN VIRTUAL
// DIEGO JÁUREGUI | CARLOS CASAS | ALEJANDRO LÓPEZ
// SISTEMA DE CONTROL PARA CALENTAMIENTO DE EXTRUSORA

// DECLARAMOS LAS LIBRERÍAS A UTILIZAR EN EL CÓDIGO
#include<max6675.h>                // LIBRERÍA PARA SENSOR 1 Y 2
#include<OneWire.h>                // LIBRERÍA PARA SENSOR 3
#include<DallasTemperature.h>      // LIBRERÍA PARA SENSOR 3

// DECLARACIÓN DE PINES Y VARIABLE PARA SENSOR 3
OneWire ourWire(2);               // PIN PARA SENSOR 3
DallasTemperature sensors(&ourWire);

//DECLARACIÓN DE PINES Y VARIABLE PARA RESITENCIA 1 Y 2
int pinres = 3;                   // PIN DE RESISTENCIA 1
int resistencia;                  // VARIABLE PARA RESISTENCIA 1

int pinres1 = 11;                 // PIN DE RESISTENCIA 2
int resistencia1;                 // VARIABLE PARA RSISTENCIA 2

// DECLARACIÓN DE PINES DE LECTURA DE SENSOR 1
int CSK=6;
int CS=5;
int SO=4;

// DECLARACIÓN DEL CONJUNTO DE LOS PINES PARA LECTURA
MAX6675 termopar(CSK,CS,SO);  // VARIABLE DE MEDICIÓN SENSOR 1

// DECLARACIÓN DE PINES DE LECTURA DE SENSOR 2
int CSK1=10; 
int CS1=9;
int SO1=8;

// DECLARACIÓN DEL CONJUNTO DE LOS PINES PARA LECTURA
MAX6675 termopar1(CSK1,CS1,SO1);  // VARIABLE DE MEDICIÓN SENSOR 2

// VARIABLES DE TEMPERATURA PARA EL SENSOR 1
float temperatura_i;    // TEMPERATURA INICIAL SENSOR 1
float temperatura;      // TEMPERATURA DE LECTURA SENSOR 1
float temperatura_f;    // TEMPERATURA ACONDICIONADA SENSOR 1

// VARIABLES DE TEMPERATURA PARA EL SENSOR 2
float temperatura_i1;    // TEMPERATURA INICIAL SENSOR 2
float temperatura1;      // TEMPERATURA DE LECTURA SENSOR 2
float temperatura_f1;    // TEMPERATURA ACONDICIONADA SENSOR 2

// VARIABLES DE TEMPERATURA PARA EL SENSOR 3
float temperatura2;      // TEMPERATURA DE LECTURA SENSOR 3
float temperatura_f2;    // TEMPERATURA ACONDICIONADA SENSOR 3

// DECLARACIÓN DE VARIABLE DE CONTADOR
int tiempo = 0;

// DECLARACIÓN DE TIEMPO INICIAL PARA CONTEO
unsigned long msegi=0;


// INICIAMOS CON LA DECLARACIÓN INICIAL EN PRIMER CICLO
void setup() {
  Serial.begin(9600);        // VELOCIDAD DE COMUNICACIÓN DEDATOS
  delay(500);                // PAUSA PARA ACONDICIONAMIENTO DE SENSORES
  pinMode(pinres, OUTPUT);   // DECLARAMOS PIN DE RESISTENCIA 1 COMO SALIDA
  pinMode(pinres1, OUTPUT);  // DECLARAMOS PIN DE RESISTENCIA 2 COMO SALIDA
  
  temperatura_i = termopar.readCelsius();   // TEMPERATURA INCIAL SENSOR 1
  temperatura = 0;    // TEMPERATURA SENSOR 1 NULA PARA EVITAR ERRORES

  temperatura_i1 = termopar1.readCelsius(); // TEMPERATURA INCIAL SENSOR 2
  temperatura1 = 0;    // TEMPERATURA SENSOR 2 NULA PARA EVITAR ERRORES

  sensors.begin();     // INICIALIZACIÓN SENSOR 3
  temperatura2 = 0;    // TEMPERATURA INCIAL SENSOR 3
  temperatura_f2 = 0;  // TEMPERATURA SENSOR 3 NULA PARA EVITAR ERRORES
  sensors.requestTemperatures();  // TOMA DE TEMPERATURA
}

// INICIAMOS EL BUCLE PRINCIPAL DEL CÓDIGO
void loop() {
  // INICIAMOS LA VARIABLE DE COMPARACIÓN DE TIEMPO
  unsigned long msegf=millis();

  // ESCTITURA DE DATOS CADA 100ms AL PUERTO SERIE
  if (msegf-msegi>= 100){
    msegi=msegf;  // IGUALAMOS TIEMPO ACTUAL CON TIEMPO FINAL
    tiempo++;     // SUMAMOS AL CONTADOR

    // MULTIPLICACIÓN A NUMEROS ENTEROS SUPERIORES PARA MANDAR DATO ENTERO
    temperatura_f = temperatura*4;  // VALOR SENSOR 1 POR 4
    
    //VALOR SENSOR 1 RECIVIDOS DE 0 A 250 EN TÉRMINOS DE 0 A 1000
    temperatura_f = map(temperatura_f, 0, 250, 0, 1000);

    temperatura_f1 = temperatura1*4;  // VALOR SENSOR 2 POR 4
    
    //VALOR SENSOR 2 RECIVIDOS DE 0 A 250 EN TÉRMINOS DE 0 A 1000
    temperatura_f1 = map(temperatura_f1, 0, 250, 0, 1000);

    temperatura_f2 = temperatura2*4;  // VALOR SENSOR 3 POR 4

    //VALOR SENSOR 3 RECIVIDOS DE 0 A 250 EN TÉRMINOS DE 0 A 1000
    temperatura_f2 = map(temperatura_f2, 0, 250, 0, 1000);

    // ENVÍO DE DATOS DE LOS TRES SENSORES A TRAVÉS DE PUERTO SERIA
    Serial.print(temperatura_f);    // VALOR SENSOR 1
    Serial.print(",");              // "," COMO SEPRACIÓN ENTRE VALORES
    Serial.print(temperatura_f1);   // VALOR SENSOR 2
    Serial.print(",");              // "," COMO SEPRACIÓN ENTRE VALORES
    Serial.println(temperatura_f2); // VALOR SENSOR 3 Y SALTO DE LÍNEA
  }
  
  // CADA 10 CICLOS, EL VALOR DE LA TEMPERATURA SERÁ LEÍDO DE LOS SENSORES
  if (tiempo>= 10){
    
    // LECTURA DE TEMPERATURA ACTUAL MENOS LA TEMPERATURA INICIAL
    temperatura = (termopar.readCelsius()-temperatura_i);     // TEMPERATURA SENSOR 1
    temperatura1 = (termopar1.readCelsius()-temperatura_i1);  // TEMPERATURA SENSOR 2
    temperatura2 = sensors.getTempCByIndex(0);                // TEMPERATURA SENSOR 3

    // REINCIO DEL TIEMPO PARA REPETICIÓN DEL CICLO
    tiempo = 0;
  }
}

//  CICLO POR SI EXISTE UNA INTERRUPCIÓN EN EL PUERTO SERIE DE LECTURA
void serialEvent()
{
  // SE ACCIONA SI EL PUERTO ESTÁ DISPONIBLE
  if(Serial.available())
  {
    // LECTURA DE LOS DATOS QUE EXISTEN EN LA RECPECIÓN DEL PUERTO SERIE
    resistencia = Serial.parseInt();    // LECTURA SEÑAL DE ENTRADA PARA RESISTENCIA 1
    resistencia1 = Serial.parseInt();   // LECTURA SEÑAL DE ENTRADA PARA RESISTENCIA 2

    // SI LEE UN "ENTER" AL FINAL DE LA LINEA, PASA A LA ESCRITURA A LAS SALIDAS
    if(Serial.read()==char(13))
    {
      // SE PASAN LOS DATOS RECIVIDOS DE 0 A 100, A 0 A 255 PARA LA SALIDA PWM
      resistencia = map(resistencia, 100, 0, 0, 255);     // VALOR SALIDA RESISTENCIA 1
      resistencia1 = map(resistencia1, 100, 0, 0, 255);   // VALOR SALIDA RESISTENICA 2
      analogWrite(pinres, resistencia);      // ESCRITURA DE VALOR EN LA SALIDA ANALÓGICA RESISTENCIA 1
      analogWrite(pinres1, resistencia1);    // ESCRITURA DE VALOR EN LA SALIDA ANALÓGICA RESISTENCIA 2
    }
  }
}
