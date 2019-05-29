/*

  TE Connectivity MSP100 Pressure Transducer Library for Arduino
  Copyright (C) 2019 by AllWize
  Copyright (C) 2019 by Xose Pérez <xose at espurna dot io>

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

#ifndef MSP100_h
#define MSP100_h

#include "Arduino.h"

#define USE_BITBANG 0

class MSP100 {

    public:

        #if USE_BITBANG
            MSP100(uint8_t cs, uint8_t clk, uint8_t miso, uint16_t max_psi);
        #else
            MSP100(uint8_t cs, uint16_t max_psi);
        #endif

        void begin();
        bool read();
        uint8_t getStatus() { return _status; }
        float getPressure() { return _pressure; }
        float getTemperature() { return _temperature; }

    protected:

        uint8_t _cs;
        uint16_t _max_psi = 250;
        uint8_t _status;
        float _pressure;
        float _temperature;

};

#endif
