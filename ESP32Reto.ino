#include <AzureIotHub.h>
#include <Esp32MQTTClient.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

// SSID y password de red abierta
const char* ssid = "INFINITUM8188";
const char* password = "Nm4Bz8Ec2t";

// api key del la base de datos en tiempo real 
#define API_KEY "AIzaSyBdNJ82PipRL0HfpVw5sqidBaZv65xWNOA"
// URL del la base de datos en tiempo real 
#define DATABASE_URL "https://tc1004b-48578-default-rtdb.firebaseio.com"

// Para definir el proyecto fbdo de firebase y darle authorizacion 
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int potentiometerValues[4] = {0}; // Este arreglo sirve para guardar el valor de cada potenciometro.

void setup() {
  Serial.begin(115200);
  delay(10);
  setup_wifi();

  // Se definen los pines en los que se encuentran conectados los potenciometros al ESP32
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
}

// Se hace la configuracion y setup de la red 
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Se hace el signup en firebase
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Signup successful");
    Firebase.reconnectWiFi(true); // Movido aquí
  } else {
    Serial.printf("Signup failed. Reason: %s\n", config.signer.signupError.message.c_str());
  }

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Leer valores analógicos de los potenciómetros
    int pot1Value = analogRead(A4);
    int pot2Value = analogRead(A5);
    int pot3Value = analogRead(A6);
    int pot4Value = analogRead(A7);

    potentiometerValues[0] = pot1Value;
    potentiometerValues[1] = pot2Value;
    potentiometerValues[2] = pot3Value;
    potentiometerValues[3] = pot4Value;

    // Imprimir los valores en el monitor serial
    Serial.print("Potentiometer 1: ");
    Serial.println(pot1Value);
    Serial.print("Potentiometer 2: ");
    Serial.println(pot2Value);
    Serial.print("Potentiometer 3: ");
    Serial.println(pot3Value);
    Serial.print("Potentiometer 4: ");
    Serial.println(pot4Value);

    // Autenticación con Firebase
    if (Firebase.ready()) {
      // En este bucle se van subiendo los datos de los potenciometros a la base de datos en tiempo real de firebase
      for (int i = 0; i < 4; i++) {
        String path = "potentiometers/pot" + String(i + 1);
        if (Firebase.RTDB.setInt(&fbdo, path.c_str(), potentiometerValues[i])) {
          Serial.println("Potentiometer " + String(i + 1) + " value sent successfully");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        } else {
          Serial.println("Failed to send potentiometer " + String(i + 1) + " value");
          Serial.println("REASON: " + fbdo.errorReason());
        }
      }
    }
  }
}
