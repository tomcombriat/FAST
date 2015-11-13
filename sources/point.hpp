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

#ifndef _points_
#define _points_ 1
#include<vector>

class Points
{
public:
  Points(int _x, int _y);
  Points();
  std::vector<double> x_pixels;
  std::vector<double> y_pixels;
  void add_pixel(int x, int y);
  double * center_position();
  double * center;
  double area();
  int track_index;
};

double  points_mean(std::vector<Points> &  points);
double points_std(std::vector<Points> & points);

#endif
