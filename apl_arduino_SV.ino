//Sensor de voltaje - V 1.0
//Proyecto de grado para Maestría en Gestión de Tecnología de Información
//Universidad Nacional Abierta y a Distancia
//Director Mg. Alvaro Cervelion
//Maestrando Ing. Jairo Patiño
//Octubre 2022

//librerias empleaadas ethernet para conexion lan,  mysql para conexion db, time para control del tiempo 
#include <SPI.h>//Serial Peripheral Interface (SPI) es un protocolo de datos serie síncrono utilizado por los microcontroladores para comunicarse con uno o más dispositivos periféricos rápidamente en distancias cortas
#include <Ethernet.h> //libreria empleada para configurar el modulo ethernet
//librerio filtro sensor voltaje
#include <Filters.h> //implifica el procesamiento y eliminación de ruidos en líneas de 50HZ o 60HZ
#include <Wire.h> //Esta librería le permite comunicarse al arduino por medio de modulo interno i2c, ya sea como maestro a otros dispositivos o como esclavo recibiendo peticiones y respondiendo datos. 
//variables configuracion sensor voltaje
float testFrequency = 60;                     // Frecuencia (Hz)
float windowLength = 40.0/testFrequency;     // promedio de la señal
float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0405; // to be adjusted based on calibration testing
float volts; // Voltage s1
float volts2; // Voltage s2
float volts3; // Voltage s3
unsigned long periodo = 5000; //determina el periodo en el cual se toma la muestra de voltaje medida en ms
unsigned long tiempoAnterior = 0; //controla cuanto tiempo a transcurrido
RunningStatistics inputStats; //inicia las variables de medicion 
int muestra=0;    
float promvolt=0;
//puerto analogo de lectura 
int Sensor = 0; //var A9 entrada analoga 
int Sensor2 = 0; //var A10 entrada analoga 
int Sensor3 = 0; //var A11 entrada analoga 
// Configuracion del Ethernet Shield
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xEE}; // Direccion MAC
IPAddress ip; 	 // Dirección IP a utilizar por el Arduino
IPAddress server; //Direccion IP para el server
EthernetClient cliente;//objeto del ethernet
//VARIABLES QUERY
int idnodo; //var nodo
int vsens;//identificador del sensor
int idtfalla; //var falla
long vvolt;//var volt var corriente
//VAR CONFIGURACION 
char opc;
String Mensaje="";
int PosicionPleca;
int PosicionSaltoLinea;
boolean newline=true;
boolean iniciar=false;
boolean programa=false;
boolean calibrar=false;
boolean cfalla=false;
boolean cfalla2=false;
boolean cfalla3=false;
boolean sens1=false;
boolean sens2=false;
boolean sens3=false;
int contmsg=0;

//FUNCIONES
void PresentarConfiguracion(){ //presenta en BT los valores actuales de configuracion de red
  Serial1.println("IP   ");
  Serial1.println(ip); //ip arduinio
  Serial1.println("     \t SERVER                    ");    
  Serial1.println(server);  //ip servidor
  Serial1.println("\tNODO                           ");    
  Serial1.println(idnodo); //id nodo
  Serial1.println("\tEstado Sensor 1            ");
  Serial1.println(sens1); //sensor 1 estado
  Serial1.println("\tEstado Sensor 2            ");
  Serial1.println(sens2); //sensor 2 estado
  Serial1.println("\tEstado Sensor 3            ");
  Serial1.println(sens3); //sensor 3 estado
  Serial1.println("\tPeriodo de muestra         "); //sensor 3 estado
  Serial1.println(periodo); //variable de perido de muestra
}

void menu(){ //menu de opciones de configuracion del arduino
  Serial1.println("1 Ver configuración         ");
  Serial1.println("DEF Valores por defecto     ");
  Serial1.println("IP Direccion IP             ");   
  Serial1.println("SERV Direccion Servidor BD  ");
  Serial1.println("NODO Id NODO                ");
  Serial1.println("S1 Inicia Sensor 1          ");
  Serial1.println("S2 Inicia Sensor 2          ");
  Serial1.println("S3 Inicia Sensor 3          ");
  Serial1.println("PER Duración muestras en ms ");  
  Serial1.println("CAL Calibrar                ");  
  Serial1.println("INI Iniciar                 ");
  Serial1.println("M Menú                      ");      
}

