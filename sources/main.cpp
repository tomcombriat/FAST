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


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include"point.hpp"
#include "find_particules.hpp"
#include <vector>
#include<cmath>
#include "track.hpp"
#include "link_particules.hpp"
#include<fstream>


using namespace cv;
using namespace std;



int main( int argc, char** argv )
{

  system("clear");

  if( argc < 3)
    {
      cout <<"\n\nUsage: ./PTV Video to load output_File -arg" << endl;
      cout<<"-arg can be -test if you want a complete display of found particles to check your parameters. This mode is not suitable for a real calculation."<<endl<<endl;
      return -1;
    }


  cout<<"\n\n     Copyright 2014,2015 Thomas Combriat\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\n This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\n You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n";


  bool mode_test=false;
  if (argc==4)
    {
      if (std::string(argv[3])=="-test") mode_test=true;
      else cout<<"\n Last argument unknown... Assuming non-test mode\n\n";
    }


  cout<<"\n\n    Welcome to FAST (a Fast And Simple Tracker), developped at the LIPhy (Grenoble - France).\n    This program is distributed under the licence GNU GPLv3.\n\n\n";

  fstream sortie(argv[2],ios::out);
  fstream log("log.txt",ios::out);  //for further developments
  double search_radius=0;
  unsigned int NB_remanence=50,area_max=1000,area_min=0;
  vector<Track> tracks;
  char key;
  unsigned int i=0,kernel_size=3,derivative_size,gap;

  cout<<"  Press enter to continue...\n\n";
  cin.ignore();


  VideoCapture video(argv[1]);

  if (video.isOpened())
    {
      cout<<"Source file opened!"<<endl<<"     Infos :"<<endl;
      cout<<"w="<<video.get(CV_CAP_PROP_FRAME_WIDTH)<<"  H="<<video.get(CV_CAP_PROP_FRAME_HEIGHT)<<"  FPS="<<video.get(CV_CAP_PROP_FPS)<<endl;
    }
  else
    {
      cout<<"An error occured while opeing the source file... \nNow exiting"<<endl;
      return 0;
    }



  int NB_Frame=video.get(CV_CAP_PROP_FRAME_COUNT);

cout<<NB_Frame<<" images to analyse"<<endl;
  Mat * output;
  output=new Mat[NB_Frame+1];


  Mat blurred_img,LOG_img[1],mean_img,frame32f,tamp,thresholded,img,input_bg;


  namedWindow( "Display", WINDOW_NORMAL);// Create a window for display.


  bool new_background=true;

  /*
  cout<<"Do you want to calculate a new background? (If the last time you have executed this program, it was on the video you (0 or 1) >>  ";
  cin>>new_background;
  */


  mean_img.convertTo(mean_img,6);
  if( new_background)
    {
      cout<<"Calculating the background..."<<endl;
      while(video.read(img))
	{
	  if (i%100==0)
	    {
	      cout<<"\r"<<i<<"/"<<NB_Frame;
	      fflush(stdout);
	    }
	  tamp=img.clone();
	  tamp.convertTo(tamp,6);

	  tamp=tamp*1./NB_Frame;
	  if (i==0) mean_img=tamp.clone();
	  else mean_img+=tamp.clone();
	  i++;
	}
      imwrite ("background.jpg",mean_img);
    }
  else
    {
      input_bg=imread("background.jpg",-1);
      mean_img=input_bg.clone();
    }


  mean_img.convertTo(mean_img,0);

  cout<<"\n\n  LOG detector: informations required:\n";

 cout<<"    Blur size (positive and odd) >>  ";
  cin>>kernel_size;
  cout<<"    Derivative size (positive and odd) >>  ";
  cin>>derivative_size;
  cout<<"    Search radius (positive) >>  ";
  cin>>search_radius;

  log<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<mode_test<<endl;
  log<<"Blur size = "<<kernel_size<<"\nDerivative size = "<<derivative_size<<"\nSearch radius = "<<search_radius<<endl;

  VideoCapture video2(argv[1]);
  video2.read(img);


  GaussianBlur(mean_img-img,blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
  cvtColor(blurred_img, blurred_img, CV_RGB2GRAY);
  Laplacian(blurred_img,LOG_img[0],CV_32F,derivative_size,-1,120,BORDER_DEFAULT);

  bool satisfied=false;
  double _threshold;
  while (!satisfied)
    {

      cout<<"    Threshold (integer between 0 and 255) >> ";
      cin>>_threshold;
      threshold(LOG_img[0],thresholded,_threshold,255,THRESH_BINARY);
      imshow("Display", thresholded);
      cv::waitKey(50);

      cout<<"    Satisfied ? (0 or 1) >> ";
      cin>>satisfied;
      //cvDestroyWindow("Display");
    }
  //cvDestroyWindow("Display");
  satisfied=false;

  vector<Points> *points;
  points=new vector<Points>[NB_Frame];

  find_particules(thresholded,0,points[0]);
  // test(0);
  cout<<"\n  First image characteristics: "<<endl;
  cout<<"    - number of detected particles: "<<points[0].size()<<endl;
  cout<<"    - mean size/std: "<<points_mean(points[0])<<"  "<<points_std(points[0])<<endl;


  while (!satisfied)
    {
      imshow("Display",thresholded);
      cv::waitKey(50);
      cout<<"     Area min/max (integers, space separated. Ex: '2 20') >> ";
      cin>>area_min>>area_max;
      Mat tamp=img.clone();
      for (int i=0;i<points[0].size();i++)
	{
	  if (points[0][i].area()>area_min && points[0][i].area()<area_max)
	    {
	      circle(tamp,Point(points[0][i].center_position()[1],points[0][i].center_position()[0]),(points[0][i].area()/2),Scalar(0xFFFF),1,8,0);
	    }
	}
      imshow("Display",tamp);
      cv::waitKey(50);
      cout<<"    Satisfied ? (0 or 1) >> ";
      cin>>satisfied;
    }
  cout<<"     Gap closing (null or positive integer) >> ";
  cin>>gap;

  i=1;
  cout<<"  Detection..."<<endl;
  while(i<NB_Frame)
    {
      video2.read(img);
      if (i%100==0)
	{
	  cout<<"\r"<<i<<"/"<<NB_Frame;
	  fflush(stdout);
	}

      GaussianBlur(mean_img-img,blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
      cvtColor(blurred_img, blurred_img, CV_RGB2GRAY);
      Laplacian(blurred_img,LOG_img[0],CV_32F,derivative_size,-1,120,BORDER_DEFAULT);
      threshold(LOG_img[0],thresholded,_threshold,255,THRESH_BINARY);
      find_particules(thresholded,0,points[i]);

      if (mode_test)
	{
	  tamp=img.clone();
	  for (int j=0;j<points[i].size();j++)
	    {
	      if (points[i][j].area()>area_min && points[i][j].area()<area_max)
		{
	      circle(tamp,Point(points[i][j].center_position()[1],points[i][j].center_position()[0]),sqrt(points[i][j].area()),Scalar(0xFFFF),1,8,0);
		}}

	  imshow("Display",tamp);
	  cv::waitKey(50);
	}

      i++;
    }

  cout<<endl<<endl<<"  Linking particles..."<<endl;
  link_particules(points,tracks,search_radius, NB_Frame,0,100,gap);

  cout<<endl<<endl<<"Done!  "<<tracks.size()<<" tracks have benn created!"<<endl;


  cout<<"Écriture des fichiers..."<<endl;
  sortie<<"#Tracks_ID,X,Y,T"<<endl;
  for (unsigned int i=0;i<tracks.size();i++)
    {
      //      cout<<tracks[i].get_lenght()<<endl;
      if (tracks[i].get_lenght()>10)
	{
	  for (unsigned int j=0;j<tracks[i].get_lenght();j++)
	    {
	      sortie<<i<<","<<tracks[i].Y.at(j)<<","<<tracks[i].X.at(j)<<","<<tracks[i].Frame.at(j)<<endl;
	    }
	}
    }
  cout<<"\n Now exiting..."<<endl;
  return 0;
}
