// games.cpp
#include "games.h"
#include <Arduino.h>
#include <GyverOLED.h>
#include "GyverButton.h"
#include <Random16.h>
#include <LittleFS.h>
#include <map>

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

// Переменные для игр
extern int score1, score2;
extern int player1Y, player2Y;
extern int ballX, ballY;
extern int ballSpeedX, ballSpeedY;
extern float birdY, birdVelocity;
extern int score;
extern bool gameOver;
extern int pipePositions[3], pipeHeights[3];
extern bool pipePassed[3];
extern int paddleX, ballX_ark, ballY_ark, ballSpeedX_ark, ballSpeedY_ark;
extern int lives, score_ark;
extern bool gameRunning;
extern bool bricks[BRICK_ROWS][BRICK_COLS];
extern float playerX, playerY, playerAngle;
extern uint32_t generateSeed();
extern uint8_t parseHFile(uint8_t *img, File &file);
extern void drawPage(File &file, bool storeHistory = true);
extern void enterToReadTxtFile(String filename);
extern void runCatosApp(String filename);
extern void enterToReadFile(void);
extern void ShowFilesLittleFS();
extern void dice_random();
extern void stopwatch();
extern void timer_oled();
extern void control_gpio(bool pin);
extern void choice_gpio();
extern void test(void);
extern void create_settings();
extern void servmode();
extern void batteryCalibration();
extern void formatFS();
extern void deleteSettings_ui();
extern void sysInfo();
extern void testBattery();
extern void deleteSettings();
extern bool draw_logo();
extern void drawbattery();
extern void buttons_tick();
extern void resetButtons();
extern void exit(bool with_update = true);
extern void drawStaticMenu();
extern void updatePointer();
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

void dice_random() {
  #define x_dice 45

  // Настройки анимации (можно менять)
  #define ANIMATION_FRAMES 20     // Общее количество кадров анимации
  #define INITIAL_DELAY 50        // Начальная задержка между кадрами (мс)
  #define FINAL_DELAY 150         // Финальная задержка между кадрами (мс)
  #define ACCELERATION 5          // Ускорение анимации (чем больше - быстрее замедление)

  ui_rama("КУБИК", true, true, true);

  while (true) {
    buttons_tick();

    if (ok.isClick()) {
      // Ждем отпускания кнопки
      while(ok.isPress()) { buttons_tick(); delay(10); }

      // Анимация вращения с замедлением
      int current_delay = INITIAL_DELAY;
      for(int i = 0; i < ANIMATION_FRAMES; i++) {
        // Плавное замедление анимации
        if(i > ANIMATION_FRAMES/2) {
          current_delay = constrain(current_delay + ACCELERATION, INITIAL_DELAY, FINAL_DELAY);
        }

        // Показываем случайный кубик
        int rand_frame = rnd.get(1, 7);
        oled.clear(x_dice, 16, x_dice + 32, 16 + 32);

        switch(rand_frame) {
          case 1: oled.drawBitmap(x_dice, 16, one_rice_32x32, 32, 32); break;
          case 2: oled.drawBitmap(x_dice, 16, two_rice_32x32, 32, 32); break;
          case 3: oled.drawBitmap(x_dice, 16, three_rice_32x32, 32, 32); break;
          case 4: oled.drawBitmap(x_dice, 16, four_rice_32x32, 32, 32); break;
          case 5: oled.drawBitmap(x_dice, 16, five_rice_32x32, 32, 32); break;
          case 6: oled.drawBitmap(x_dice, 16, six_rice_32x32, 32, 32); break;
        }

        oled.update();
        delay(current_delay);
      }

      // Финальный результат
      int rand_dice = rnd.get(1, 7);
      ui_rama("КУБИК", true, true, true);

      switch(rand_dice) {
        case 1: oled.drawBitmap(x_dice, 16, one_rice_32x32, 32, 32); break;
        case 2: oled.drawBitmap(x_dice, 16, two_rice_32x32, 32, 32); break;
        case 3: oled.drawBitmap(x_dice, 16, three_rice_32x32, 32, 32); break;
        case 4: oled.drawBitmap(x_dice, 16, four_rice_32x32, 32, 32); break;
        case 5: oled.drawBitmap(x_dice, 16, five_rice_32x32, 32, 32); break;
        case 6: oled.drawBitmap(x_dice, 16, six_rice_32x32, 32, 32); break;
      }


      oled.update();
    }

    if(ok.isHold()) {
      exit(false);
      return;
    }
  }
}

