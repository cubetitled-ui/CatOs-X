// games.cpp
#include "games.h"
#include <Arduino.h>
#include <GyverOLED.h>
#include "GyverButton.h"
#include <Random16.h>
#include <LittleFS.h>

// Внешние переменные
extern GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, CS, DC, RES> oled;
extern GButton up, down, left, right, ok;
extern Random16 rnd;
extern GyverDBFile db;
extern SettingsGyver sett;
extern GTimer_ms gameTimer;
extern GTimer_ms animTimer;

// Глобальные переменные игр 
extern bool alert_f;
extern bool wifiConnected;
extern byte cursor;
extern byte files;
extern float batteryVoltage;
extern int batteryPercentage;

// Функции из main.cpp
extern float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
extern void ui_rama(const char* name, bool draw_b, bool draw_l, bool cleardisplay);
extern void drawbattery();
extern void resetButtons();
extern void buttons_tick();
extern void exit(bool with_update = true);
extern void drawStaticMenu();
extern void updatePointer();



void snakeGame() {
  // Настройки змейки
  const int BLOCK_SIZE = 4; // Размер блока
  const int MOVE_DELAY = 200; // Задержка движения

  // Переменные
  int snakeLength = 4;
  int snakeX[128];
  int snakeY[64];
  int foodX = rnd.get(2, 30) * BLOCK_SIZE;
  int foodY = rnd.get(2, 14) * BLOCK_SIZE;
  int headX = 20;
  int headY = 20;
  int dirX = BLOCK_SIZE;
  int dirY = 0;
  
  uint32_t snakeMoveTmr;

  // Инициализация змейки
  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = headX - i * BLOCK_SIZE;
    snakeY[i] = headY;
  }

  while (true) {
    buttons_tick();
    // Управление
    if (left.isClick() && dirX == 0) {
      dirX = -BLOCK_SIZE;
      dirY = 0;
    } else if (right.isClick() && dirX == 0) {
      dirX = BLOCK_SIZE;
      dirY = 0;
    } else if (down.isClick() && dirY == 0) {
      dirX = 0;
      dirY = BLOCK_SIZE;
    } else if (up.isClick() && dirY == 0) {
      dirX = 0;
      dirY = -BLOCK_SIZE;
    }

    // Движение
    if (millis() - snakeMoveTmr >= MOVE_DELAY) {
      snakeMoveTmr = millis();
      
      // Обновление хвоста
      for (int i = snakeLength; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
      }
      
      // Обновление головы
      headX += dirX;
      headY += dirY;
      snakeX[0] = headX;
      snakeY[0] = headY;

      // Проверка на съедение
      if (headX == foodX && headY == foodY) {
        snakeLength++;
        foodX = rnd.get(2, 30) * BLOCK_SIZE;
        foodY = rnd.get(2, 14) * BLOCK_SIZE;
      }
      
      // Отрисовка в буфер
      oled.clear(); // Очищаем экран перед отрисовкой

      // Змейка
      for (int i = 0; i < snakeLength; i++) {
        oled.rect(snakeX[i], snakeY[i], 
                 snakeX[i] + BLOCK_SIZE - 1, 
                 snakeY[i] + BLOCK_SIZE - 1);
      }
      
      // Еда
      oled.rect(foodX, foodY, 
               foodX + BLOCK_SIZE - 1, 
               foodY + BLOCK_SIZE - 1);
      
      oled.update(); // Обновляем экран только один раз за итерацию
    }

    // Выход
    if (ok.isHold()){
      exit(false);
      return;
    }  
}
}