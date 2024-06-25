#define LED 2
#define QTD_USU 3
struct usuario
{
  String senha;
  String tag;
  String nome;
};
struct usuario usuarios[QTD_USU];
char senha_in[6];
void add_usuario();
//TECLADO MATRICIAL ------------------------------------------------------
#include <Keypad.h>
const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
uint8_t colPins[COLS] = { 5, 14, 2, 15 };
uint8_t rowPins[ROWS] = { 32, 33, 25, 26 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
bool senha_completa = false;
void validacao_senha();
void ler_senha();
//RFID -----------------------------------------------------------------
#include <SPI.h>
#include <MFRC522.h>

#define PINO_SS 21
#define PINO_RST 22
String tag_in;

MFRC522 mfrc522(PINO_SS, PINO_RST);
void validar_tag();
void ler_tag();
//LCD  -----------------------------------------------------------------
#include <LCDWIKI_GUI.h>
#include <SSD1283A.h>

SSD1283A_GUI mylcd(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*LED=*/ -1); //hardware spi,cs,cd,reset,led

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void lcd_print(String texto, int16_t x, int16_t y);
//SPIFFS ---------------------------------------------------------------
#include <FS.h>
#include "SPIFFS.h"

String log_text = "";

String readFile(String path);
void writeFile(String state, String path, String mode);
void formatFile();
void openFS(void);
//NTP ------------------------------------------------------------------
#include <WiFi.h>
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

void tempo_local();
//MQTT -----------------------------------------------------------------
#include <PubSubClient.h>
#include <math.h>

#define IO_USERNAME  "adafruit_io_user"
#define IO_KEY       "adafruit_io_key"
const char* ssid = "rede_wifi";
const char* password ="senha";

const char* mqttserver = "io.adafruit.com";
const int mqttport = 1883;
const char* mqttUser = IO_USERNAME;
const char* mqttPassword = IO_KEY;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

void setup_wifi();
void reconnect();
void enviar_mqtt(String nome, String datetime);
//----------------------------------------------------------------------
void setup() 
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  add_usuario();
  openFS();
  mylcd.init();
  mylcd.Fill_Screen(BLACK);

  setup_wifi();

  client.setServer(mqttserver, 1883);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() 
{
    mylcd.Set_Text_Mode(0);
    mylcd.Fill_Screen(0x0000);
    lcd_print("Bem Vindo", 0, 0, 2);
    ler_senha();
    if(!mfrc522.PICC_IsNewCardPresent()) return;
    if(!mfrc522.PICC_ReadCardSerial()) return;
    ler_tag();

}

void add_usuario()
{
  usuarios[0].nome = "Joao";
  usuarios[0].tag = "DB 4D 5D E1";
  usuarios[0].senha = "AAAAAA";

  usuarios[1].nome = "Pedro";
  usuarios[1].tag = "";
  usuarios[1].senha = "BBBBBB";

  usuarios[2].nome = "Carlos";
  usuarios[2].tag = "99 01 3D 53";
  usuarios[2].senha = "FFFFFF";
}

//TECLADO MATRICIAL ------------------------------------------------------
void validacao_senha()
{
  log_text = "";
  bool isLiberado = false;
  if(senha_completa == true)
  {
    for(int j = 0; j < 3; j++)
    {
      isLiberado = false;
      for(int i = 0; i < 6; i++)
      {
        if(senha_in[i] != usuarios[j].senha[i])
        {
          isLiberado = false;
          break;
        }
        isLiberado = true;
      }
      if(isLiberado)
      {
        Serial.println("Liberado");
        mylcd.Fill_Screen(0x0000);
        lcd_print("Liberado", 0, 0, 2);
        lcd_print(usuarios[j].nome, 0, 32, 2);
        tempo_local();
        digitalWrite(LED, 1);
        enviar_mqtt(usuarios[j].nome, log_text);
        log_text = log_text + " " + usuarios[j].nome;
        writeFile(log_text, "/logs.txt", "w");
        writeFile(log_text, "/lastlogs.txt", "a");
        digitalWrite(LED, 0);
        break;
      }
    }
    if(isLiberado == false)
    {
      mylcd.Fill_Screen(0x0000);
      lcd_print("Negado", 0, 0, 2);
      for (int i = 0; i < 5; i++)
      {
        digitalWrite(LED, 1);
        delay(500);
        digitalWrite(LED, 0);
        delay(500);
      }
    }
    senha_completa = false;
  }
}

void ler_senha()
{
  char key = keypad.getKey();
  // comprueba que se haya presionado una tecla
  if (key && senha_completa == false)
  {
    String temp_senha = "*";
    Serial.println("Senha: ");
    mylcd.Fill_Screen(0x0000);
    lcd_print("Senha: ", 0, 0, 2);
    for(int i = 0; i < 6; )
    {
      do
      {
        key = keypad.getKey();
      }while(!key);
      senha_in[i] = key;
      mylcd.Fill_Screen(0x0000);
      lcd_print("Senha: ", 0, 0, 2);
      lcd_print(temp_senha, 0, 32, 2);
      Serial.print(key);
      Serial.print(" ");
      temp_senha += "*";
      i++;
    }
    senha_completa = true;
    Serial.println("");
  }
  delay(500);
  validacao_senha();
}
//LCD  -------------------------------------------------------------------
void lcd_print(String texto, int16_t x, int16_t y, uint8_t size)
{
  mylcd.Set_Text_colour(RED);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(size);
  mylcd.Print_String(texto, x, y);
  delay(100);
}

//RFID  ------------------------------------------------------------------
void validar_tag()
{
  int i;
  bool isLiberado = false; 
  log_text = "";
  for (i = 0; i < 3; i++)
  {
    if (tag_in.substring(1) == usuarios[i].tag)  
    {
      isLiberado = true;
      break;
    }
  }

  if (isLiberado)
  {
    Serial.println("Liberado");
    mylcd.Fill_Screen(0x0000);
    lcd_print("Liberado", 0, 0, 2);
    lcd_print(usuarios[i].nome, 0, 32, 2);
    tempo_local();
    digitalWrite(LED, 1);
    enviar_mqtt(usuarios[i].nome, log_text);
    log_text = log_text + " " + usuarios[i].nome;
    writeFile(log_text, "/logs.txt", "w");
    writeFile(log_text, "/lastlogs.txt", "a");
    digitalWrite(LED, 0);
  }
  else
  {
    mylcd.Fill_Screen(0x0000);
    lcd_print("Negado", 0, 0, 2);
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(LED, 1);
      delay(500);
      digitalWrite(LED, 0);
      delay(500);
    }
  }
}