void CargarCadena() { //funcion de lectura de serial de BT celular
  while (!newline){
    opc=Serial1.read();
    if(opc=='\r')    
      continue;
    else if(opc=='\n'){
      newline=false;
      break;
    }
    else
      Mensaje=Mensaje+opc;        
  }
}

void SacarIP(){ //funcion de analsis de cadena BT 
  int PosicionPleca = Mensaje.indexOf('/'); //busca en la cadena la posicion de /
  int PosicionSaltoLinea = Mensaje.length(); //mide la longitud de la cadena
  String TConf = Mensaje.substring(0, PosicionPleca); //divide el mensaje desde el inicio hasta el /   
  String Dato = Mensaje.substring(0, PosicionPleca);  //toma la primera parte del mensaje hasta /     
  //mensaje original
    Dato = Mensaje.substring(PosicionPleca + 1, PosicionSaltoLinea);
    //primer dato
    PosicionPleca = Dato.indexOf(',');   //busca en el mensaje la ,  
    PosicionSaltoLinea = Dato.length();    //mide la longitud del msg
    if (PosicionPleca<0)
      TConf = Mensaje.substring(0, PosicionSaltoLinea); //saca el msg cuando no hay ,
    else
      TConf = Mensaje.substring(0, PosicionPleca); //saca el primer dato antes de la primer ,
    //saca los valores de la IP
    Dato = Dato.substring(PosicionPleca + 1, PosicionSaltoLinea);
    PosicionPleca = Dato.indexOf(',');
    if (TConf.equals("PER")){
      periodo=Dato.substring(0, PosicionPleca).toFloat(); //convierte el valor a int
      Serial1.println("Las muestras se tomaran cada: ");
      if (sens1==1 && sens2==0 && sens3==0)
        Serial1.println(periodo/1000);
      if (sens1==1 && sens2==1 && sens3==0)  
        Serial1.println(2*periodo/1000);
      if (sens1==1 && sens2==1 && sens3==1)
        Serial1.println(3*periodo/1000);
      Serial1.println(" segundos");
    }
    int d1=Dato.substring(0, PosicionPleca).toInt(); //convierte el valor a int
    PosicionSaltoLinea = Dato.length(); //borra el primer dato para sacar el segundo, el proceso se repite para cada valor   
    Serial.println(d1);
    //segundo dato
    Dato = Dato.substring(PosicionPleca + 1, PosicionSaltoLinea);
    PosicionPleca = Dato.indexOf(',');     
    PosicionSaltoLinea = Dato.length();    
    int d2=Dato.substring(0, PosicionPleca).toInt();    
    PosicionSaltoLinea = Dato.length();    
    //tercer dato
    Dato = Dato.substring(PosicionPleca + 1, PosicionSaltoLinea);  
    PosicionPleca = Dato.indexOf(',');     
    PosicionSaltoLinea = Dato.length();    
    int d3=Dato.substring(0, PosicionPleca).toInt();   
    PosicionSaltoLinea = Dato.length();    
    //dato 4
    Dato = Dato.substring(PosicionPleca + 1, PosicionSaltoLinea);  
    PosicionPleca = Dato.indexOf(',');     
    PosicionSaltoLinea = Dato.length();    
    int d4=Dato.substring(0, PosicionPleca).toInt();    
  if(TConf.equals("IP")){//verifica si ingresa IP
      ip=IPAddress(d1,d2,d3,d4); 	//carga el valor IP para arduino
      Serial1.println("IP Arduino: ");
      Serial1.println(ip);
  }
  else{
    if(TConf.equals("S3")){ //verifica si ingresa dns
        if (d1==1)
          sens3=true;      
        else
          sens3=false;
        Serial1.println("S3: ");
        Serial1.println(sens3);
      }                    
    else{
        if(TConf.equals("S1")){ //Habilita sensor1            
          if (d1==1)
            sens1=true;      
          else
            sens1=false;       
          Serial1.println("S1: ");
          Serial1.println(sens1);   
        }      
        else{
          if(TConf.equals("S2")){ //verifica si ingresa mred                        
            if (d1==1)
              sens2=true;      
            else
              sens2=false;
            Serial1.println("S2: ");
            Serial1.println(sens2);
          }                
          else{
              if(TConf.equals("SERV")){//verifica si ingresa serv
                server=IPAddress(d1,d2,d3,d4); //carga ip servidor
                Serial1.println("IP Servidor: ");
                Serial1.println(server);
              }
              else{
                  if(TConf.equals("NODO")){ ////verifica si ingresa nodo
                      idnodo=d1; //carga valor del nodo              
                      Serial1.println("ID Nodo: ");
                      Serial1.println(idnodo);
                  }
                  else{
                      if(TConf.equals("INI")){ ////verifica si ingresa INI
                        iniciar=true; //cambio valor para iniciar programa
                        Ethernet.begin(mac, ip); // Inicializamos el Ethernet Shield                                                 
                        Serial1.println("Dispositivo iniciado");
                      }                      
                      else{
                          if(TConf.equals("DEF")){ //verifica si ingresa def
                              ip=IPAddress(192,168,0,150); 	//carga ip por defecto 
                              server=IPAddress(192,168,0,107); //carga ip sever por defecto
                              idnodo=1; //asigna a valor de nodo 4      
                              sens1=true;                
                              sens2=false;                
                              sens3=false; 
                              periodo=10000;                   
                              Serial1.println("Valores por defecto: ");
                              PresentarConfiguracion(); //presenta los valores de configuracion por BT                                                
                          }
                          else{
                              if(TConf.equals("CAL")){ //verifica si ingresa Ç
                                calibrar=true;
                                periodo=5000;  
                                sens1=true; 
                                Serial1.println("Ingreso al modo calibración"); 
                              }
                              else{
                                if(!TConf.equals("PER")){
                                  Serial.print("No es una configuracion valida");  //si no se ingresa una configuración valida 
                                  Serial1.print("No es una configuracion valida");  //si no se ingresa una configuración valida 
                                }
                              }
                          }                           
                      }  
                  }
              }
          }
        }
    }
  } 
}

