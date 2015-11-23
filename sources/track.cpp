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


#include<iostream>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include<vector>
#include"point.hpp"
#include"track.hpp"
#include"find_particules.hpp"

using namespace std;


Track::Track(double x, double y, int frame)
{
  X.push_back(x);
  Y.push_back(y);
  Frame.push_back(frame);
  lenght=1;
  
}


void Track::add_point(double x, double y, int frame)
{
  X.push_back(x);
  Y.push_back(y);
  Frame.push_back(frame);
  lenght++;
}


Track::Track(double x, double y, int frame,int size)
{
  X.push_back(x);
  Y.push_back(y);
  Frame.push_back(frame);
  lenght=1;
  size_P.push_back(size);
  
}


void Track::add_point(double x, double y, int frame,int size)
{
  X.push_back(x);
  Y.push_back(y);
  Frame.push_back(frame);
    size_P.push_back(size);
  lenght++;
}


int Track::get_lenght()
{
  lenght=X.size();
  return lenght;
}
Track::~Track() {}