void ler_tag()
{
  // Serial.print("UID da tag:");
  mylcd.Fill_Screen(0x0000);
  lcd_print("Lendo TAG", 0, 0, 2);
  tag_in = "";
  byte letra;

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
      if (mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print(" 0");
    }
    else {
      Serial.print(" ");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    if (mfrc522.uid.uidByte[i] < 0x10) {
      tag_in.concat(String(" 0"));
    }
    else {
      tag_in.concat(String(" "));
    }
    tag_in.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tag_in.toUpperCase();
  // Serial.println(tag_in);
  delay(1000);
  validar_tag();
}
//SPIFFS ---------------------------------------------------------------
void writeFile(String state, String path, String mode) { //escreve conteúdo em um arquivo
  File rFile = SPIFFS.open(path, "w");
  if(mode == "a")
    File rFile = SPIFFS.open(path, "a");//a para anexar - w para truncar

  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    //Serial.print("tamanho");
    //Serial.println(rFile.size());
    rFile.println(state);
    //Serial.print("Gravou: ");
    //Serial.println(state);
    //Serial.println();
  }
  rFile.close();
}

String readFile(String path) 
{
  String s;
  Serial.println("Read file");
  File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita
  if (!rFile) 
  {
    Serial.println("Erro ao abrir arquivo!");
  }
  else 
  {
    Serial.print("----------Lendo arquivo ");
    //Serial.print(path);
    Serial.println("  ---------");
    while (rFile.position() < rFile.size())
    {
      s = rFile.readStringUntil('\n');
      s.trim();
      //Serial.println(s);
    }
    rFile.close();
    return s;
  }
}

void formatFile() {
  Serial.println("Formantando SPIFFS");
  SPIFFS.format();
  Serial.println("Formatou SPIFFS");
}

void openFS(void) {
  if (!SPIFFS.begin()) {
    Serial.println("\nErro ao abrir o sistema de arquivos");
  }
  else {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }
}
//NTP ------------------------------------------------------------------
void tempo_local()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  String datetime = String(timeinfo.tm_year + 1900) + "-" +
                    String(timeinfo.tm_mon + 1) + "-" +
                    String(timeinfo.tm_mday) + " " +
                    String(timeinfo.tm_hour) + ":" +
                    String(timeinfo.tm_min) + ":" +
                    String(timeinfo.tm_sec);
  
  log_text = log_text + datetime;
}

//MQTT -----------------------------------------------------------------
void setup_wifi() {

  delay(10);
 
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    // Create a random client ID
    String clientId = "ESP32 - Sensores";
    clientId += String(random(0xffff), HEX);
    // Se conectado
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("conectado");
      // Depois de conectado, publique um anúncio ...
      client.publish("joaogabrielra/feeds/usuario", "Iniciando Comunicação");
      client.publish("joaogabrielra/feeds/datetime", "Iniciando Comunicação");
      client.publish("joaogabrielra/feeds/access", "Iniciando Comunicação");
      //... e subscribe.
      client.subscribe("joaogabrielra/feeds/access"); // <<<<----- mudar aqui
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s");
      delay(5000);
    }
  }
}

void enviar_mqtt(String nome, String datetime)
{
  if (!client.connected()) {
    reconnect();
  }
  reconnect();
  client.loop();
  delay(5000);
  unsigned long now = millis();
  if (now - lastMsg > 2000) 
  {
    lastMsg = now;

    char nome_char[10];
    nome.toCharArray(nome_char, 10);
    client.publish("joaogabrielra/feeds/usuario", nome_char); 
    char datetime_char[20];
    datetime.toCharArray(datetime_char, 20);
    client.publish("joaogabrielra/feeds/datetime", datetime_char);

    bool abilitado = true;
    char s_habilitado[5];
    dtostrf(abilitado,1,2,s_habilitado);
    //dtostrf(ldr,1,2,s_habilitado);
    client.publish("joaogabrielra/feeds/access", s_habilitado);
    delay(5000);
    abilitado = false;
    s_habilitado[5];
    dtostrf(abilitado,1,2,s_habilitado);
    client.publish("joaogabrielra/feeds/access", s_habilitado);
  }
}
