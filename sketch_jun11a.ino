#include <WiFi.h>
#include <PubSubClient.h>
int sensorPin = 36;
const char* ssid = "home";
const char* password = "safety2015";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "83fe7957-9c47-44ad-abd9-e07fcaa226c4";
const char* mqtt_username = "NaLgc76Fta2JHFSyjLsKFRvc8tdPMeLX";
const char* mqtt_password = "q9V(g)B6UBKSBNs0FaOvkkkvbn8Uyq01";
    
WiFiClient espClient;
PubSubClient client(espClient);
char msg[100];

void reconnect() {
  while (!client.connected()) {
  Serial.print("Sensor MQTT connection…");
  if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
  Serial.println("connected");
  }
  else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println("try again in 5 seconds");
    delay(5000);
  }
 }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  int Moisture;
  Moisture = analogRead(sensorPin);
  Moisture = map(Moisture, 0, 1023, 0, 100);
  Serial.print("Soil moisture : ");
  Serial.print(Moisture);
  Serial.println(" %");
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  String data = "{\"data\": {\"Moisture\":" + String(Moisture) + "}}";
  Serial.println(data);
  data.toCharArray(msg, (data.length() + 1));
  client.publish("@shadow/data/update", msg);
  delay(2000);
}
