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


#include<iostream>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include<vector>
#include"point.hpp"

using namespace std;

Points::Points(int _x, int _y)
{
  center=new double [2];
  center[0]=-1;
  center[1]=-1;
  x_pixels.push_back(_x);
  y_pixels.push_back(_y);
  track_index=-1;
}
Points::Points()
{
  center=new double [2];
  center[0]=-1;
  center[1]=-1;
  track_index=-1;
}

void Points::add_pixel(unsigned int _x,unsigned int _y)
{
  x_pixels.push_back(_x);
  y_pixels.push_back(_y);
  //Points::center_position();
}

double * Points::center_position()
{
  if (center[0]==-1)
    {
    
  double tamp_x=0,tamp_y=0;
  for (int i=0;i<x_pixels.size();i++)
    {
      tamp_x=(x_pixels[i]+tamp_x*(i))/(i+1)*1.;
      // cout<<x_pixels[i]<<" ";
      tamp_y=(y_pixels[i]+tamp_y*(i))/(i+1)*1.;
    }
  center[0]=tamp_x;
  center[1]=tamp_y;
  
  // cout<<"Size : "<<x_pixels.size()<<" x "<<center[0]<<" y "<<center[1]<<endl;
    }
  return center;
}

double Points::area()
{
  return x_pixels.size();
}


Points::~Points()
{
  // x_pixels.~vector();
  //y_pixels.~vector();
  //delete center;
}



double  points_mean(vector<Points> & points)
{
  double mean=0;
 unsigned int size=points.size();
  for (unsigned int i=0;i<size;i++)
    {
      mean+=points.at(i).area();
    }
  mean=mean/size*1.;
  return mean;
}


double points_std(vector<Points> & points)
{
   unsigned int size=points.size();
  double std=0;
  double mean=points_mean(points);
  double sum=0;
    for (unsigned int i=0;i<size;i++)
      {
	sum+=points.at(i).area()*points.at(i).area();
      }
    //    cout<<"Sum/n : "<<sum/size-mean*mean<<endl;
    std=sqrt(sum/size-mean*mean);
    return std;
}



