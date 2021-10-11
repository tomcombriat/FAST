Copyright 2021,2020,2019 Thomas Combriat
   This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

   This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


Welcome to FAST (Fast and Simple Tracker)! FAST is a particle tracker developped in order to perform efficient Particle tracking Velocimetry (PTV).


# Installation on GNU/Linux

## Dependencies
In order to compile FAST, the following packages are needed:

 - libopencv-dev 
 - libboost-dev
 - libboost-serialization-dev
 - cmake
 
 They can be installed on a Debian based distribution using:
 `sudo apt install libopencv-dev libboost-dev libboost-serialization-dev cmake`

## Compilation

FAST uses cmake and opencv, after installing these softwares you can compile the program by typing in a command prompt from the FAST main folder:

`cmake .`
`make`


# Usage

./FAST input_video output_txt_file -supp_arg

-supp_arg can be `-test`, `-low-ram`, `-no-BG` or `-inv`

 - `-test` allows you to visualize the detection step performed by FAST. This is slower but can be useful for initial testings
 - `-low-ram` uses serialization to limit the amount of memory needed to process the movie: each frame is perform sequentially and written to the HD instead of being written into RAM. This is slightly slower but sometimes needed if the movie is very big
 - `-no-BG` can be used in order to tell FAST not to substract the average of all frames to the movie. This can be useful if the movie is well contrasted or if the particles of interest are not moving much.
 - `-inv` should be precised in order to track bright particles. But default FAST will track black particles




# Publications
FAST has been used in the following publications:

 - Bertin, N., Spelman, T. A., Combriat, T., Hue, H., Stéphan, O., Lauga, E., & Marmottant, P. (2017). Bubble-based acoustic micropropulsors: active surfaces and mixers. Lab on a Chip, 17(8), 1515-1528.
 - Busek, M., Nøvik, S., Aizenshtadt, A., Amirola-Martinez, M., Combriat, T., Grünzner, S., & Krauss, S. (2021). Thermoplastic Elastomer (TPE)–Poly (Methyl Methacrylate)(PMMA) Hybrid Devices for Active Pumping PDMS-Free Organ-on-a-Chip Systems. Biosensors, 11(5), 162.
 - Combriat, T., Mekki-Berrada, F., Thibault, P., & Marmottant, P. (2018). Trapping and exclusion zones in complex streaming patterns around a large assembly of microfluidic bubbles under ultrasound. Physical Review Fluids, 3(1), 013602.
 - Spelman, T. A., Lauga, E. J. M., Hue, H., Combriat, T., Bertin, N., Stéphan, O., & Marmottant, P. (2017). Bubble-based acoustic micropropulsors: active surfaces and mixers.
 - Spelman, T. A., Stephan, O., & Marmottant, P. (2020). Multi-directional bubble generated streaming flows. Ultrasonics, 102, 106054.
