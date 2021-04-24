//
//  FoodTag_5_Component.ino
//
//  Created by Erik Coltey on 4/20/21.
//  Copyright 2021 Erik Coltey. All rights reserved.
//

#include <stdint.h>

int32_t prev_max_val = 0;
int32_t max_val = 0;
#define NUM_SAMPLES 64

// the setup function runs once when you press reset or power the board
void setup() {
  // Display envelope detection results on to computer
  Serial.begin(9600);
  Serial.println("Min:0,Max:5000");
}

// Rudimentary envelope detector due to irregularities in power input for transmitter.
void loop() {

  // Read 64 samples at once.
  for (int32_t i = 0; i < NUM_SAMPLES; ++i)
  {
    int32_t val = analogRead(A1);  // read the input pin
    delay(1);
    
    if (val > max_val) max_val = val;
  }

  // Interpolate from previous to current max value, and then display.
  for (int32_t i = 0; i < NUM_SAMPLES; ++i)
  {
    Serial.println( (prev_max_val * NUM_SAMPLES) + (i * (max_val - prev_max_val)) );
  }
  prev_max_val = max_val;
  max_val = 0;
}
