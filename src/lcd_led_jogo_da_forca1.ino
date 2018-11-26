/*
LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/* VARIÁVEIS DE CONTROLE */
// CHEAT: vermlhoursoaventurabrcofutebolabcte
int stageNumber = 0; // Número da fase
int numberOfWords = 6; // Número de palavras
int lifes = 3; // Numero de tentativas
int score = 0; // Pontuação
char currentletter; // Letra atual do buffer
String answer = ""; // String com a palavra
String temp = ""; // String utilizada para trocar a palavra a cada nova letra 


/* PALAVRAS DO JOGO */
String words[6] = {"vermelho","urso","aventura","barco","futebol","abacate"};
String tips[6] = {"Cor","Animal","Filme","Veiculo","Esporte","Fruta"};


/* Função que mostra um texto no LCD */
void show(int col,int row,String content){
  lcd.setCursor(col,row);
  lcd.print(content);
}

/* Função que esconde a resposta */
void hideword(int index){
  answer = "";
  for(int i = 0; i < words[index].length();i++){
   	answer += "_";
  }
}

/* Função que checa se a letra está certa */
void checkletter(char letter,String word){
  temp = answer; // Guarda a palavra anterior
  for(int j = 0; j < word.length(); j++){
    if(letter == word[j]){
      answer[j] = letter; // Altera a palavra
      score+=20;
    }
  }
  /* Se a palavra não foi modificada perde uma vida*/
  if(temp == answer){ // Compara a palavra antiga com a nova
   	lifes--; 
  }
  /* Se a palavra estiver completa passa de fase */
  if(answer == words[stageNumber]){
    stageNumber++; // Incrementa a fase
    answer = ""; // Limpa a resposta
    lcd.clear();
    selectStage(stageNumber); // Troca de fase
  }
}

/* Pega a letra do buffer e confere se é um caracter alfabético */
void getletter(int index){
  if (Serial.available() > 0) {
    currentletter = Serial.read();
    if(isAlpha(currentletter)){
      checkletter(currentletter,words[index]); // Checa a letra
    } else {
      currentletter = ' ';
   	}
  }
  showLifes(); 
}

/*Mostra a quantidade de vidas restantes */
void showLifes(){
  show(0,1,answer);
  show(13,1,"(");
  show(14,1,String(lifes));
  show(15,1,")");
}

/* Mostra que ganhou */
void win(){
  lcd.clear();
  show(0,0,"Voce ganhou!");
}

/* Mostra que perdeu */
void lose(){
  lcd.clear();
  show(0,0,"Voce perdeu!");
}

/* Mostra a pontuação */
void showScore(){
  show(0,1,"Pontos:");
  show(7,1,String(score));
}

/* Seleciona uma fase */
void selectStage(int stagenumber){
  if(stageNumber < numberOfWords){
    show(0,0,"Dica:");
  	show(5,0,tips[stagenumber]);
  	if(answer == ""){
    	hideword(stagenumber);
  	}
  	show(0,1,answer);
  	getletter(stagenumber);
  }
}

/* Configuração */
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
}

/* Laço de repetição*/
void loop() {
  while(lifes > 0 && stageNumber <= 5){
    delay(500);
    selectStage(stageNumber);
     
  }
  lcd.clear();
  delay(1000);
  if(lifes <= 0){
    lose();
  } else {
    win();
  }
  showScore();
  delay(1000);
}