#include <Arduino.h>
#include "Game.h"

void setup() {
    Serial.begin(115200);
}

void loop() {
    game.play();
}
