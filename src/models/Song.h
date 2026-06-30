#pragma once

#include <Arduino.h>

struct SongVariant {
  String tuningName;
  String tuning[6];
  String pedals[4];
};

struct Song {
  String title;
  SongVariant bass;
  SongVariant guitar;
};