void SensorVoltaje1() { //funcion de lectura de voltaje por A9
  bool flag=true; //controla la toma de muestras
  while (flag){ //control de repeticion de toma de muestras
    Sensor = analogRead(A9);  //Leer pin Analógico
    inputStats.input(Sensor);  //captura los valores del puerto 9
    if((unsigned long)(millis() - tiempoAnterior) >= periodo) {  //controla el numero de muestras que se toman, define el tiempo entre muestras        
      volts = intercept + slope * inputStats.sigma(); //offset y amplitud
      volts = volts*(40.3231);                        //calibración  
      Serial.print("\tVoltage S1: "); //imprime mensaje 
      Serial.println(volts); //imprime valor medida en voltio
      Serial1.print("\tVoltage S1: "); //imprime mensaje 
      Serial1.println(volts); //imprime valor medida en voltio
      tiempoAnterior = millis(); //corre el tiempo 
      flag =false; //finaliza la toma de muestras    
    }
  }
}

void SensorVoltaje2() { //funcion de lectura de voltaje por A9
  bool flag=true; //controla la toma de muestras
  //while(true){
  while (flag){ //control de repeticion de toma de muestras
    Sensor2 = analogRead(A10);  //Leer pin Analógico
    inputStats.input(Sensor2);  //captura los valores del puerto 9
    if((unsigned long)(millis() - tiempoAnterior) >= periodo) {  //controla el numero de muestras que se toman, define el tiempo entre muestras        
      volts2 = intercept + slope * inputStats.sigma(); //offset y amplitud
      volts2 = volts2*(40.3231);                        //calibración  
      Serial.print("\tVoltage S2: "); //imprime mensaje 
      Serial.println(volts2); //imprime valor medida en voltio
      Serial1.print("\tVoltage S2: "); //imprime mensaje 
      Serial1.println(volts2); //imprime valor medida en voltio
      tiempoAnterior = millis(); //corre el tiempo 
      flag =false; //finaliza la toma de muestras    
    }
  }
}

