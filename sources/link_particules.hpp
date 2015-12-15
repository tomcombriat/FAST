/*

Copyright 2014,2015 Thomas Combriat
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


*/

#ifndef _link_
#define _link_ 1
#include <vector>
#include "point.hpp"
#include "track.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"


double cost_function(double * current, double * expected);
bool in_area(double * current, double * expected, double radius); 
void link_particules(std::vector<Points> * & points,std::vector<Track> & tracks, double search_radius, int NB_frames);
void link_particules(std::vector<Points> * & points,std::vector<Track> & tracks, double search_radius, int NB_frames, int size_min, int size_max,unsigned int gap, unsigned int strategy,double flow_x, double flow_y);


#endif
