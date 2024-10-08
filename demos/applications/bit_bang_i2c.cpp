// Copyright 2024 Khalil Estell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libhal-soft/bit_bang_i2c.hpp>
#include <libhal-stm-imu/lis3dhtr_i2c.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal-util/units.hpp>

#include "../hardware_map.hpp"

void application(hardware_map_t& p_map)
{
  using namespace std::chrono_literals;
  using namespace hal::literals;

  auto& clock = *p_map.clock;
  auto& console = *p_map.console;
  auto& scl = *p_map.scl;
  auto& sda = *p_map.sda;

  hal::print(
    console,
    "Starting bit bang i2c Application using a lis3dhtr_i2c driver...\n");
  hal::delay(clock, 50ms);

  hal::soft::bit_bang_i2c bit_bang_i2c({ .sda = &sda, .scl = &scl }, clock);
  bit_bang_i2c.configure(hal::i2c::settings{ .clock_rate = 100.0_kHz });

  hal::stm_imu::lis3dhtr_i2c accelerometer(bit_bang_i2c);

  while (true) {
    hal::delay(clock, 500ms);
    auto const acceleration = accelerometer.read();
    hal::print<128>(console,
                    "Scale: 2g \t x = %fg, y = %fg, z = %fg \n",
                    acceleration.x,
                    acceleration.y,
                    acceleration.z);
  }
}
