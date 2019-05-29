/*

  TE Connectivity MSP100 Pressure Transducer Library for Arduino
  Copyright (C) 2019 by AllWize
  Copyright (C) 2019 by Xose Pérez <xose at espurna dot io>

  Basic usage example

  The MSP100 library is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The MSP100 library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with the MSP100 library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "MSP100.h"

#ifdef ARDUINO_ARCH_SAMD
#define DEBUG_SERIAL    SerialUSB
#else
#define DEBUG_SERIAL    Serial
#endif

#define MSP100_CS       10
#define MSP100_MAX_PSI  250

MSP100 msp100(MSP100_CS, MSP100_MAX_PSI);

// ----------------------------------------------------------------------------

void setup() {
    
    // Init DEBUG_SERIAL link to computer
    DEBUG_SERIAL.begin(115200);
    while (!DEBUG_SERIAL && millis() < 5000);
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println("MSP100 example");
    DEBUG_SERIAL.println();

    // Init sensor communications
    msp100.begin();

}

void loop() {

    if (msp100.read()) {
        DEBUG_SERIAL.print("MSP100 status: ");
        DEBUG_SERIAL.println(msp100.getStatus());
        DEBUG_SERIAL.print("MSP100 pressure: ");
        DEBUG_SERIAL.println(msp100.getPressure());
        DEBUG_SERIAL.print("MSP100 temperature: ");
        DEBUG_SERIAL.println(msp100.getTemperature());
    } else {
        DEBUG_SERIAL.print("Error reading sensor");
    }
    DEBUG_SERIAL.println();

    delay(5000);

}
