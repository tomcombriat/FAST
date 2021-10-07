Copyright 2020,2019 Thomas Combriat
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


Welcome to FAST (Fast and Simple Tracker)! FAST is a particle tracker developped in order to performe Particle tracking Velocimetry (PTV) developped by T. Combriat, PhD student at the LIPhy lab, Grenoble, France.


///////////////
DEP ON DEBIAN 10
///////////////
libopencv-dev 
libboost-dev
libboost-serialization-dev

//////////////
HOW TO INSTALL ON LINUX
/////////////
FAST uses cmake and opencv, after installing these softwares you can compile the program:

cmake .
make

in a command prompt.


//////////////
    LAUNCH
/////////////

usage:

./FAST input_video output_txt_file -supp_arg

-supp_arg can be "-test" or "-low-ram". Executed with -test, the program will show you which particles it found for each step, thus slowering the program.


///////////////
PUBLICATIONS
///////////////
[TODO]