void PlayDinosaurGame(void) {
  down.setTimeout(160);         // Настраиваем удобные таймауты удержания
  ok.setTimeout(160);
  ok.setStepTimeout(160);

startDinoGame:                         // Начало игры
  uint8_t gameSpeed = 10;              // Скорость игры
  uint16_t score = 0;                  // Текущий счет
  uint16_t bestScore = 0;              // Рекорд
  int8_t oldEnemyPos = 128;            // Позиция старого противника (тот, что уже заходит за горизонт)
  int8_t oldEnemyType = 0;             // Тип старого противника (тот, что уже заходит за горизонт)
  int8_t newEnemyPos = 128;            // Позиция нового противника (тот, что только выходит изза горизонта)
  int8_t newEnemyType = rnd.get(0, 3);  // Тип нового противника - определяем случайно
  bool dinoStand = true;               // Динозавр стоит на земле
  bool legFlag = true;                 // Флаг переключения ног динозавра
  bool birdFlag = true;                // Флаг взмахов птицы
  int8_t dinoY = DINO_GROUND_Y;        // Позиция динозавра по вертикали (изначально на земле)
  float dinoU = 0.0;                   // Скорость динозавра (вектор направлен вниз)


  while (1) {
    buttons_tick();                                             // Бесконечный цикл игры
    if (left.isClick()) { // Клик кнопки влево мгновенно возвращает нас в игровое меню
      down.setTimeout(300);
      ok.setTimeout(300);
      ok.setStepTimeout(400);
      exit(false);
      return;
    }

    /* ------------------ User input ------------------ */
    if ((ok.isClick()) and dinoY == DINO_GROUND_Y) {                           // Клик по ОК и динозавр стоит на земле (слабый прыжок)
      dinoU = -2.8;                                                          // Прибавляем скорости по направлению вверх
      dinoY -= 4;                                                            // Подкидываем немного вверх
    } else if ((ok.isHolded() or ok.isStep()) and dinoY == DINO_GROUND_Y) {  // Удержание ОК и динозавр стоит на земле (сильный прыжок)
      dinoU = -3.4;                                                          // Прибавляем скорости по направлению вверх
      dinoY -= 4;                                                            // Подкидываем немного вверх
    } else if (down.state()) {                                               // Нажатие ВНИЗ
      dinoU = 3.2;                                                           // Прибавляем скорости по направлению к земле
      if (dinoY >= DINO_GROUND_Y) {                                          // Если динозавр коснулся земли
        dinoY = DINO_GROUND_Y;                                               // Ставим его на землю
        dinoU = 0.0;                                                         // Обнуляем скорость
      }
    }

    if (down.isHold() and dinoY >= DINO_GROUND_Y) {                          // Удержание ВНИЗ и дино стоит на земле
      dinoStand = false;                                                     // Переходим в присяд
    } else {
      dinoStand = true;                                                      // Иначе встаем обратно
    }

    /* ------------------ Game processing ------------------ */
    static uint32_t scoreTimer = millis();                                   // Таймер подсчета очков
    if (millis() - scoreTimer >= 100) {
      scoreTimer = millis();
      score++;                                                               // Увеличиваем счет
      gameSpeed = constrain(map(score, 1000, 0, 4, 10), 4, 10);              // Увеличиваем скорость игры! (10 - медленно, 4 - очень быстро)
    }

    static uint32_t enemyTimer = millis();                                   // Таймер кинематики противников
    if (millis() - enemyTimer >= gameSpeed) {                                // Его период уменьшается с ростом счета
      enemyTimer = millis();
      if (--newEnemyPos < 16) {                                              // Как только НОВЫЙ противник приближается к динозавру
        oldEnemyPos = newEnemyPos;                                           // Новый противник становится старым
        oldEnemyType = newEnemyType;                                         // И копирует тип нового к себе
        newEnemyPos = 128;                                                   // Между тем новый противник выходит изза горизонта
        newEnemyType = rnd.get(0, 3);                                         // Имея каждый раз новый случайный тип
      }
      if (oldEnemyPos >= -24) {                                              // Двигаем старый пока он полностью не скроется за горизонтом
        oldEnemyPos--;                                                       // Двигаем старый
      }
    }

    static uint32_t legTimer = millis();                                     // Таймер анимации ног динозавра
    if (millis() - legTimer >= 130) {
      legTimer = millis();
      legFlag = !legFlag;                                                    // Он просто переключает флаг
    }

    static uint32_t birdTimer = millis();                                    // Таймер анимации взмахов птицы
    if (millis() - birdTimer >= 200) {
      birdTimer = millis();
      birdFlag = !birdFlag;                                                  // Он тоже просто переключает флаг!
    }

    static uint32_t dinoTimer = millis();                                    // Таймер кинематики динозавра
    if (millis() - dinoTimer >= 15) {                                        // С периодом DT
      dinoTimer = millis();
      dinoU += (float)DINO_GRAVITY;                                          // Увеличиваем скорость
      dinoY += (float)dinoU;                                                 // И соответственно координату (динозавр падает)
      if (dinoY >= DINO_GROUND_Y) {                                          // При касании с землей
        dinoY = DINO_GROUND_Y;                                               // Ставим динозвра на землю
        dinoU = 0.0;                                                         // Тормозим его до нуля
      }
    }

    /* ------------------ Drawing ------------------ */
    static uint32_t oledTimer = millis();                                    // Таймер отрисовки игры!
    if (millis() - oledTimer >= (1000 / DINO_GAME_FPS)) {                    // Привязан к FPS игры
      oledTimer = millis();

      oled.clear();                                                                                     // Чистим дисплей
      oled.setCursor(0, 0); oled.print("HI");                                                           // Выводим рекорд
      oled.setCursor(13, 0); oled.print(bestScore); oled.print(":"); oled.print(score);                 // Рекорд:текущий счет
      oled.line(0, 63, 127, 63);                                                                        // Рисуем поверхность земли (линия)

      switch (oldEnemyType) {                                                                           // Выбираем старого противника
        case 0: oled.drawBitmap(oldEnemyPos, 48, CactusSmall_bmp, 16, 16);                   break;     // Рисуем маленький кактус
        case 1: oled.drawBitmap(oldEnemyPos, 48, CactusBig_bmp, 24, 16);                     break;     // Рисуем большой кактус
        case 2: oled.drawBitmap(oldEnemyPos, 35, birdFlag ? BirdL_bmp : BirdR_bmp, 24, 16);  break;     // Рисуем птицу (выбираем одну из двух картинок для анимации)
      }

      switch (newEnemyType) {                                                                           // Выбираем нового противника
        case 0: oled.drawBitmap(newEnemyPos, 48, CactusSmall_bmp, 16, 16);                     break;   // Рисуем маленький кактус
        case 1: oled.drawBitmap(newEnemyPos, 48, CactusBig_bmp, 24, 16);                       break;   // Рисуем большой кактус
        case 2: oled.drawBitmap(newEnemyPos, 35, birdFlag ? BirdL_bmp : BirdR_bmp, 24, 16);    break;   // Рисуем птицу (выбираем одну из двух картинок для анимации)
      }

      if (oldEnemyPos <= 16 and oldEnemyPos >= (oldEnemyType > 0 ? -24 : -16)) {                        // Если противник в опасной зоне (Отслеживаем столкновения)
        if (oldEnemyType != 2 ? dinoY > 32 : dinoStand and dinoY > 19) {                                // Выбираем условие столкновения в зависимости от типа противника
          oled.drawBitmap(0, dinoY, DinoStandDie_bmp, 16, 16);                                          // Столкнулись - рисуем погибшего динозавра :(
          oled.roundRect(0, 10, 127, 40, OLED_CLEAR); oled.roundRect(0, 10, 127, 40, OLED_STROKE);      // Очищаем и обводим область
          oled.setScale(2); oled.setCursor(7, 2); oled.print(F("GAME OVER!"));                          // Выводим надпись
          oled.setScale(1); oled.setCursor(3, 4); oled.print(F("<- MENU"));                             // Выводим подсказку
          oled.setCursor(83, 4); oled.print(F("PLAY ->"));                                              // Выводим подсказку
          oled.update();                                                                                // Отрисовка картинки на дисплей                                     // Если новый рекорд - обновляем его
          while (1) {
            buttons_tick();                                                                           // Бесконечный цикл
            if (right.isClick()) goto startDinoGame;                                                    // Нажали на правую - начинаем сначала
            if (left.isClick()) {
              down.setTimeout(300);
              ok.setTimeout(300);
              ok.setStepTimeout(400);
              exit(false);
              return;
            }                                                            // Нажали на левую - вернулись в меню                                                                          // Уходим в сон по необходимости
            }
          }
        }
      }

      if (dinoStand) {                                                                                  // Если все окей, столкновения нет и дино стоит в полный рост
        oled.drawBitmap(0, dinoY, legFlag ? DinoStandL_bmp : DinoStandR_bmp, 16, 16);                   // Выводим в полный рост с анимацией переступания
      } else {                                                                                          // Дино пригнулся
        oled.drawBitmap(0, 56, legFlag ? DinoCroachL_bmp : DinoCroachR_bmp, 16, 8);                     // Выводим пригнувшимся, тоже с анимацией ног
      }

      oled.update();                                                                                    // Финальная отрисовка на дисплей
    }
}
