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

#include "MSP100.h"

#if not USE_BITBANG
#include <SPI.h>
#endif

#if USE_BITBANG
MSP100::MSP100(uint8_t cs, uint8_t clk, uint8_t miso, uint16_t max_psi) {
    _cs = cs;
    _clk = clk;
    _miso = miso;
    _max_psi = max_psi;
}
#else
MSP100::MSP100(uint8_t cs, uint16_t max_psi) {
    _cs = cs;
    _max_psi = max_psi;
}
#endif

void MSP100::begin() {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
    #if USE_BITBANG
        pinMode(_clk, OUTPUT);
        pinMode(_miso, INPUT);
    #else
        SPI.begin();
    #endif
}

bool MSP100::read() {
    
    uint8_t data[4] = {0};

    #if USE_BITBANG

        digitalWrite(_clk, LOW);
        digitalWrite(_cs, LOW);
        delayMicroseconds(3);

        uint8_t bit;
        uint8_t i;
        data[3] = 0;
        for (i=0; i<3; i++) {
            
            for (bit = 0x80; bit; bit >>= 1) {

                // Read data
                if (digitalRead(_miso) == HIGH) {
                    data[i] |= bit;
                }
                
                // Clock transition
                digitalWrite(_clk, HIGH);
                delayMicroseconds(1);
                digitalWrite(_clk, LOW);

            }
        }
        
        digitalWrite(_cs, HIGH);
        delayMicroseconds(3);
    
    #else

        SPISettings config(100000, MSBFIRST, SPI_MODE1); 
        SPI.beginTransaction(config);
        digitalWrite(_cs, LOW);
        data[0] = SPI.transfer(0);
        data[1] = SPI.transfer(0);
        data[2] = SPI.transfer(0);
        data[3] = 0;
        digitalWrite(_cs, HIGH);
        SPI.endTransaction();

    #endif

    _status = data[0] >> 6;

    bool response = true;

    uint16_t _p_counts = ((data[0] & 0x3F) << 8) + data[1];
    if ((1000 < _p_counts) && (_p_counts < 15000)) {
        _pressure = _max_psi * (_p_counts - 1000.0) / 14000.0;
    } else {
        response = false;
        _pressure = 0;
    }

    uint16_t _t_counts = (data[2] << 3) + (data[3] >> 5);
    if ((512 < _t_counts) && (_t_counts < 1075)) {
        _temperature = (200.0 * _t_counts) / 2048.0 - 50.0;
    } else {
        response = false;
        _temperature = 0;
    }

    return response;

}