void SensorVoltaje3() { //funcion de lectura de voltaje por A9
  bool flag=true; //controla la toma de muestras
  //while(true){
  while (flag){ //control de repeticion de toma de muestras
    Sensor3 = analogRead(A11);  //Leer pin Analógico
    inputStats.input(Sensor3);  //captura los valores del puerto 9
    if((unsigned long)(millis() - tiempoAnterior) >= periodo) {  //controla el numero de muestras que se toman, define el tiempo entre muestras        
      volts3 = intercept + slope * inputStats.sigma(); //offset y amplitud
      volts3 = volts3*(40.3231);                        //calibración  
      Serial.print("\tVoltage S3: "); //imprime mensaje 
      Serial.println(volts3); //imprime valor medida en voltio
      Serial1.print("\tVoltage S3: "); //imprime mensaje 
      Serial1.println(volts3); //imprime valor medida en voltio
      tiempoAnterior = millis(); //corre el tiempo 
      flag =false; //finaliza la toma de muestras    
    }
  }
}

void grabar_seguimiento(){//registra en la bd los valores correspondientes al seguimiento de la red
if (cliente.connect(server, 80)>0) {  // Conexion con el servidor(client.connect(server, 80)>0
    cliente.print("GET /arduino/seguimiento/conexion_arduino.php?node_id="); // Enviamos los datos por GET
    cliente.print(idnodo); //se envia el valor nodo
    cliente.print("&valor_voltaje1="); //se define la variable valorvoltaje para enviarla por get
    cliente.print(volts);  //valor voltaje
    cliente.print("&valor_voltaje2=");//se define la variable valorvoltaje segundo sensor para enviarla por get
    cliente.print(volts2); //valor voltaje2
    cliente.print("&valor_voltaje3=");//se define la variable valorvoltaje tercer sensor para enviarla por get
    cliente.print(volts3); //valor voltaje3
    cliente.println(" HTTP/1.0"); //se finaliza la cadena con el protocolo empleado
    cliente.println("User-Agent: Arduino 1.0"); //se identifica el agente que envia el mensaje
    cliente.println();
    // Lee y muestra la respuesta del servidor
    while (cliente.connected()) {
      if (cliente.available()) {
        char c = cliente.read();
        Serial.print(c);
        Serial1.print(c);
      }
    }
    Serial.println("Envio con exito a PHP seguimiento");//mensaje de confirmacion 
    //delay(1000);
  } else {
    Serial.println("Fallo en la conexion"); //mensaje sin conexion 
    Serial1.println("Fallo en la conexion"); //mensaje sin conexion 
    //delay(2000);
  }
  if (!cliente.connected()) {
    Serial.println("Desconectando");
    //delay(1000);
  }
  cliente.stop(); //detiene la instancia del cliente
  cliente.flush(); //finaliza la instancia del cliente
}

void grabar_fallas(){ //registra en la bd los valores correspondientes al seguimiento
  if (cliente.connect(server, 80)>0) {  // Conexion con el servidor(client.connect(server, 80)>0
    cliente.print("GET /arduino/fallas/curl_falla.php?voltaje="); // Enviamos los datos por GET
    cliente.print(vvolt); //envia el valor del voltaje de la falla medida
    cliente.print("&sensor="); //se define la variable sensor para enviarla por get
    cliente.print(vsens);  //código de identificación del sensor 
    cliente.print("&node_id=");//se define la variable node_id para enviarla por get
    cliente.print(idnodo); //codigo idenficación nodo
    cliente.print("&tipo=");//se define la variable tipo para enviarla por get
    cliente.print(idtfalla); //valor voltaje3
    cliente.println(" HTTP/1.0"); //se finaliza la cadena con el protocolo empleado
    cliente.println("User-Agent: Arduino 1.0"); //se identifica el agente que envia el mensaje
    cliente.println();
    Serial.println("Envio con exito a PHP seguimiento");//mensaje de confirmacion 
    cliente.println(" HTTP/1.0"); //se finaliza la cadena con el protocolo empleado
    cliente.println("User-Agent: Arduino 1.0"); //se identifica el agente que envia el mensaje
    cliente.println();
    // Lee y muestra la respuesta del servidor
    while (cliente.connected()) {
      if (cliente.available()) {
        char c = cliente.read();
        Serial.print(c);
        Serial1.print(c);
      }
    Serial.println("Envio con exito a PHP seguimiento");//mensaje de confirmacion 
    }
  } else {
    Serial.println("Fallo en la conexion"); //mensaje sin conexion 
    Serial1.println("Fallo en la conexion"); //mensaje sin conexion 
  }
  if (!cliente.connected()) {
   Serial.println("Desconectando");
  }
  cliente.stop(); //detiene la instancia del cliente
  cliente.flush(); //finaliza la instancia del cliente
}


