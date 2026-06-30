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

enum AppState {
  APP_MENU = 0,
  APP_PLAYLIST,
  APP_SETTINGS
};

enum Instrument {
  INSTRUMENT_BASS = 0,
  INSTRUMENT_GUITAR
};

const char* modeLabel(ScreenMode mode) {
  switch (mode) {
    case SCREEN_MAIN: return "Main";
    case SCREEN_ALT: return "Alt";
    default: return "Main";
  }
}

const char* instrumentLabel(Instrument instrument) {
  switch (instrument) {
    case INSTRUMENT_BASS: return "Bass";
    case INSTRUMENT_GUITAR: return "Guitar";
    default: return "Bass";
  }
}

String joinTuning(const SongVariant& variant) {
  String result;
  for (int i = 0; i < 6; ++i) {
    if (variant.tuning[i].length() == 0) {
      continue;
    }
    if (result.length() > 0) {
      result += " ";
    }
    result += variant.tuning[i];
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

const SongVariant& getActiveVariant(const Song& song, Instrument instrument) {
  return instrument == INSTRUMENT_GUITAR ? song.guitar : song.bass;
}

void renderSong(const Song& song, ScreenMode mode, uint8_t songIndex, uint8_t totalSongs, Instrument instrument) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(instrumentLabel(instrument));
  display.setCursor(100, 0);
  display.print(modeLabel(mode));
  display.drawFastHLine(0, 10, 128, SSD1306_WHITE);

  const SongVariant& variant = getActiveVariant(song, instrument);
  String titleText = fitTextToWidth(song.title, 118);
  String tuningText = fitTextToWidth(joinTuning(variant), 118);
  String pedalsText = fitTextToWidth(variant.pedals[0], 118);
  String pedalsTextAlt = fitTextToWidth(variant.pedals[1], 118);

  if (mode == SCREEN_MAIN) {
    display.setCursor(0, 14);
    display.print("Song:");
    display.setCursor(42, 14);
    display.print(formatSongIndex(songIndex, totalSongs).c_str());
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
    display.print(formatSongIndex(songIndex, totalSongs).c_str());
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

void renderMenu(uint8_t selectedItem, Instrument instrument) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Menu");
  display.drawFastHLine(0, 10, 128, SSD1306_WHITE);

  const char* labels[4] = {"Playlist", "Settings", "View", "Back"};
  const uint8_t xPositions[2] = {8, 68};
  const uint8_t yPositions[2] = {20, 40};

  for (uint8_t i = 0; i < 4; ++i) {
    const uint8_t col = i % 2;
    const uint8_t row = i / 2;
    const uint8_t x = xPositions[col];
    const uint8_t y = yPositions[row];

    display.setCursor(x, y);
    display.print(selectedItem == i ? ">" : " ");
    display.print(labels[i]);
  }

  display.setCursor(0, 56);
  display.print("Inst:");
  display.setCursor(28, 56);
  display.print(instrumentLabel(instrument));

  display.display();
}

void renderSettings(Instrument instrument, uint8_t selectedItem, ScreenMode currentView) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Settings");
  display.drawFastHLine(0, 10, 128, SSD1306_WHITE);

  const char* labels[4] = {"Inst", "View", "Reset", "Back"};
  const uint8_t xPositions[2] = {8, 68};
  const uint8_t yPositions[2] = {20, 40};

  for (uint8_t i = 0; i < 4; ++i) {
    const uint8_t col = i % 2;
    const uint8_t row = i / 2;
    const uint8_t x = xPositions[col];
    const uint8_t y = yPositions[row];

    display.setCursor(x, y);
    display.print(selectedItem == i ? ">" : " ");
    display.print(labels[i]);

    if (i == 0) {
      display.setCursor(x + 24, y);
      display.print(instrumentLabel(instrument));
    } else if (i == 1) {
      display.setCursor(x + 24, y);
      display.print(currentView == SCREEN_MAIN ? "Main" : "Alt");
    } else if (i == 2) {
      display.setCursor(x + 24, y);
      display.print("Song");
    }
  }

  display.setCursor(0, 56);
  display.print("L/R move  C select");

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
  static AppState appState = APP_MENU;
  static uint8_t selectedMenuItem = 0;
  static uint8_t selectedSettingsCell = 0;
  static Instrument selectedInstrument = INSTRUMENT_BASS;
  static uint8_t currentSong = 0;
  static ScreenMode currentView = SCREEN_MAIN;
  static unsigned long lastInput = 0;
  static bool lastLeft = false;
  static bool lastCenter = false;
  static bool lastRight = false;
  static unsigned long centerPressStartMs = 0;
  static bool centerLongPressActive = false;

  size_t activeSongCount = kSongCount;

  if (currentSong >= activeSongCount) {
    currentSong = 0;
  }

  if (millis() - lastInput > 180) {
    bool left = digitalRead(BTN_LEFT) == LOW;
    bool center = digitalRead(BTN_CENTER) == LOW;
    bool right = digitalRead(BTN_RIGHT) == LOW;

    if (appState == APP_MENU) {
      if (left && !lastLeft) {
        selectedMenuItem = (selectedMenuItem + 3) % 4;
        lastInput = millis();
      } else if (right && !lastRight) {
        selectedMenuItem = (selectedMenuItem + 1) % 4;
        lastInput = millis();
      } else if (center && !lastCenter) {
        if (selectedMenuItem == 0) {
          appState = APP_PLAYLIST;
          currentSong = 0;
          currentView = SCREEN_MAIN;
        } else if (selectedMenuItem == 1) {
          appState = APP_SETTINGS;
          selectedSettingsCell = 0;
        } else if (selectedMenuItem == 2) {
          currentView = static_cast<ScreenMode>((currentView + 1) % 2);
        }
        lastInput = millis();
      }
    } else if (appState == APP_PLAYLIST) {
      if (left && !lastLeft) {
        currentSong = (currentSong + activeSongCount - 1) % activeSongCount;
        centerLongPressActive = false;
        centerPressStartMs = 0;
        lastInput = millis();
      } else if (right && !lastRight) {
        currentSong = (currentSong + 1) % activeSongCount;
        centerLongPressActive = false;
        centerPressStartMs = 0;
        lastInput = millis();
      } else if (center && !lastCenter) {
        centerPressStartMs = millis();
        centerLongPressActive = true;
        lastInput = millis();
      } else if (center && centerLongPressActive && millis() - centerPressStartMs >= 2000) {
        appState = APP_MENU;
        selectedMenuItem = 0;
        centerLongPressActive = false;
        centerPressStartMs = 0;
        lastInput = millis();
      } else if (!center && centerLongPressActive) {
        if (millis() - centerPressStartMs < 2000) {
          currentView = static_cast<ScreenMode>((currentView + 1) % 2);
        }
        centerLongPressActive = false;
        centerPressStartMs = 0;
      }
    } else if (appState == APP_SETTINGS) {
      if (left && !lastLeft) {
        selectedSettingsCell = (selectedSettingsCell + 3) % 4;
        lastInput = millis();
      } else if (right && !lastRight) {
        selectedSettingsCell = (selectedSettingsCell + 1) % 4;
        lastInput = millis();
      } else if (center && !lastCenter) {
        if (selectedSettingsCell == 0) {
          selectedInstrument = static_cast<Instrument>((selectedInstrument + 1) % 2);
        } else if (selectedSettingsCell == 1) {
          currentView = static_cast<ScreenMode>((currentView + 1) % 2);
        } else if (selectedSettingsCell == 2) {
          currentSong = 0;
        } else {
          appState = APP_MENU;
        }
        lastInput = millis();
      }
    }

    lastLeft = left;
    lastCenter = center;
    lastRight = right;
  }

  if (appState == APP_MENU) {
    renderMenu(selectedMenuItem, selectedInstrument);
  } else if (appState == APP_PLAYLIST) {
    renderSong(kSongs[currentSong], currentView, currentSong, static_cast<uint8_t>(activeSongCount), selectedInstrument);
  } else {
    renderSettings(selectedInstrument, selectedSettingsCell, currentView);
  }

  delay(100);
}