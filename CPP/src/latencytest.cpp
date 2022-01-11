/*
 * Copyright (C) 2022 Claudio Cambra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <chrono>
#include "clothsimulation.h"

int main(int argc, char **argv) {
    ClothSimulation simulation;

    auto start = std::chrono::high_resolution_clock::now();

    auto upd_pos_start = std::chrono::high_resolution_clock::now();
    simulation.update_positions();
    auto upd_pos_stop = std::chrono::high_resolution_clock::now();
    auto upd_pos_duration = std::chrono::duration_cast<std::chrono::milliseconds>(upd_pos_stop - upd_pos_start);
    std::cout << "Time taken by estimating positions: " << upd_pos_duration.count() << " milliseconds" << std::endl;

    auto val_pos_start = std::chrono::high_resolution_clock::now();
    simulation.validate_positions();
    auto val_pos_stop = std::chrono::high_resolution_clock::now();
    auto val_pos_duration = std::chrono::duration_cast<std::chrono::milliseconds>(val_pos_stop - val_pos_start);
    std::cout << "Time taken by validating and correcting positions: " << val_pos_duration.count() << " milliseconds" << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "Time taken by single cycle: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}
