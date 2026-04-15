# CatOS X Прошивка игровой консоли
| *> CatOs X <* | [CatOs Lite](https://github.com/CatDevCode/CatOs_Lite/) |
| --- | --- |
> [!WARNING]
> Это форк оригинального проекта могут быть недочеты и ошибки для стабильной версии перейдите на оригинальный репозиторий!

![LOGO](assets/logo.jpg)

Прошивка для портативной игровой консоли на базе ESP32 с OLED-дисплеем. Включает набор игр, утилит и системных инструментов.

## Особенности
- 🕹️ Игры: Тетрис, Змейка, Flappy Bird, Ардуино дино, Понг, Арнакоид, Space Invaders
- ⚙️ Системные настройки через веб-интерфейс
- 📶 Поддержка WiFi (STA и AP режимы)
- 📖 Файловый менеджер для LittleFS
- 🛠️ Сервисное меню с калибровкой
- 🧮 Встроенный калькулятор
- ⏱️ Секундомер и таймер

## Компоненты
- Микроконтроллер ESP32
- OLED дисплей 128x64 (SPI, 7 pins)
- 5 кнопок управления
- Литий-ионный аккумулятор

## Простой для DIY
1. Схема подключения

![scheme_CATOS](https://github.com/CatDevCode/CatOs/blob/main/assets/sheme_catos.png)

2. Схема питания

![scheme_bat_CATOS](https://github.com/CatDevCode/CatOs/blob/main/assets/bat.png)

> [!TIP]
> Резисторы на 100 kOm
## PCB
- Ссылка на проект [EasyEDA](https://oshwlab.com/oleggator2013/catos_catdevcode)
![PCB1](assets/pcb1.jpg)
![PCB2](assets/pcb2.jpg)
![PCB3](assets/pcb_with_components.jpg)
> [!WARNING]
> При прошивке или зарядке устройства отключите питание от батареи с помощью переключателя.
## Создание изображений и загрузка
1. Запустите [Сайт](https://alexgyver.github.io/Bitmaper2/index/) (веб версия инструкция не актуальна)

![IMG1](assets/img1.png)

2. Откройте изображение

![IMG2](assets/img2.png)

3. Настройте размер и порог изображения для получения лучшего результата

![IMG3](assets/img3.png)

4. Сделайте инверсию цвета (белый цвет будет отображаться на экране). И убедитесь что Result height и Result width стоят также как на изображении

![IMG4](assets/img4.png)

5. Сохраните файл нажав SAVE, в папке image-processor появится файл .h . Также можно переименовать этот файл.

![IMG5](assets/img5.png)

## Библиотеки
- [GyverOled](https://github.com/GyverLibs/GyverOLED/)
- [GyverButton(Старое, но работает отлично)](https://github.com/GyverLibs/GyverButton)
- [GyverTimer(Старое, но для совместимости)](https://github.com/GyverLibs/GyverTimer)
- [Settings](https://github.com/GyverLibs/Settings)
- [Random16](https://github.com/GyverLibs/Random16)
- PS. Все библиотеки от гайвера скоро появятся новые

## Установка
1. Установите [PlatformIO](https://platformio.org/)
```bash
pip install platformio
```
2. Клонируйте репозиторий:
```bash
git clone https://github.com/CatDevCode/CatOs.git
```
3. Перейдите в папку с проектом:
```bash
cd CatOs
```
4. Сбилдите проект
```bash
pio run
```
5. Загрузите проект на ESP32
```bash
pio run --target upload 
```
## Кредиты
- Спасибо [Алексу Гайверу](https://github.com/GyverLibs/) за библиотеки ❤
- Спасибо проекту [MicroReader](https://github.com/Nich1con/microReader/) за некоторые функции и игры.
- Спасибо [x4m](https://github.com/x4m) в [pull request](https://github.com/Nich1con/microReader/pull/11) за новый калькулятор
- Спасибо VoltimeterXPayalnuk за игру Space Invaders (для i2c).
- Спасибо HakerShark за иконки.
## Star History

<a href="https://www.star-history.com/#cubetitled-ui/CatOs-X&Date">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=CatDevCode/CatOs&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=CatDevCode/CatOs&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=CatDevCode/CatOs&type=Date" />
 </picture>
</a>

## Проект открыт для Pull-реквестов
## Сделано с любовью ❤







