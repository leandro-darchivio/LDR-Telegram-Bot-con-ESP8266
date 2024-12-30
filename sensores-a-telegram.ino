#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// Credenciales WiFi
#define WIFI_SSID "@@@"
#define WIFI_PASSWORD "passw"

// Datos del bot de Telegram
#define BOT_TOKEN "66666666669:AooanoeCE_r4sXL1FzN1FnbTrKohYZ6JoV8"
#define CHAT_ID "1598888888878"

WiFiClientSecure client;

void sendTelegramMessage(String message) {
  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Error conectando al servidor de Telegram");
    return;
  }

  String url = "/bot" + String(BOT_TOKEN) + "/sendMessage?chat_id=" + String(CHAT_ID) + "&text=" + message;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.telegram.org\r\n" +
               "Connection: close\r\n\r\n");

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line); // Para depuración
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Conectando a WiFi...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConexión establecida.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  client.setInsecure(); // Deshabilita la verificación SSL (inseguro, solo para pruebas)
}

void loop() {
  static unsigned long lastLDRTime = 0;
  unsigned long currentTime = millis();

  // Leer y enviar valor del LDR cada 10 segundos
  if (currentTime - lastLDRTime >= 10000) {
    int ldrValue = analogRead(A0); // Leer el pin analógico
    sendTelegramMessage("Valor del LDR: " + String(ldrValue));
    lastLDRTime = currentTime;
  }
}
