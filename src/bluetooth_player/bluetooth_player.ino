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
  /* As opções estão no formato [comando / atalho]
  Onde o comando é a letra referente ao comando
  e o atalho é referente ao botão do aplicativo Bluetooth Terminal que foi personalizado */
  bluetooth.println();
  bluetooth.println(" Menu:");
  bluetooth.println(" [1-9 MUS1 - MUS9] Selecionar musica");
  bluetooth.println(" [s/STOP] Parar reproducao");
  bluetooth.println(" [p/PAUSE] Pausa/continua a musica");
  bluetooth.println(" [e/EQUA] Seleciona equalizacao");
  bluetooth.println(" [+/- VOL+/VOL-] Aumenta / Diminui o volume");
  bluetooth.println(" [m/MENU] Aumenta / Diminui o volume");
  bluetooth.println("");
}

void carregarLeitorSD(){
  bluetooth.println();
  bluetooth.println("DFRobot DFPlayer Mini");
  bluetooth.println("Inicializando modulo DFPlayer... (3~5 segundos)");
  if (!myDFPlayer.begin(sdcard)) // Verifica se o cartão foi carregado
  {
    erroLeitura();
  }
  bluetooth.println();
  bluetooth.println("Modulo DFPlayer Mini inicializado!");
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
  
  /* CONFIGURAÇÃO DO PLAYER */
  carregarLeitorSD();
  configurarPlayer(500,15,0); // Timeout, Volume, Equalizador
  mostrarMenu();
}

void loop()
{
  bluetooth.listen(); // Altera o foco da conexão para a porta serial bluetooth
  while (bluetooth.available()) // Enquanto houver entradas no buffer
  {
    buf = bluetooth.read(); // Recebe o comando via bluetooth (Aplicativo Terminal Bluetooth)

    //Reproducao
    if ((buf >= '1') && (buf <= '9'))
    {
      bluetooth.print("Reproduzindo: ");  
      bluetooth.println(buf);
      buf = buf - 48; // Valor que será usado para executar a faixa de mp3
      myDFPlayer.play(buf); 
      delay(500);
    }
    
    //Parada
    if (buf == 's') // Letra s - para a faixa de áudio
    {
      myDFPlayer.stop();
      bluetooth.println("Parado.");
    }
    
    //Pausa/Continua a musica
    if (buf == 'p') // Letra p - pausa ou continua a faixa de áudio
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
    if (buf == 'e') // Letra e - muda a equalização da faixa de áudio
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
    if (buf == '+') // Caractere + - aumenta o vloume
    {
      myDFPlayer.volumeUp();
      bluetooth.println("Mais volume.");
    }
    
    //Diminui volume
    if (buf == '-') // Caractere + - diminui o volume
    {
      bluetooth.println();
      myDFPlayer.volumeDown();
      bluetooth.print("Menos volume.");
    }

    // Mostrar menu
    if(buf == 'm'){ // Letra m - mostra o menu
      mostrarMenu();
    }
  }
}
