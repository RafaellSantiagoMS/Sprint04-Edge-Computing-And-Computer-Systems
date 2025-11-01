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
