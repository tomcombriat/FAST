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
#include "link_particules.hpp"






using namespace std;
using namespace cv;




// /***********************************************
//  ************COST FUNCTION**********************
//  **********************************************/


double cost_function(double * current, double * expected)
{
  return (sqrt((current[0]-expected[0])*(current[0]-expected[0])+(current[1]-expected[1])*(current[1]-expected[1])));
}


// /***********************************************
//  ************IN AREA****************************
//  **********************************************/

bool in_area(double * current, double * expected, double radius)
{
  if (sqrt((current[0]-expected[0])*(current[0]-expected[0])+(current[1]-expected[1])*(current[1]-expected[1]))<radius) return true;
  else return false;
}

/*
  double cost_function(int * current, double * expected)
  {
  return sqrt((current[0]-expected[0])*(current[0]-expected[0])+(current[1]-expected[1])*(current[1]-expected[1]));
  }
*/

// /***********************************************
//  ************IN AREA****************************
//  **********************************************/
/*
  bool in_area(int * current, double * expected, double radius)
  {
  if (sqrt((current[0]-expected[0])*(current[0]-expected[0])+(current[1]-expected[1])*(current[1]-expected[1]))<radius) return true;
  else return false;
  
  }
*/



void link_particules(vector<Points> * & points,vector<Track> & tracks,double search_radius,int NB_frames)
{
  cout<<"This version of link_particules is deprecated, use the other one..."<<endl;
  double expected_position[2];
  unsigned int frame=0;
  //initialisation
  for (unsigned int i=0;i<points[0].size();i++)
    {
      
      tracks.push_back(Track(points[0].at(i).center_position()[0],points[0].at(i).center_position()[1],0));
      points[0].at(i).track_index=i;
    }


  //calcul
  for (unsigned int i=0;i<NB_frames-1;i++) //	  expected_position[0]=points[i][j].center[0];
    {
      if (i%100==0)  cout<<i<<"/"<<NB_frames<<endl;
      for (unsigned int j=0;j<tracks.size();j++)  //loop on particule of frame N
	{
	  // test(j);
	  if (tracks[j].Frame.back()==i)
	    {
	      //Linking of particules already linked in the precedent frame
	      //TO change in further developments
	      expected_position[0]=tracks[j].X.back();
	      expected_position[1]=tracks[j].Y.back();
	      Points * candidate;
	      double candidate_cost=search_radius+1;
	      for (unsigned int k=0;k<points[i+1].size();k++) //loop in particules in next frame
		{
	     
		  if (points[i+1].at(k).track_index==-1)
		    {
		      double x_current=points[i+1].at(k).center_position()[0];
		      double y_current=points[i+1].at(k).center_position()[1];

		      if (in_area(points[i+1].at(k).center_position(),expected_position,search_radius))  //put the cost function here ?
			{
			  if (cost_function(points[i+1].at(k).center_position(),expected_position)<=candidate_cost)
			    {  
			      candidate=&points[i+1].at(k);
			      candidate_cost=cost_function(points[i+1].at(k).center_position(),expected_position);
			  
			    } //end if cost<
			}// end if in area
		    }//end if not already allocated
		}//end loop particules in next frame
	      if (candidate_cost<search_radius+1){
		tracks[j].X.push_back(candidate->center_position()[0]);
		tracks[j].Y.push_back(candidate->center_position()[1]);
		tracks[j].Frame.push_back(i+1);
		candidate->track_index=j;
		// cout<<"Track "<<j<<" found a match at position "<<candidate->center[0]<<" "<<candidate->xcenter[1]<<endl;
	      }
	    }  
	} //end loop on tracks of frame N


       
      for (unsigned int j=0;j<points[i].size();j++)  //loop on particules not being attributed on a tracks previously
	{
	  // test(j);
	  if (points[i].at(j).track_index==-1)
	    {
	      expected_position[0]=points[i].at(j).center_position()[0];
	      expected_position[1]=points[i].at(j).center_position()[1];
	      Points * candidate;
	      double candidate_cost=search_radius+1;

	      for (unsigned int k=0;k<points[i+1].size();k++) //loop in particules in next frame
		{
		  //cout<<"k="<<k<<endl; 
		  if (points[i+1].at(k).track_index==-1)
		    {
		      double x_current=points[i+1].at(k).center_position()[0];
		      double y_current=points[i+1].at(k).center_position()[1];
		      
		      //cout<<in_area(points[i+1].at(k).center_position(),expected_position,search_radius)<<endl;
		      if (in_area(points[i+1].at(k).center_position(),expected_position,search_radius))  //put the cost function here ?
			{
			 
			  if (cost_function(points[i+1].at(k).center_position(),expected_position)<=candidate_cost)
			    {
			     
			      candidate=&points[i+1].at(k);
			      candidate_cost=cost_function(points[i+1].at(k).center_position(),expected_position);
			      
			    } //end if cost<
			}// end if in area
		    }//end if not already allocated
		}//end loop particules in next frame
	      if (candidate_cost<search_radius+1)
		{
		  tracks.push_back(Track(points[i].at(j).center_position()[0],points[i].at(j).center_position()[1],i));
		  tracks.back().add_point(candidate->center_position()[0],candidate->center_position()[1],i+1);
		  candidate->track_index=tracks.size();
	      
	      
		  //cout<<"Particules in "<<points[i].at(j).center[0]<<" "<<points[i].at(j).center[1]<<" @ "<<i<<" found a match in "<<candidate->center[0]<<" "<<candidate->center[1]<<"  New track n : "<<candidate->track_index<<endl;
		  //test(7);
		}
	      //test(8);
	    } //not already attributed
	} //not previously attributed
    
    } //loop on frames
 
} //end of function











