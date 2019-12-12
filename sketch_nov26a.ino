// Esta es la librería para utilizar las funciones de red del ESP8266
#include <ESP8266WiFi.h> 

// Datos acceso AP de casa
//const char* ssid = ""; // Rellena con el nombre de tu red WiFi
//const char* password = ""; // Rellena con la contraseña de tu red WiFi

// Datos acceso AP Terrassa
const char* ssid = "Projectes";
const char* password = "Petrux10";
int val;
const char* host = "147.83.83.21";
const char* token = "d3ea4f8b63a99e134e273b2ad5fadb4bf88410aec2023ff3b8e57f276658a836";
const char* provider = "grup_2-101@team2_provider/";
const char* sensor = "brightness-sensor/";
//int value = val;
//String thisString="PUT /data/grup_2-101@team2_provider/brightness-sensor/";
//String putreq = thisString + value + " HTTP/1.1\r\nIDENTITY_KEY: d3ea4f8b63a99e134e273b2ad5fadb4bf88410aec2023ff3b8e57f276658a836\r\n\r\n";

// Configuracion del puerto serie y conexion al AP
void setup() {  
  pinMode(12, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  delay(10);

  // Conectamos a la red WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Configuramos el ESP8266 como cliente WiFi. Si no lo hacemos 
  se configurará como cliente y punto de acceso al mismo tiempo */
  // Modo cliente WiFi
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);

  // Esperamos a que estemos conectados a la red WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Indicamos que hay conexion con el AP
  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}


// Bucle infinito
void loop() {
  // Mostarmos que queremos conectar con el servidor Sentilo
  Serial.print("Abriendo la conexión con ");
  Serial.println(host);
  val = digitalRead(12);
  if(val == 1){digitalWrite(LED_BUILTIN, HIGH); } // turn the LED on (HIGH is the voltage level)
  else {digitalWrite(LED_BUILTIN, LOW); }// turn the LED off by making the voltage LOW
  int value = val;
  String thisString="PUT /data/grup_2-101@team2_provider/brightness-sensor/";
  String putreq = thisString + value + " HTTP/1.1\r\nIDENTITY_KEY: d3ea4f8b63a99e134e273b2ad5fadb4bf88410aec2023ff3b8e57f276658a836\r\n\r\n";
  
  // Establecemos la conexion con el servidor Sentilo
  WiFiClient client;
  // Puerto HTTP
  const int httpPort = 8081;
  // Iniciamos la conexion
  if (!client.connect(host, httpPort)) {
    // ¿hay algún error al conectar?
    Serial.println("Ha fallado la conexión");
    return;
  }
  
  // Enviamos el comando PUT definido
  Serial.print(putreq);
  client.print(putreq);

  // Esperamos a que responda el servidor
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Superado el tiempo de espera !");
      client.stop();
      return;
    }
  }
  
  // Leemos la respuesta y la enviamos al monitor serie
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  // si ha respondido esperamos un poco para cerrar la conexion con el servidor
  timeout = millis();
  while (millis() - timeout < 200);
  // Cerramos la conexion
  client.stop();
  
  Serial.print("Cerrando la conexión con ");
  Serial.println(host);
  // ESperamos a cerrar los procesos de la conexión WiFi
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(500);  
    
  digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
     
 
 
}
