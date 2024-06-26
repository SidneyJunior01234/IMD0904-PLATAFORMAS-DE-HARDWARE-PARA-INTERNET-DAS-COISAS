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

*Bibliotecas*

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
*Erro de compilação*
Também é necessário ter a bibliotec `Adafruit_GFX_Library` para que seja possível utilizar corretamente o LCD SSD1283A. Além disso, pode ocorrer um erro de comparação entre um ponteiro e um inteiro 0, que ocorre no arquivo MFRC522Extended.cpp. Para resolver o problema é necessário ir até o diretório das bibliotecas do arduino em Documentos `..\Documentos\Arduino\libraries\MFRC522\src\MFRC522Extended.cpp` nas linhas 824 e 847 onde há `if(backData && (backLen > 0)){` e realizar a seguinte modificação `if(backData && (*backLen > 0)){`, com isso o erro será resolvido.

Para Adicionar novos usuários é preciso conhecer a estrutura e modificar a função que adiciona os usuários.

*Estrutura*

```
struct usuario
{
  String senha;
  String tag;
  String nome;
};
```

Para adicionar mais usuários altere o valor de `#define QTD_USU valor` e na função add_usuario adicionar as seguintes linhas:

```
  usuarios[indice].nome = "nome";
  usuarios[indice].tag = "TAG RFID";
  usuarios[indice].senha = "senha";
```

Para utilizar a data e hora de alguma região específica, altere gmtOffset_sec que corresponde ao UTC e daylightOffset_sec que é referente ao horpario de verão:

```
const long  gmtOffset_sec = -10800;//Brasília UTC-3 -> -3*60*60 = -10800
const int   daylightOffset_sec = 0;
```

Para realizar a conexão com o Adafruit IO, assim como a conexão com a internet via WIFI modifique:

```
#define IO_USERNAME  "adafruit_io_user"
#define IO_KEY       "adafruit_io_key"
const char* ssid = "rede_wifi";
const char* password ="senha";
```
Cso seja necessário visualizar os logs armazenados, basta utilizar `String readFile(String path);` para ler uma linha, ao executar em um laço irá ler as linhas uma a uma.

# Código Fonte

[Código Fonte disponível](https://github.com/SidneyJunior01234/IMD0904-PLATAFORMAS-DE-HARDWARE-PARA-INTERNET-DAS-COISAS/blob/main/Projeto%20Final%20-%20Sistema%20de%20Seguran%C3%A7a/C%C3%B3digo%20Fonte/Projeto_Seguranca.ino)

# Visualização do Teste

[![Teste do Projeto](https://img.youtube.com/vi/zWmpUKK1g3U/0.jpg)](https://youtu.be/zWmpUKK1g3U)

#Conclusão

Para uma primeira versão do projeto, tivemos resultados satisfatórios devido ao seu funcionamento. Para que se torne uma aplicação para o dia a dia, devemos utilizar sensores específicos para as portas para identificar se houve uma entrada forçada, assim como melhorar em quesito de código criando uma biblioteca própria para o projeto permitindo criar módulos onde podemos escolher quais componentes podemos utilizar em locais diferentes, melhoria em adicionar novos usuários como por meio de uma interface gráfica.

# Participanetes

[João Gabriel Rodrigues de Azevedo ](http://github.com/joaogabrielra)
Sidney Alves dos Santos Junior
