#include <ESP8266WebServer.h>

#define HTTP_WEB_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50
#define MOTORE_SX_PIN1 D0
#define MOTORE_SX_PIN2 D1
#define MOTORE_DX_PIN1 D3
#define MOTORE_DX_PIN2 D4
#define AVANTI 1
#define INDIETRO 0

const char* wifi_ssid = "RobotCar-01";

IPAddress local_IP(192,168,10,10);
IPAddress gateway(192,168,10,10);
IPAddress subnet(255,255,255,0);

ESP8266WebServer http_server(HTTP_WEB_PORT);

void setup(void) {
    Serial.begin(115200);
    pinMode(MOTORE_SX_PIN1, OUTPUT);
    pinMode(MOTORE_SX_PIN2, OUTPUT);
    pinMode(MOTORE_DX_PIN1, OUTPUT);
    pinMode(MOTORE_DX_PIN2, OUTPUT);

    Serial.println("Configurazione RoboCar WiFi AP..........");
    WiFi.softAPConfig(local_IP, gateway, subnet); 
    WiFi.softAP(wifi_ssid);

    Serial.print("RoboCar WiFi AP IP Address = ");
    Serial.println(WiFi.softAPIP());

    config_server_routing();

    http_server.begin();
    Serial.println("RoboCar HTTP Server Started");
}

void config_server_routing() {
    http_server.on("/", HTTP_GET, []() { http_server.send(200, "text/html", "Benvenuto sul ESP8266 RoboCar Web Server"); });
    http_server.on("/robocar", HTTP_GET, muovi_robocar);
}

void muovi_robocar() {
  String comando = String(http_server.arg(0));
  Serial.print("Ricevuto comando = ");
  Serial.println(comando);

  http_server.send(200, "text/html", "robocar - comando ricevuto");

  if (comando == "AVANTI")
    robocar_avanti();
  else if (comando == "INDIETRO")
    robocar_indietro();
  else if (comando == "DESTRA")
    robocar_destra();
  else if (comando == "SINISTRA")
    robocar_sinistra();
  else if (comando == "FERMO")
    robocar_fermo();
}

void motore_sx_avanti() {
  Serial.println("motore_sx avanti");
  
  digitalWrite(MOTORE_SX_PIN1, HIGH);
  digitalWrite(MOTORE_SX_PIN2, LOW);
}

void motore_sx_indietro() {
  Serial.println("motore_sx");
 
  digitalWrite(MOTORE_SX_PIN1, LOW);
  digitalWrite(MOTORE_SX_PIN2, HIGH);
}

void motore_sx_fermo() {
  Serial.println("motore_sx fermo");
  digitalWrite(MOTORE_SX_PIN1, LOW);
  digitalWrite(MOTORE_SX_PIN2, LOW);
}

void motore_dx_avanti() {
  Serial.println("motore_dx");
  
  digitalWrite(MOTORE_DX_PIN1, HIGH);
  digitalWrite(MOTORE_DX_PIN2, LOW);
}

void motore_dx_indietro() {
  Serial.println("motore_dx");
  
  digitalWrite(MOTORE_DX_PIN1, LOW);
  digitalWrite(MOTORE_DX_PIN2, HIGH);
}

void motore_dx_fermo() {
  Serial.println("motore_dx fermo");
  digitalWrite(MOTORE_DX_PIN1, LOW);
  digitalWrite(MOTORE_DX_PIN2, LOW);
}

void robocar_avanti() {
  motore_sx_avanti();
  motore_dx_avanti();
}

void robocar_indietro() {
  motore_sx_indietro();
  motore_dx_indietro();
}

void robocar_destra() {
  motore_dx_avanti();
}

void robocar_sinistra() {
  motore_sx_avanti();
}

void robocar_fermo() {
  motore_sx_fermo();
  motore_dx_fermo();
}

void loop(void) {
    http_server.handleClient();
}
