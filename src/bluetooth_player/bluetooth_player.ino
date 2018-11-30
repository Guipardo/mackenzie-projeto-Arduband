//Programa: Modulo MP3 DFPlayer Mini com Arduino
//Autor Original: Arduino e Cia

/***************************************************
  DFPlayer - A Mini MP3 Player For Arduino
 ***************************************************
  Created 2016-12-07
  By [Angelo qiao](Angelo.qiao@dfrobot.com)

  GNU Lesser General Public License.
  See <http://www.gnu.org/licenses/> for details.
 ****************************************************/

/* Bibliotecas Arduino */
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial sdcard(10, 11);  // RX, TX (ENTRADA SERIAL LEITOR SD)
SoftwareSerial bluetooth(2, 3); //TX, RX (ENTRADA SERIAL BLUETOOTH)

DFRobotDFPlayerMini myDFPlayer; // Objeto da biblioteca DFRobot

char buf; // Buffer de mostrarMenu
int pausa = 0; // Status da reprodução 0 = Tocando, 1 = Pausado 
int equalizacao = 0; // Número do equalizador selecionado

void mostrarMenu() // Exibe o menu
{
  bluetooth.println();
  bluetooth.println("Menu:");
  bluetooth.println(" [MUS1 - MUS9] Selecionar musica");
  bluetooth.println(" [STOP] Parar reproducao");
  bluetooth.println(" [PAUSE] Pausa/continua a musica");
  bluetooth.println(" [EQUA] Seleciona equalizacao");
  bluetooth.println(" [VOL+/VOL-] Aumenta / Diminui o volume");
  bluetooth.println("");
}

void carregarLeitorSD(){
  bluetooth.println();
  bluetooth.println(F("DFRobot DFPlayer Mini"));
  bluetooth.println(F("Inicializando modulo DFPlayer... (3~5 segundos)"));
  if (!myDFPlayer.begin(sdcard))
  {
    erroLeitura();
  }
  bluetooth.println();
  bluetooth.println(F("Modulo DFPlayer Mini inicializado!"));
}

void erroLeitura(){
  bluetooth.println("ERRO: Nao inicializado:");
  bluetooth.println("1.Cheque as conexoes do DFPlayer Mini");
  bluetooth.println("2.Insira um cartao SD");
  while (true);
}

void configurarPlayer(int timeout, int volume, int equalizador){
  //Definicoes iniciais
  myDFPlayer.setTimeOut(timeout); //Timeout serial
  myDFPlayer.volume(volume); //Volume
  myDFPlayer.EQ(equalizador); //Equalizacao normal
}

/* SETUP */
void setup()
{
  bluetooth.begin(9600); // Inicializa o bluetooth
  sdcard.begin(9600); // Inicializa o leitor SD
  Serial.begin(9600); // Inicializa a porta serial
  
  carregarLeitorSD();
  configurarPlayer(500,25,0); // Timeout, Volume, Equalizador
  mostrarMenu();
}

void loop()
{
  bluetooth.listen(); // Altera o foco da conexão para a porta serial bluetooth
  while (bluetooth.available())
  {
    buf = bluetooth.read(); // Recebe o comando via bluetooth (Aplicativo Terminal Bluetooth)

    //Reproducao
    if ((buf >= '1') && (buf <= '9'))
    {
      bluetooth.print("Reproduzindo: ");  
      bluetooth.println(buf);
      buf = buf - 48;
      myDFPlayer.play(buf); 
      delay(500);
    }
    
    //Parada
    if (buf == 's')
    {
      myDFPlayer.stop();
      bluetooth.println("Parado.");
    }
    
    //Pausa/Continua a musica
    if (buf == 'p')
    {
      pausa = !pausa;
      if (pausa == 0)
      {
        myDFPlayer.start();
        bluetooth.println("Tocando.");
      }
      if (pausa == 1)
      {
        
        myDFPlayer.pause();
        bluetooth.println("Pausado.");
      }
    }
    
    //Seleciona equalizacao
    if (buf == 'e')
    {
      equalizacao++;
      if (equalizacao == 6)
      {
        equalizacao = 0;
      }
      myDFPlayer.EQ(equalizacao);
      
      bluetooth.println("");
      bluetooth.print("Equalizacao: ");
      bluetooth.print(equalizacao);
      bluetooth.println(" (0)Normal (1)Pop (2)Rock (3)Jazz (4)Classic (5)Bass");
      bluetooth.println("");
    }
    
    //Aumenta volume
    if (buf == '+')
    {
      myDFPlayer.volumeUp();
      bluetooth.println("Mais volume.");
    }
    
    //Diminui volume
    if (buf == '-')
    {
      bluetooth.println();
      myDFPlayer.volumeDown();
      bluetooth.print("Menos volume.");
    }

    // Mostrar menu
    if(buf == 'm'){
      mostrarMenu();
    }
  }
}
