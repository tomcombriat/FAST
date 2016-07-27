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
#include "point.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "find_particules.hpp"



using namespace std;
using namespace cv;


// /***********************************************
//  ************COST FUNCTION**********************
//  **********************************************/

/*
  double cost_function(double * current, double * expected)
  {
  return (sqrt((current[0]-expected[0])*(current[0]-expected[0])+(current[1]-expected[1])*(current[1]-expected[1])));
  }
*/

// /***********************************************
//  ************IN AREA****************************
//  **********************************************/
/*
  bool in_area(double * current, double * expected, double radius)
  {
  if (sqrt((current[0]-expected[0])*(current[0]-expected[0])+(current[1]-expected[1])*(current[1]-expected[1]))<radius) return true;
  else return false;
  }
*/




/**************************************************
 ***************FIND PARTICULES********************
 *************************************************/


void find_particules(cv::Mat & input, int frame, vector <Points> & points)
{
  
  unsigned int count=0;
  uint8_t *myData = input.data;
  int width = input.cols;
  int height = input.rows;
  int _stride = input.step;//in case cols != strides
  float* ptr = (float*) input.data;
   size_t elem_step = input.step / sizeof(float);
  // size_t elem_step = input.step / 9;
  for(int i = 0; i < height; i++)
    {
      //   cout<<"i="<<i<<"/"<<height<<endl;
      for(int j = 0; j < width; j++)
	{
	  // cout<<"j="<<j<<endl;
	  float val = ptr[i * elem_step + j];

	  if (val!=0)
	    {
	      //  cout<<"Particles found at position : "<<i<<"  "<<j<<endl;
	      int partial_count=1;
	      int current_partial=0;
	      //   cout<<"P1 "<<points.size()<<"   ";
	      points.push_back(Points(i,j));
	      // cout<<"P2 "<<points.size()<<endl;
	      ptr[i*elem_step+j]=0;
	      // cout<<"P3 "<<points.size()<<endl;
	      
	      unsigned int _i=i, _j=j;
	      while(current_partial<partial_count)
		{

		  //	  cout<<partial_count<<"   count="<<count<<"   current_partial="<<current_partial<<"  partial count="<<partial_count<<"    point size="<<points.size()<<endl;
		  //	  test(0);
		  _i=points[count].x_pixels.at(current_partial);
		  _j=points[count].y_pixels.at(current_partial);
		  //	  test(1);
		  
		  if (_i<height-1 && _j < width-1 && _j>1)
		    {
		      if (ptr[(_i) * elem_step + _j+1]!=0)
			{
			  points[count].add_pixel(_i,_j+1);
			  ptr[(_i) * elem_step + _j+1]=0;
			  partial_count++;
			}

		      if (ptr[(_i+1) * elem_step + _j-1]!=0)
			{
			  points[count].add_pixel(_i+1,_j-1);
			  ptr[(_i+1) * elem_step + _j-1]=0;
			  partial_count++;
			}

		      if (ptr[(_i+1) * elem_step + _j]!=0)
			{
			  //cout<<"salut3"<<endl;
			  points[count].add_pixel(_i+1,_j);
			  ptr[(_i+1) * elem_step + _j]=0;
			  partial_count++;
			}
	      
		      if (ptr[(_i+1) * elem_step + _j+1]!=0)
			{
			  //cout<<"salut4"<<endl;
			  points[count].add_pixel(_i+1,_j+1);
			  ptr[(_i+1) * elem_step + _j+1]=0;
			  partial_count++;
			}
		    }
		  current_partial++;		  
		}// end of while	      
	      count++;
	    }
	}
    }
}




int test(int i)
{
  cout<<"test "<<i<<endl;
  return 0;
}









