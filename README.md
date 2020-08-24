# Ogami-Itto

Before compiling, some edits are needed in the Audio Library ``(Program Files\Arduino\hardware\teensy\avr\libraries\Audio)``

   1. in **input_i2s.h** & **output_i2s.h** comment out the begin function, it will look like this: `{}//begin(); }`
   2. in **control_AK4588.h** change PIN_PDN to pin 17 `#define PIN_PDN 17`
   3. in **control_AK4588.cpp**, line 133 & 135, there is an hardcoded digitalWrite, change it to `PIN_PDN`
