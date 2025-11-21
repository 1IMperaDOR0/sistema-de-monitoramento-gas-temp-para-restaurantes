#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define PIN_DS18B20 4       
#define PIN_GAS 34         

#define LED_TEMP_VERDE 14
#define LED_TEMP_VERMELHO 12
#define LED_TEMP_AMARELO 13

#define LED_GAS_VERDE 25
#define LED_GAS_VERMELHO 26
#define LED_GAS_AMARELO 27

#define BUZZER_TEMP 33
#define BUZZER_GAS 32

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";

OneWire oneWire(PIN_DS18B20);
DallasTemperature sensors(&oneWire);

void beep(int pin, int tempoMs) {
  digitalWrite(pin, HIGH);
  delay(tempoMs);
  digitalWrite(pin, LOW);
}

WiFiClient WOKWI_Client;
PubSubClient client(WOKWI_Client);

void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.println("Conectando para ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attemping MQTT connection...");

    if (client.connect("WOKWI_Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // LEDs
  pinMode(LED_TEMP_VERDE, OUTPUT);
  pinMode(LED_TEMP_VERMELHO, OUTPUT);
  pinMode(LED_TEMP_AMARELO, OUTPUT);

  pinMode(LED_GAS_VERDE, OUTPUT);
  pinMode(LED_GAS_VERMELHO, OUTPUT);
  pinMode(LED_GAS_AMARELO, OUTPUT);

  pinMode(2, OUTPUT);
  pinMode(15, OUTPUT);

  // Buzzers
  pinMode(BUZZER_TEMP, OUTPUT);
  pinMode(BUZZER_GAS, OUTPUT);

  sensors.begin();

  // ADC do ESP32 pega 0–4095
  analogReadResolution(12);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void WiFi_Conectado() {
  if (WiFi.status()) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
}

void Mosquitto_Conectado() {
  if (client.connected()) {
    digitalWrite(15, HIGH);
  } else {
    digitalWrite(15, LOW);
  }
}

void loop() {
  reconnect();
  WiFi_Conectado();
  Mosquitto_Conectado();

  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  if (temp >= 50) {
    digitalWrite(LED_TEMP_VERDE, LOW);
    digitalWrite(LED_TEMP_VERMELHO, HIGH);
    digitalWrite(LED_TEMP_AMARELO, LOW);
    beep(BUZZER_TEMP, 250);
  } 
  else if (temp > 30) {  
    digitalWrite(LED_TEMP_VERDE, LOW);
    digitalWrite(LED_TEMP_VERMELHO, LOW);
    digitalWrite(LED_TEMP_AMARELO, HIGH);
  } 
  else {
    digitalWrite(LED_TEMP_VERDE, LOW);
    digitalWrite(LED_TEMP_VERMELHO, LOW);
    digitalWrite(LED_TEMP_AMARELO, HIGH);
  }

  int gas = analogRead(PIN_GAS);  

  if (gas > 3500) {  
    digitalWrite(LED_GAS_VERDE, LOW);
    digitalWrite(LED_GAS_VERMELHO, HIGH);
    digitalWrite(LED_GAS_AMARELO, LOW);
    beep(BUZZER_GAS, 250);
  } else if (gas > 2750) {
    digitalWrite(LED_GAS_VERDE, LOW);
    digitalWrite(LED_GAS_VERMELHO, LOW);
    digitalWrite(LED_GAS_AMARELO, HIGH);
  } else {
    digitalWrite(LED_GAS_VERDE, HIGH);
    digitalWrite(LED_GAS_VERMELHO, LOW);
    digitalWrite(LED_GAS_AMARELO, LOW);
  }

  client.publish("topico_wokwi_temp", String(temp).c_str());
  client.publish("topico_wokwi_gas", String(gas).c_str());

  delay(300);
}