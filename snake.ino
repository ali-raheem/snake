/*
Ali Raheem 2022
https://github.com/ali-raheem/snake
MIT License
*/
#include "snake.h"

#include <MD_MAX72xx.h>
#define CLK_PIN   13  // or SCK 
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES  16
#define COLS 32
#define ROWS 32
#define LED_BRIGHTNESS 1

#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void sendBlock(uint8_t *data, uint8_t r, uint8_t c) {
  uint8_t block[8];
  uint8_t *blocksStart = data  + (COLS * r) + c;
  uint8_t i;
  for (i = 0; i < 8; i++)
      block[i] = blocksStart[(COLS/8) * i];
  mx.setBuffer(8 * ((r * (COLS/8)) + c + 1) - 1, 8, block);
}

void ledRender(uint8_t *data){
  uint8_t i, j;
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  mx.clear();
  for(i = 0; i < ROWS/8; i++)
    for(j = 0; j < COLS/8; j++)
      sendBlock(data, i, j);
  mx.transform(MD_MAX72XX::TRC);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}


void render() {
  uint8_t i, j;
  Serial.println("+----------------------------------------------------------------+");
  for(i = 0; i < rows; i++) {
    Serial.print("|");
    for(j = 0; j < cols; j++) {
      Serial.print(getCellState(j, i)?" @":"  ");
    }
    Serial.println("|");
  }
    Serial.println("+----------------------------------------------------------------+");
}


void setup () {
  Serial.begin(115200);
  mx.begin();
  mx.clear();
  mx.control(MD_MAX72XX::INTENSITY, LED_BRIGHTNESS);
  pinMode(A0, INPUT);
  randomSeed(analogRead(A0));
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  gameInit();
}

void reset() {
  asm volatile ("jmp 0");
}

void readButtons(Button *b) {
  *b = BUT_NONE;
  if(!digitalRead(BUTTON_RIGHT)) {
      *b |= BUT_RIGHT;
   };
   if(!digitalRead(BUTTON_LEFT)) {
      *b |= BUT_LEFT;
   };
}

void loop() {
  ledRender((uint8_t *) framebuffer);
  if(dead)
    reset();
  Button buttons;
  unsigned long d = 250 - 10 * snakeLength;
  delay(50);
  readButtons(&buttons);
  gameLoop(buttons);
  delay(d);
}
