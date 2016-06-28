/*

Copyright 2014,2015,2016 Thomas Combriat
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
  ~Points();
  std::vector<unsigned int> x_pixels;
  std::vector<unsigned int> y_pixels;
  void add_pixel(unsigned int x,unsigned int y);
  double * center_position();
  double * center;
  double area();
  int track_index;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & x_pixels;
    ar & y_pixels;
    ar & center[0];
    ar & center[1];
  }
};

double  points_mean(std::vector<Points> &  points);
double points_std(std::vector<Points> & points);

#endif
