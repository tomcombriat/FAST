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



#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include "point.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "find_particules.hpp"
#include "link_particules.hpp"






using namespace std;
using namespace cv;




// /***********************************************
//  ************COST FUNCTION**********************
//  **********************************************/


/**************************************************
Cost functions compute the cost to link to different particles.
This function can be computed from different factors: the distance,
the size, the form factor etc…
Currently, the first version computes the cost from the distance only,
the second includes the size…
Finally these functions will be merged in order to have the same prototypes,
thus it will be easier to make this function evolve…

****************************************************/

double cost_function(double * current, double * expected)
{
  return (sqrt((current[0]-expected[0])*(current[0]-expected[0])+(current[1]-expected[1])*(current[1]-expected[1])));
}

double cost_function(double * expected, Points & candidate,int size)
{
  double classical_cost;
  classical_cost=(sqrt((expected[0]-candidate.center[0])*(expected[0]-candidate.center[0])+(expected[1]-candidate.center[1])*(expected[1]-candidate.center[1])));
  int size_cost=abs(size-candidate.area());
  return classical_cost+size_cost;

}


// /***********************************************
//  ************IN AREA****************************
//  **********************************************/

bool in_area(double * current, double * expected, double radius)
{
  if (sqrt((current[0]-expected[0])*(current[0]-expected[0])+(current[1]-expected[1])*(current[1]-expected[1]))<radius) return true;
  else return false;
}






