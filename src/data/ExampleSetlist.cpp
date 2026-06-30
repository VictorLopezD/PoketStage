#include "data/ExampleSetlist.h"

const Song kSongs[] = {
  {
    "Living After Midnight",
    {"E Standard", {"E", "A", "D", "G", "", ""}, {"Compressor", "Overdrive", "", ""}},
    {"E Standard", {"E", "B", "G", "D", "A", "E"}, {"Overdrive", "Delay", "", ""}}
  },
  {
    "No One Like You",
    {"E Standard", {"E", "A", "D", "G", "", ""}, {"Compressor", "Overdrive", "", ""}},
    {"E Standard", {"E", "B", "G", "D", "A", "E"}, {"Overdrive", "Reverb", "", ""}}
  },
  {
    "Iron Man",
    {"E Standard", {"E", "A", "D", "G", "", ""}, {"Compressor", "Fuzz/Overdrive", "", ""}},
    {"E Standard", {"E", "B", "G", "D", "A", "E"}, {"Distortion", "Compressor", "", ""}}
  },
  {
    "Wasted Years",
    {"E Standard", {"E", "A", "D", "G", "", ""}, {"Compressor", "Delay ligero", "", ""}},
    {"E Standard", {"E", "B", "G", "D", "A", "E"}, {"Modulation", "Delay", "", ""}}
  },
  {
    "Wasting Love",
    {"E Standard", {"E", "A", "D", "G", "", ""}, {"Compressor", "Modulation", "", ""}},
    {"E Standard", {"E", "B", "G", "D", "A", "E"}, {"Compressor", "Reverb", "", ""}}
  },
  {
    "Symphony of Destruction",
    {"E Standard", {"E", "A", "D", "G", "", ""}, {"Compressor", "Overdrive", "", ""}},
    {"Drop D", {"D", "A", "D", "G", "B", "E"}, {"Overdrive", "Noise Gate", "", ""}}
  },
  {
    "Would?",
    {"D# Standard", {"D#", "G#", "C#", "F#", "", ""}, {"Compressor", "Reverb", "", ""}},
    {"D# Standard", {"D#", "A#", "F#", "C#", "G#", "D#"}, {"Compressor", "Reverb", "", ""}}
  },
  {
    "Hail to the King",
    {"C# Standard", {"C#", "G#", "C#", "F#", "", ""}, {"Compressor", "Delay", "", ""}},
    {"C# Standard", {"C#", "A#", "F#", "C#", "G#", "D#"}, {"Distortion", "Delay", "", ""}}
  }
};

const size_t kSongCount = sizeof(kSongs) / sizeof(kSongs[0]);