void SeguimientoRed() { //funcion encargada de registrar los valores del sensor del voltaje e identificar fallas
  digitalWrite(36, HIGH); //prender led verde
  delay(500);
  digitalWrite(36, LOW); //apagar led verde
//validacion sensor 1  
  if (sens1==true){
  vvolt=volts; //captura el valor del voltaje para valores diferentes al rango 100-150 se considera falla y se clasifica segun la tabla de tipos de fallas
  switch (vvolt) {
    case 50 ... 100:
      idtfalla=1;
      break;
    case -110 ... 49:
      idtfalla=2;
      break;
    case 132 ... 500:
      idtfalla=3;
      break;
    default: 
      idtfalla=0;
    break;
  }
  if(idtfalla!=0 && cfalla==false){ //si se presenta una falla y no ha sido reportada
      vsens=1;
      cfalla=true;
      grabar_fallas(); //graba la falla en la bd
  } 
  if(idtfalla==0){ //si hay una falla y llega la luz
      cfalla=false;  //activa el seguimiento para nuevas fallas
  }  
  }
//validacion del sensor 2
  if (sens2==true){
  vvolt=volts2; //captura el valor del voltaje para valores diferentes al rango 100-150 se considera falla y se clasifica segun la tabla de tipos de fallas
  switch (vvolt) {
    case 50 ... 100:
      idtfalla=1;
      break;
    case -110 ... 49:
      idtfalla=2;
      break;
    case 132 ... 500:
      idtfalla=3;
      break;
    default: 
      idtfalla=0;
    break;
  }
  if(idtfalla!=0 && cfalla2==false){ //si se presenta una falla y no ha sido reportada
      vsens=2;
      cfalla2=true;
      grabar_fallas(); //graba la falla en la bd
  } 
  if(idtfalla==0){ //si hay una falla y llega la luz
      cfalla2=false;  //activa el seguimiento para nuevas fallas
  }  
  }
//validacion del sensor 3
  if (sens3==true){
  vvolt=volts3; //captura el valor del voltaje para valores diferentes al rango 100-150 se considera falla y se clasifica segun la tabla de tipos de fallas
 switch (vvolt) {
    case 50 ... 100:
      idtfalla=1;
      break;
    case -110 ... 49:
      idtfalla=2;
      break;
    case 132 ... 500:
      idtfalla=3;
      break;
    default: 
      idtfalla=0;
    break;
  }
  if(idtfalla!=0 && cfalla3==false){ //si se presenta una falla y no ha sido reportada
      vsens=3;
      cfalla3=true;
      grabar_fallas(); //graba la falla en la bd
  } 
  if(idtfalla==0){ //si hay una falla y llega la luz
      cfalla3=false;  //activa el seguimiento para nuevas fallas
  }
  }    
//grabar seguimiento
  grabar_seguimiento(); //graba el valor del voltaje
}

void(* resetSoftware)(void) = 0;

void setup() //incializa el arduino
{
  Serial1.begin(38400);       // Inicializamos el puerto serie BT (Para Modo AT 2)
  Serial.begin(38400);   // Inicializamos  el puerto serie  
  delay(5000);
  pinMode(34,OUTPUT);
  pinMode(36,OUTPUT);
  digitalWrite(36, LOW);  //apaga led verde  
  Serial1.println("Bienvenido configure el sistema");
}
 
