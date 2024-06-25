# Projeto - Sistema de Seguramça

## Objetivo do Projeto

O projeto tem como objetivo, a implementação dos conceitos adquiridos nas aulas teóricas e práticas da disciplina, em algo de complexidade maior que englobe os conceitos.
Para que o projeto seja considerado completo, ele deve satisfazer o uso de: Sensores, Atuadores, Comunicação Bluetooth Low Energy / MQTT / Serial, Armazenamento Interno, Dashboard e uso do Kit de desenvolvimento esp32.

## Componentes Usados

esp32 WROOM Dev Kit

Módulo RFID MFRC522

SSD1283A

Teclado Matricial 4x4

![Imagem dos componentes](https://github.com/SidneyJunior01234/IMD0904-PLATAFORMAS-DE-HARDWARE-PARA-INTERNET-DAS-COISAS/blob/main/Projeto%20Final%20-%20Sistema%20de%20Seguran%C3%A7a/Imagens/Componentes_IMG.jpg)

## Esquemático do Circuito

![](https://github.com/SidneyJunior01234/IMD0904-PLATAFORMAS-DE-HARDWARE-PARA-INTERNET-DAS-COISAS/blob/main/Projeto%20Final%20-%20Sistema%20de%20Seguran%C3%A7a/Imagens/bibliotecas_01_IMG.jpg)

![](https://github.com/SidneyJunior01234/IMD0904-PLATAFORMAS-DE-HARDWARE-PARA-INTERNET-DAS-COISAS/blob/main/Projeto%20Final%20-%20Sistema%20de%20Seguran%C3%A7a/Imagens/bibliotecas_02.jpg)

## Bibliotecas Necessárias para o Funcionamento
```
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LCDWIKI_GUI.h>
#include <SSD1283A.h>
#include <FS.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include "time.h"
#include <PubSubClient.h>
#include <math.h>
```
Também é necessário ter a bibliotec `Adafruit_GFX_Library` para que seja possível utilizar corretamente o LCD SSD1283A. Além disso, pode ocorrer um erro de comparação entre um ponteiro e um inteiro 0, que ocorre no arquivo MFRC522Extended.cpp. Para resolver o problema é necessário ir até o diretório das bibliotecas do arduino em Documentos `..\Documentos\Arduino\libraries\MFRC522\src\MFRC522Extended.cpp` nas linhas 824 e 847 onde há `if(backData && (backLen > 0)){` e realizar a seguinte modificação `if(backData && (*backLen > 0)){`, com isso o erro será resolvido.



