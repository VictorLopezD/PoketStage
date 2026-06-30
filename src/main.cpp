#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "data/ExampleSetlist.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
#define OLED_SDA 8
#define OLED_SCL 9

#define BTN_LEFT 0
#define BTN_CENTER 1
#define BTN_RIGHT 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum ScreenMode {
  SCREEN_MAIN = 0,
  SCREEN_ALT
};

const char* modeLabel(ScreenMode mode) {
  switch (mode) {
    case SCREEN_MAIN: return "Main";
    case SCREEN_ALT: return "Alt";
    default: return "Main";
  }
}

String joinTuning(const Song& song) {
  String result;
  for (int i = 0; i < 6; ++i) {
    if (song.tuning[i].length() == 0) {
      continue;
    }
    if (result.length() > 0) {
      result += " ";
    }
    result += song.tuning[i];
  }
  return result;
}

String fitTextToWidth(const String& text, int maxWidth) {
  if (text.length() == 0) {
    return String();
  }

  String candidate = text;
  int16_t x1 = 0;
  int16_t y1 = 0;
  uint16_t w = 0;
  uint16_t h = 0;

  display.getTextBounds(candidate, 0, 0, &x1, &y1, &w, &h);
  if (w <= maxWidth) {
    return candidate;
  }

  while (candidate.length() > 0) {
    candidate.remove(candidate.length() - 1);
    display.getTextBounds(candidate, 0, 0, &x1, &y1, &w, &h);
    if (w <= maxWidth) {
      return candidate;
    }
  }

  return String();
}

String formatSongIndex(uint8_t songIndex, uint8_t totalSongs) {
  String value;
  if (songIndex + 1 < 10) {
    value += "0";
  }
  value += String(songIndex + 1);
  value += "/";
  if (totalSongs < 10) {
    value += "0";
  }
  value += String(totalSongs);
  return value;
}

void renderSong(const Song& song, ScreenMode mode, uint8_t songIndex) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("PoketStage");
  display.setCursor(86, 0);
  display.print(modeLabel(mode));
  display.drawFastHLine(0, 10, 128, SSD1306_WHITE);

  String titleText = fitTextToWidth(song.title, 118);
  String tuningText = fitTextToWidth(joinTuning(song), 118);
  String pedalsText = fitTextToWidth(song.pedals[0], 118);
  String pedalsTextAlt = fitTextToWidth(song.pedals[1], 118);

  if (mode == SCREEN_MAIN) {
    display.setCursor(0, 14);
    display.print("Song:");
    display.setCursor(42, 14);
    display.print(formatSongIndex(songIndex, kSongCount).c_str());
    display.setCursor(0, 24);
    display.print(titleText);
    display.setCursor(0, 36);
    display.print("Tuning:");
    display.setCursor(0, 46);
    display.print(tuningText);
    display.setCursor(0, 56);
    display.print("Pedals:");
    display.setCursor(40, 56);
    display.print(pedalsText);
  } else {
    display.setCursor(0, 14);
    display.print("Song:");
    display.setCursor(42, 14);
    display.print(formatSongIndex(songIndex, kSongCount).c_str());
    display.setCursor(0, 24);
    display.print(titleText);
    display.setCursor(0, 36);
    display.print("Pedals:");
    display.setCursor(0, 46);
    display.print(pedalsText);
    display.setCursor(0, 56);
    display.print(pedalsTextAlt);
  }

  display.display();
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_CENTER, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    for (;;) {
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 24);
  display.print("PoketStage booting...");
  display.display();
  delay(1000);
}

void loop() {
  static uint8_t currentSong = 0;
  static uint8_t currentView = SCREEN_MAIN;
  static unsigned long lastInput = 0;
  static bool lastLeft = false;
  static bool lastCenter = false;
  static bool lastRight = false;

  if (millis() - lastInput > 180) {
    bool left = digitalRead(BTN_LEFT) == LOW;
    bool center = digitalRead(BTN_CENTER) == LOW;
    bool right = digitalRead(BTN_RIGHT) == LOW;

    if (left && !lastLeft) {
      currentSong = (currentSong + kSongCount - 1) % kSongCount;
      lastInput = millis();
    } else if (right && !lastRight) {
      currentSong = (currentSong + 1) % kSongCount;
      lastInput = millis();
    } else if (center && !lastCenter) {
      currentView = (currentView + 1) % 2;
      lastInput = millis();
    }

    lastLeft = left;
    lastCenter = center;
    lastRight = right;
  }

  renderSong(kSongs[currentSong], static_cast<ScreenMode>(currentView), currentSong);
  delay(100);
}