void loop() //cuerpo del programa
{
  digitalWrite(34, HIGH);  //prende led rojo      
  if(Serial1.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    Mensaje = Serial1.readStringUntil('\n'); //lee una cadena desde BT hasta que llega un fin de linea
    if (Mensaje.equals("M")) //valida la opcion menu  
      menu();
    else{
      if (Mensaje=="1")  
        PresentarConfiguracion(); //presenta los valores de configuracion por BT 
      else{
        CargarCadena(); //valida las demas opciones de cadena
        SacarIP();  //carga los valores de configuracion de IP      
      }
    }
  }
   if(Serial.available())  // Si llega un dato por el monitor serial se envía al puerto BT
  {
     Serial1.write(Serial.read());
  }
  delay(500);
  digitalWrite(34, LOW);  //apaga led rojo  
  delay(500);

  inputStats.setWindowSecs(windowLength);
  muestra=0;
  contmsg=0;
  while (calibrar){
    if (sens1==true) 
      SensorVoltaje1(); //mide el sensor de voltaje
    if (sens2==true) 
      SensorVoltaje2(); //mide el sensor de voltaje
    if (sens3==true) 
      SensorVoltaje3(); //mide el sensor de voltaje
    if(contmsg<10){
      contmsg+1;
      Serial1.println("R Resetea Arduino  ");
    }
    Mensaje = Serial1.readStringUntil('\n'); //lee una cadena desde BT hasta que llega un fin de 
    if (Mensaje.equals("R")){ //valida la opcion menu  
      Serial1.println("Reseteado.");
      delay(500);
      resetSoftware();
    }
  }

  contmsg=0;
  while (iniciar){ //el BT da la orden de iniciar con el comando INI
  //while (programa){ //para probarlo sin red 
    Serial.println("Conectando..."); //mensaje de conexion con bd
    digitalWrite(36, HIGH);  //prende led verde
    delay(500); 
    digitalWrite(36, LOW); //apaga led verde
    if (cliente.connect(server, 80)>0) { //llama a la instacia cliente para determinar si conecto 
      digitalWrite(34, LOW); //apaga led rojo
      Serial.println("Comenzando lectura");
      delay(1000);
      iniciar=false; //deshabilita la inicializacion de configuraciones 
      programa=true; //inicia la captura de datos
      cliente.stop(); //detiene instancia cliente
      cliente.flush(); //finaliza estancia cliente
    }else{
      digitalWrite(34, HIGH);//inciende led rojo
      delay(1000);
      digitalWrite(34, LOW);    //apaga led rojo
      delay(1000);
      if(contmsg<10){
        contmsg+1;
        Serial1.println("R Resetea Arduino  ");
        Serial1.print("No se ha podido conectar"); //informa por BT
        Serial.print("No se ha podido conectar"); //informa por terminal
      }
      Mensaje = Serial1.readStringUntil('\n'); //lee una cadena desde BT hasta que llega un fin de 
      if (Mensaje.equals("R")){ //valida la opcion menu  
        Serial1.println("Reseteado.");
        delay(500);
        resetSoftware();
      }
      }    
    }
    while (programa){//la variable logica programa cambia a true y se queda asi para realizar el seguimiento de la red. 
  //while(iniciar){ //para probarlo sin red
    if (sens1==true) 
      SensorVoltaje1(); //mide el sensor de voltaje
    if (sens2==true) 
      SensorVoltaje2(); //mide el sensor de voltaje
    if (sens3==true)   
      SensorVoltaje3(); //mide el sensor de voltaje
      if (volts!=-1.61) //-1.61 es la medida que da al medir por primera vez con el sensor ya que no posee muestras de voltaje, se descarta
        SeguimientoRed();  //graba las medidas de voltaje 
      Mensaje = Serial1.readStringUntil('\n'); //lee una cadena desde BT hasta que llega un fin de 
      if (Mensaje.equals("R")){ //valida la opcion menu  
        Serial1.println("Reseteado.");
        delay(500);
        resetSoftware();
      }
    }
 }
