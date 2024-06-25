# Projeto - Sistema de Seguramça

# Objetivo do Projeto

O projeto tem como objetivo, a implementação dos conceitos adquiridos nas aulas teóricas e práticas da disciplina, em algo de complexidade maior que englobe os conceitos.
Para que o projeto seja considerado completo, ele deve satisfazer o uso de: Sensores, Atuadores, Comunicação Bluetooth Low Energy / MQTT / Serial, Armazenamento Interno, Dashboard e uso do Kit de desenvolvimento esp32.

# Problematização Geral

No dia a dia do brasileiro há os imprevistos ocasionados pelo crime, desde um furto assim como a invasão de residências. Com essas preocupações a tecnologia foi utilizada no contexto da segurança e onde podemos obserar de perto o uso dessas tecnologias, nas portarias inteligentes de condomínos, onde são utilizadas câmeras para reconhecimento facial, leitor de cartões ou chaveiros via radio frequência, e até mesmo o uso de teclados para o uso de senhas.

# Problematização Aplicada ao Projeto 

Dado a problematização geral, a ideia da portaria inteligente nos permite pensar, se seria aplicável realizar algo similar com a utilização de alguns componentes eletrônicos. Com esse problema, nos motivamos a propor um sistema similar, que nos permite realizar o controle de acesso dos usuários ao laboratório 201 do Núcleo de Pesquisa e Inovação em Tecnologia da Informação (nPITI) - IMD/UFRN. Para isso utilizamos os componentes disponíveis em laboratório e fornecidos pelo professor da disciplina.

# Descrição de Funcionamento

O funcionamento da aplicação se dá na entrada de informação no kit de desenvolvimento, entrada essa proveniente de uma senha digitada no teclado ou pela aproximação de um cartão / chaveiro RFID. Uma dessas informações será lida e comparadas com a dos usuários cadastrados, caso seja um usuário válido o sistema libera o acesso, armasena internamente o log do nome do usuário juntamente com a data e hora de acesso, Assim como envia via protocolo MQTT para o Adafruit IO onde é prossível acompanhar o acesso dos usuários.

# Componentes Usados

esp32 WROOM Dev Kit

Módulo RFID MFRC522

SSD1283A

Teclado Matricial 4x4

[Adafruit IO](https://io.adafruit.com/)

*Componentes*
![Imagem dos componentes](https://github.com/SidneyJunior01234/IMD0904-PLATAFORMAS-DE-HARDWARE-PARA-INTERNET-DAS-COISAS/blob/main/Projeto%20Final%20-%20Sistema%20de%20Seguran%C3%A7a/Imagens/Componentes_IMG.jpg)

*Dashboard - Adafruit IO*
![Dashboard](https://github.com/SidneyJunior01234/IMD0904-PLATAFORMAS-DE-HARDWARE-PARA-INTERNET-DAS-COISAS/blob/main/Projeto%20Final%20-%20Sistema%20de%20Seguran%C3%A7a/Imagens/adafruit.jpg)

# Esquemático do Circuito

![](https://github.com/SidneyJunior01234/IMD0904-PLATAFORMAS-DE-HARDWARE-PARA-INTERNET-DAS-COISAS/blob/main/Projeto%20Final%20-%20Sistema%20de%20Seguran%C3%A7a/Imagens/bibliotecas_01_IMG.jpg)

![](https://github.com/SidneyJunior01234/IMD0904-PLATAFORMAS-DE-HARDWARE-PARA-INTERNET-DAS-COISAS/blob/main/Projeto%20Final%20-%20Sistema%20de%20Seguran%C3%A7a/Imagens/bibliotecas_02.jpg)

# Bibliotecas Necessárias para o Funcionamento e Eventuais Modificações
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

#Código Fonte
#Conclusão
