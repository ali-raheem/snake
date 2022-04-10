/*
Ali Raheem 2022
https://github.com/ali-raheem/snake
MIT License
*/

#include "snake.h"
#include "LED.h"

//#define USE_SERIAL
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3

#ifdef USE_SERIAL
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
#endif

void setup () {
#ifdef USE_SERIAL
  Serial.begin(115200);
#endif
  setupLED();
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
#ifdef USE_SERIAL
  render();
#endif
  ledRender((uint8_t *) framebuffer);
  if(dead)
    reset();
  Button buttons;
  unsigned long d = 10 * (SNAKE_SEGMENTS - snakeLength);
  delay(50);
  readButtons(&buttons);
  gameLoop(buttons);
  if(snakeLength < SNAKE_SEGMENTS)
    delay(d);
}
