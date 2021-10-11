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






