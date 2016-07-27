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

#ifndef _track_
#define _track_ 1
#include<vector>

class Track
{
public:

  Track(double x, double y, int frame);
  Track(double x, double y, int frame,int size);
  void add_point(double x, double y, int frame);
  void add_point(double x, double y, int frame,int size);
  std::vector<double> X,Y;
  std::vector<int> Frame,size_P;
  int lenght;
  int get_lenght();
  
  void clear();
  
};




#endif