void link_particules(vector<Points> * & points,vector<Track> & tracks,double search_radius,int NB_frames, int size_min, int size_max,unsigned int gap, unsigned int strategy, double flow_x, double flow_y,boost::archive::text_iarchive & ia,bool mode_low_ram)
{

  /*///////////////////////////////STRATEGY//////////////////////////////
    if 0 : neirest neighbor
    if 1 : linear prediction
    if 2 : quadratic predicition
  */
  //test(0);

 
  double expected_position[2];
  unsigned int frame=0;
  //initialisation
  for (unsigned int i=0;i<points[0].size();i++)
    {
      if (points[0].at(i).area()>=size_min && points[0].at(i).area()<=size_max)
	{
	  tracks.push_back(Track(points[0].at(i).center_position()[0],points[0].at(i).center_position()[1],0,points[0].at(i).area()));
	  //cout<<points[0].at(i).area()<<endl;
	  points[0].at(i).track_index=i;
	}
    }


  //calcul
  for (unsigned int i=0;i<NB_frames-1;i++) //	  expected_position[0]=points[i][j].center[0];
    {
      
      if (mode_low_ram) ia >> points[i+1];
   
      if (i%100==0)
	{
	  cout<<"\r"<<i<<"/"<<NB_frames;
	  fflush(stdout);
	}
      for (unsigned int j=0;j<tracks.size();j++)  //loop on particule of frame N
	{
	  // test(j);
	  if (tracks[j].Frame.back()<=i && tracks[j].Frame.back()>=i-gap)
	    {
	      //Linking of particules already linked in the precedent frame
	      //TO change in further developments
	      if (strategy==1)
		{
		  if (tracks[j].X.size()>1)
		    {
		      int size=tracks[j].X.size();
		      expected_position[0]=(tracks[j].X.at(size-1)-tracks[j].X.at(size-2))/((tracks[j].Frame.at(size-1)-tracks[j].Frame.at(size-2)))+tracks[j].X.at(size-1)+flow_x;
		      expected_position[1]=(tracks[j].Y.at(size-1)-tracks[j].Y.at(size-2))/((tracks[j].Frame.at(size-1)-tracks[j].Frame.at(size-2)))+tracks[j].Y.at(size-1)+flow_y;

		    }
		  else
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



		       
	      Points * candidate=NULL;
	      double candidate_cost=search_radius+1;
	      for (unsigned int k=0;k<points[i+1].size();k++) //loop in particules in next frame
		{
		  
		  if (points[i+1].at(k).track_index==-1 && points[i+1].at(k).area()>=size_min && points[i+1].at(k).area()<=size_max)
		    {
		      double x_current=points[i+1].at(k).center_position()[0];
		      double y_current=points[i+1].at(k).center_position()[1];

		      if (in_area(points[i+1].at(k).center_position(),expected_position,search_radius))  //put the cost function here ?
			{
			  if (cost_function(points[i+1].at(k).center_position(),expected_position)<=candidate_cost)
			    {  
			      candidate=&points[i+1].at(k);
			      candidate_cost=cost_function(points[i+1].at(k).center_position(),expected_position);
			  
			    } //end if cost<
			}// end if in area
		    }//end if not already allocated
		}//end loop particules in next frame
	      if (candidate_cost<search_radius+1){
		tracks[j].X.push_back(candidate->center_position()[0]);
		tracks[j].Y.push_back(candidate->center_position()[1]);
		tracks[j].Frame.push_back(i+1);
		tracks[j].size_P.push_back(candidate->area());
		candidate->track_index=j;
		// cout<<"Track "<<j<<" found a match at position "<<candidate->center[0]<<" "<<candidate->center[1]<<endl;
	      }
	    }  
	} //end loop on tracks of frame N


       
      for (unsigned int j=0;j<points[i].size();j++)  //loop on particules not being attributed on a tracks previously
	{
	  // test(j);
	  if (points[i].at(j).track_index==-1 && points[i].at(j).area()>=size_min && points[i].at(j).area()<=size_max)
	    {
	      expected_position[0]=points[i].at(j).center_position()[0]+flow_x;
	      expected_position[1]=points[i].at(j).center_position()[1]+flow_y;
	      Points * candidate=NULL;
	      double candidate_cost=search_radius+1;

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
			 
			  if (cost_function(points[i+1].at(k).center,expected_position)<=candidate_cost)
			    {
			     
			      candidate=&points[i+1].at(k);
			      candidate_cost=cost_function(points[i+1].at(k).center_position(),expected_position);
			      
			    } //end if cost<
			}// end if in area
		    }//end if not already allocated
		}//end loop particules in next frame
	      if (candidate_cost<search_radius+1)
		{
		  tracks.push_back(Track(points[i].at(j).center_position()[0],points[i].at(j).center_position()[1],i,points[i].at(j).area()));
		  tracks.back().add_point(candidate->center_position()[0],candidate->center_position()[1],i+1,candidate->area());
		  candidate->track_index=tracks.size();
	      
		}
	    } //not already attributed
	} //not previously attributed
    	  vector<Points> tamp_vec;
	  points[i].swap(tamp_vec);
    } //loop on frames
 
} //end of function


