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

// =================== INICIALIZAÇÕES ===================
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);

  Serial.println("------ Conexão Wi-Fi ------");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado com sucesso!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  MQTT.setServer(BROKER_MQTT, BROKER_PORT);

  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("Falha ao conectar ao MPU6050!");
    while (1);
  }
  Serial.println("MPU6050 conectado com sucesso!");
  delay(1500);

  // ---- Calibração inicial ----
  Serial.println("Calibrando MPU6050... Mantenha o sensor imóvel!");
  const int samples = 200;
  for (int i = 0; i < samples; i++) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    offsetX += ax;
    offsetY += ay;
    offsetZ += az;
    delay(10);
  }
  offsetX /= samples;
  offsetY /= samples;
  offsetZ /= samples;
  Serial.println("Calibração concluída!");
  delay(1000);
}

// =================== LOOP PRINCIPAL ===================
void loop() {
  if (!MQTT.connected()) reconnectMQTT();
  handleMPU();
  MQTT.loop();
}

// =================== FUNÇÃO: LEITURA DO SENSOR ===================
void handleMPU() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Conversão e remoção do offset
  float accX = ((ax - offsetX) / 16384.0) * 9.81;
  float accY = ((ay - offsetY) / 16384.0) * 9.81;
  float accZ = ((az - offsetZ) / 16384.0) * 9.81;

  // --- FILTRO DE MÉDIA EXPONENCIAL ---
  accX_f = 0.9 * accX_f + 0.1 * accX;
  accY_f = 0.9 * accY_f + 0.1 * accY;
  accZ_f = 0.9 * accZ_f + 0.1 * accZ;

  // --- REMOVER GRAVIDADE (Z) APÓS CALIBRAÇÃO ---
  float accZ_corrigido = accZ_f - 9.81;
  float accResultCorrigido = sqrt(accX_f * accX_f + accY_f * accY_f + accZ_corrigido * accZ_corrigido);


  // --- CÁLCULO DO MÓDULO DA ACELERAÇÃO ---
  float accResult = sqrt(accX_f * accX_f + accY_f * accY_f + accZ_f * accZ_f);

  // --- CÁLCULO DO Δt ---
  unsigned long currentTime = millis();
  float deltaT = (currentTime - lastTime) / 1000.0;
  if (deltaT <= 0) deltaT = 0.1; // segurança mínima


  // --- LIMIAR PARA RUÍDO ---
  if (fabs(accResult) < 0.6) { // ruído típico de sensores parados
    accResult = 0;
  }

  // --- INTEGRAÇÃO ---
  vel += accResult * deltaT;

  // Desaceleração gradual se parado
  if (accResult == 0 && vel > 0) {
    vel *= 0.95; // perda de 5% por ciclo
    if (vel < 0.01) vel = 0;
  }

  dist += vel * deltaT;