void link_particules(vector<Points> * & points,vector<Track> & tracks,double search_radius,int NB_frames, int size_min, int size_max,unsigned int gap, unsigned int strategy, double flow_x, double flow_y,boost::archive::text_iarchive & ia,bool mode_low_ram,fstream & sortie)
{

  /*///////////////////////////////STRATEGY//////////////////////////////
    if 0 : neirest neighbor
    if 1 : linear prediction
    if 2 : quadratic predicition   (not implementated… (Yet?))
  */

 
  double expected_position[2];
  unsigned int frame=0;
  //initialisation: will assign every point in frame 0 to a new track
  for (unsigned int i=0;i<points[0].size();i++)
    {
      if (points[0].at(i).area()>=size_min && points[0].at(i).area()<=size_max)
	{
	  tracks.push_back(Track(points[0].at(i).center_position()[0],points[0].at(i).center_position()[1],0,points[0].at(i).area()));
	  points[0].at(i).track_index=i;
	}
    }


  
  for (unsigned int i=0;i<NB_frames-1;i++) // loop on all frames of the movie
    {
      
      if (mode_low_ram) ia >> points[i+1];  //Backup the archive 
   
      if (i%100==0)
	{
	  cout<<"\r"<<i<<"/"<<NB_frames;
	  fflush(stdout);
	}
      for (unsigned int j=0;j<tracks.size();j++)  //loop on ALL tracks (not so efficient…)
	{
	  // test(j);
	  if (tracks[j].Frame.back()<=i && tracks[j].Frame.back()>=i-gap)  //test if track is still open.
	    {
	      //Linking of particules already linked in the precedent frame
	      if (strategy==1)
		{
		  if (tracks[j].X.size()>1) //linear interpolation
		    {
		      int size=tracks[j].X.size();
		      expected_position[0]=(tracks[j].X.at(size-1)-tracks[j].X.at(size-2))/((tracks[j].Frame.at(size-1)-tracks[j].Frame.at(size-2)))+tracks[j].X.at(size-1)+flow_x;
		      expected_position[1]=(tracks[j].Y.at(size-1)-tracks[j].Y.at(size-2))/((tracks[j].Frame.at(size-1)-tracks[j].Frame.at(size-2)))+tracks[j].Y.at(size-1)+flow_y;

		    }
		  else  //track to short to compute the interpolation (2 points needed)
		    {
		      expected_position[0]=tracks[j].X.back()+flow_x;
		      expected_position[1]=tracks[j].Y.back()+flow_y;
		    }
		}
	      else if (strategy==0)
		{
		  expected_position[0]=tracks[j].X.back()+flow_x;
		  expected_position[1]=tracks[j].Y.back()+flow_y;
		}



		       
	      Points * candidate=NULL; //Windows compatibility…
	      double candidate_cost=search_radius*2;
	      for (unsigned int k=0;k<points[i+1].size();k++) //loop in particules in next frame
		{		  
		  if (points[i+1].at(k).track_index==-1 && points[i+1].at(k).area()>=size_min && points[i+1].at(k).area()<=size_max)  //test if particle is not attributed (track_index=-1) and its size 
		    {
		      double x_current=points[i+1].at(k).center_position()[0];
		      double y_current=points[i+1].at(k).center_position()[1];

		      if (in_area(points[i+1].at(k).center_position(),expected_position,search_radius))  //check if the particle is close enought to the expected position
			{
			  //if (cost_function(points[i+1].at(k).center_position(),expected_position)<=candidate_cost)
			  if (cost_function(expected_position,points[i+1].at(k),tracks[j].size_P.back())<=candidate_cost)
			    {  
			      candidate=&points[i+1].at(k);
			      //candidate_cost=cost_function(points[i+1].at(k).center_position(),expected_position);
			      candidate_cost=cost_function(expected_position,points[i+1].at(k),tracks[j].size_P.back());
			  
			    } //end if cost<
			}// end if in area
		    }//end if not already allocated
		}//end loop particules in next frame
	      if (candidate_cost<search_radius+1) //a match is found:
		{
		  //put the particle at the end of the track
		  tracks[j].X.push_back(candidate->center_position()[0]);
		  tracks[j].Y.push_back(candidate->center_position()[1]);
		  tracks[j].Frame.push_back(i+1);
		  tracks[j].size_P.push_back(candidate->area());
		  //give the number of the track to the particle (then it will not be attributed a second time…)
		  candidate->track_index=j;
		}  //end of if there is a match	      
	    }  //end of if on particles who are between N and N-gap
	} //end loop on tracks of frame N
      

       
      for (unsigned int j=0;j<points[i].size();j++)  //loop on particules not being attributed on a tracks previously
	{
	  // test(j);
	  if (points[i].at(j).track_index==-1 && points[i].at(j).area()>=size_min && points[i].at(j).area()<=size_max)
	    {
	      expected_position[0]=points[i].at(j).center_position()[0]+flow_x;
	      expected_position[1]=points[i].at(j).center_position()[1]+flow_y;
	      Points * candidate=NULL;
	      double candidate_cost=search_radius*2;

	      for (unsigned int k=0;k<points[i+1].size();k++) //loop in particules in next frame
		{
		  //cout<<"k="<<k<<endl; 
		  if (points[i+1].at(k).track_index==-1 && points[i+1].at(k).area()>=size_min && points[i+1].at(k).area()<=size_max)
		    {
		      double x_current=points[i+1].at(k).center_position()[0];
		      double y_current=points[i+1].at(k).center_position()[1];
		      
		      //cout<<in_area(points[i+1].at(k).center_position(),expected_position,search_radius)<<endl;
		      if (in_area(points[i+1].at(k).center_position(),expected_position,search_radius))  //put the cost function here ?
			{
			 
			  //if (cost_function(points[i+1].at(k).center,expected_position)<=candidate_cost)
			  if (cost_function(expected_position,points[i+1].at(k),points[i].at(j).area())<=candidate_cost)
			    {
			     
			      candidate=&points[i+1].at(k);
			      //candidate_cost=cost_function(points[i+1].at(k).center_position(),expected_position);
			      candidate_cost=cost_function(expected_position,points[i+1].at(k),points[i].at(j).area());
			      
			    } //end if cost<
			}// end if in area
		    }//end if not already allocated
		}//end loop particules in next frame
	      if (candidate_cost<search_radius+1) //put the two particle in a new track
		{
		  tracks.push_back(Track(points[i].at(j).center_position()[0],points[i].at(j).center_position()[1],i,points[i].at(j).area()));
		  tracks.back().add_point(candidate->center_position()[0],candidate->center_position()[1],i+1,candidate->area());
		  candidate->track_index=tracks.size();
	      
		}
	    } //not already attributed
	} //not previously attributed
      vector<Points> tamp_vec;
      points[i].swap(tamp_vec); //free memory
    } //loop on frames
} //end of function


