/*
Ali Raheem 2022
https://github.com/ali-raheem/snake
MIT License
*/

#ifndef _LED_H
#define _LED_H

#include <MD_MAX72xx.h>
#define CLK_PIN   13  // or SCK 
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES  16
#define COLS 32
#define ROWS 32
#define LED_BRIGHTNESS 1
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

void setupLED() {
  mx.begin();
  mx.clear();
  mx.control(MD_MAX72XX::INTENSITY, LED_BRIGHTNESS);
}
#endif
