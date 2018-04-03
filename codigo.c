//Include da lib de Wifi do ESP8266
#include <ESP8266WiFi.h>
//Include da lib do sensor DHT11 e DHT22
#include "DHT.h"

//Define do pino a ser utilizado no ESP para o sensor = GPIO4
#define DHT_DATA_PIN 4
#define UMIDADE_SOLO_DATA_PIN 0
#define DHTTYPE DHT22

//Constante que representa o pino onde o positivo do buzzer será ligado.
const int buzzer = 16;

//Definir o SSID e a senaha da rede WiFi
const char* ssid = "GVT-396B";
const char* password = "S1EB023463";
//const char* ssid = "iPhone de Savio";
//const char* password = "savio123";

//Colocar a API Key para escrita neste campo
//Ela é fornecida no canal que foi criado na aba API Keys
String apiKey = "HU918WU3HD1NSRMH";
const char* server = "api.thingspeak.com";

DHT dht(DHT_DATA_PIN, DHTTYPE);
WiFiClient client;

void setup(){

  //Definindo o pino buzzer como de saída.
  pinMode(buzzer,OUTPUT);
  
  //Configuração da UART
  Serial.begin(9600);
  //Inicia o WiFi
  WiFi.begin(ssid, password);

  //Espera a conexão no router
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  dht.begin();

  //Logs na porta serial
  Serial.println("");
  Serial.print("Conectado na rede: ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  //Espera 60 segundos para fazer a leitura
  delay(60000);
  
  Serial.print("\n");
  Serial.println("...............................");
  Serial.println("Lendo os sensores!");
  
  //Leitura de umidadeAmbiente
  float umidadeAmbiente = dht.readHumidity();
  //Leitura de temperaturaAmbiente
  float temperaturaAmbiente = dht.readTemperature();

  //Se não for um numero retorna erro de leitura
  if (isnan(umidadeAmbiente) || isnan(temperaturaAmbiente)) {
    Serial.println("Erro ao ler o sensor!");
    return;
  }

  int valorADC;
  float umidadeSolo;

  valorADC = analogRead(UMIDADE_SOLO_DATA_PIN);   //978 -> 3,3V   
  umidadeSolo = 100 * ((978-(float)valorADC) / 978);


  //Inicia um client TCP para o envio dos dados
  if (client.connect(server,80)) {
    String postStr = apiKey;
           postStr +="&amp;field1=";
           postStr += String(temperaturaAmbiente);
           postStr +="&amp;field2=";
           postStr += String(umidadeAmbiente);
           postStr +="&amp;field3=";
           postStr += String(umidadeSolo);
           postStr += "\r\n\r\n";

     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);

     Serial.println("Dados enviados com sucesso!");

     //Logs na porta serial
     Serial.print("Temperatura do Ambiente: ");
     Serial.println(temperaturaAmbiente);
     Serial.print("Umidade do Ambiente: ");
     Serial.println(umidadeAmbiente);
     Serial.print("Umidade do Solo: ");
     Serial.println(umidadeSolo);

  }
  client.stop();

  //Ligando o buzzer com uma frequencia de 1500 hz.
  tone(buzzer,1500);   
  delay(500);
   
  //Desligando o buzzer.
  noTone(buzzer);
  delay(500);
}
