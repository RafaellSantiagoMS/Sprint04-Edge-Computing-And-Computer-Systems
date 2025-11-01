#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <MPU6050.h>

// =================== CONFIGURAÇÕES ===================
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";
const char* BROKER_MQTT = "18.228.26.115";
const int BROKER_PORT = 1883;

const char* TOPICO_SUBSCRIBE = "/TEF/iot001/cmd";
const char* TOPICO_PUBLISH_ACC = "/TEF/iot001/attrs/acc";
const char* TOPICO_PUBLISH_VEL = "/TEF/iot001/attrs/vel";
const char* TOPICO_PUBLISH_DIST = "/TEF/iot001/attrs/dist";
const char* TOPICO_PUBLISH_STATUS = "/TEF/iot001/attrs";
const char* ID_MQTT = "fiware_iot001";

const int LED = 2; // LED onboard

// =================== OBJETOS GLOBAIS ===================
WiFiClient espClient;
PubSubClient MQTT(espClient);
MPU6050 mpu;

// =================== VARIÁVEIS FÍSICAS ===================
float vel = 0.0;
float dist = 0.0;
unsigned long lastTime = 0;

// Filtro e calibração
float accX_f = 0, accY_f = 0, accZ_f = 0;
float offsetX = 0, offsetY = 0, offsetZ = 0;
