/*
Ali Raheem 2022
https://github.com/ali-raheem/snake
MIT License
*/

#ifndef _SNAKE_H
#define _SNAKE_H

enum Direction{
  DIR_UP,
  DIR_RIGHT,
  DIR_DOWN,
  DIR_LEFT,
};

enum Button {
  BUT_NONE = 0,
  BUT_LEFT = 1,
  BUT_RIGHT = 2,
};

struct Coords {
  uint8_t x;
  uint8_t y;
};

struct Segment {
  uint8_t x;
  uint8_t y;
  Segment *next;
};

//typedef struct Segment Segment;

#define SNAKE_SEGMENTS 10

Segment snake[SNAKE_SEGMENTS];
Segment *head, *tail;
bool full = false;
bool dead = false;
uint8_t snakeLength = 1;

Direction direction;
uint8_t rows = 32;
uint8_t cols = 32;

uint32_t framebuffer[32] = {0};

void makeCircular(){
  uint8_t seg;
  for(seg = 0; seg < SNAKE_SEGMENTS - 1; seg++) {
    snake[seg].next = &snake[seg + 1];
  }
  snake[seg].next = &snake[0];
}

bool getCellState(uint8_t x, uint8_t y) {
  uint32_t one = 1;
  return !!(framebuffer[y] & (one << x));
}

void turnOnCell(uint8_t x, uint8_t y) {
  uint32_t *row = &framebuffer[y];
  uint32_t one = 1;
  *row |= one << x;
}

void turnOffCell(uint8_t x, uint8_t y) {
  uint32_t *row = &framebuffer[y];
  uint32_t one = 1;
  *row &= ~(one << x);
}

void placeApple() {
  while(true){
    uint8_t a = random() % cols;
    uint8_t b = random() % rows;
    if(!getCellState(a, b)) {
      turnOnCell(a, b);
      break;
    }
  }
}

void gameInit() {
  direction = DIR_RIGHT;
  makeCircular();
  head = snake;
  tail = head;
  head->x = 0;
  head->y = 0;
  turnOnCell(head->x, head->y);
  placeApple();
}

void nextCoords(Coords *cp, Direction dirr) {
  switch(dirr) {
  case DIR_UP:
    cp->y -= 1;
    break;
  case DIR_DOWN:
    cp->y += 1;
    break;
  case DIR_LEFT:
    cp->x -= 1;
    break;
  case DIR_RIGHT:
    cp->x += 1;
    break;
  }
  cp->y %= rows;
  cp->x %= cols;
}

void buttonLeft() {
  switch(direction) {
    case DIR_UP:
      direction = DIR_LEFT;
      break;
    case DIR_DOWN:
      direction = DIR_RIGHT;
      break;
    case DIR_LEFT:
      direction = DIR_DOWN;
      break;
    case DIR_RIGHT:
      direction = DIR_UP;
      break;
  }
  return;
}

void buttonRight() {
  switch(direction) {
    case DIR_DOWN:
      direction = DIR_LEFT;
      break;
    case DIR_UP:
      direction = DIR_RIGHT;
      break;
    case DIR_RIGHT:
      direction = DIR_DOWN;
      break;
    case DIR_LEFT:
      direction = DIR_UP;
      break;
  }
  return;
}

void moveSnake(uint8_t x, uint8_t y) {
  turnOffCell(tail->x, tail->y);
  head = head->next;
  head->x = x;
  head->y = y;
  turnOnCell(x, y);
  tail = tail->next;
}

void growSnake(uint8_t x, uint8_t y) {
  if (full) {
    moveSnake(x, y);
    placeApple();
    return;
  }
  snakeLength++;
  head = head->next;
  head->x = x;
  head->y = y;
  turnOnCell(x, y);
  if(head->next == tail)
    full = true;
  placeApple();
}

bool pointInSnake(uint8_t x, uint8_t y) {
  Segment *t = tail;
  while(t != head) {
    if((x == t->x) && (y == t->y))
      return true;
    t = t->next;
  };
  return false;
}

void gameLoop(Button buttons) {
  if(dead)
    return;
  Coords cp;
  cp.x = head->x;
  cp.y = head->y;
  switch (buttons) {
    case BUT_LEFT:
      buttonLeft();
      break;
    case BUT_RIGHT:
      buttonRight();
      break;
  };
  nextCoords(&cp, direction);
  if(pointInSnake(cp.x, cp.y)) {
      dead = true;
  } else if(getCellState(cp.x, cp.y)) {
    growSnake(cp.x, cp.y);
  } else {
    moveSnake(cp.x, cp.y);
  }
}

#endif
