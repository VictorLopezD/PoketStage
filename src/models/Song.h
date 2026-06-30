#pragma once

#include <Arduino.h>

struct Song {
  String title;
  String tuningName;
  String tuning[6];
  String pedals[4];
};
