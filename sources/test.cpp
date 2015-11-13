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
  /* if( argc != 2)
     {
     cout <<" Usage: display_image Video to load..." << endl;
     return -1;
     }*/

 fstream sortie("tracked_spots.csv",ios::out);
 double search_radius=0;
  unsigned int NB_remanence=50;
  vector<Track> tracks; 
  char key;
  unsigned int i=0,kernel_size=3,derivative_size;
  cout<<"    Blur size (positive and odd) >>  ";
  cin>>kernel_size;
  cout<<"    Derivative size (positive and odd) >>  ";
  cin>>derivative_size;
  cout<<"    Search radius (positive) >>  ";
  cin>>search_radius;

  

  // VideoCapture video(argv[1]);
  // VideoCapture video("../img/video%04d.tif");
  VideoCapture video("../video.avi");

  //video.set(CV_CAP_PROP_FOURCC,CV_FOURCC('H','F','Y','U'));
  if (video.isOpened()) cout<<"Fichier source ouvert correctement !"<<endl<<"     Infos :"<<endl;
 
  cout<<"w="<<video.get(CV_CAP_PROP_FRAME_WIDTH)<<"  H="<<video.get(CV_CAP_PROP_FRAME_HEIGHT)<<"  FPS="<<video.get(CV_CAP_PROP_FPS)<<endl;


  
  //video.set(CV_CAP_PROP_FRAME_WIDTH,video.get(CV_CAP_PROP_FRAME_WIDTH)-100);
  /*
    video.read(image);
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);
    Scalar intensity=image.at<uchar>(0,0);
    cout<<intensity.val[0];
    image.at<uchar>(0,0)=255;
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.
    waitKey(0);*/




  

  int NB_Frame=video.get(CV_CAP_PROP_FRAME_COUNT);
  cout<<NB_Frame<<" images à traiter..."<<endl;
  Mat image[NB_Frame+1];
  Mat output[NB_Frame+1];
  Mat blurred_img,LOG_img[NB_Frame+1],mean_img,frame32f,tamp,thresholded;
 
  //  namedWindow( "Display window", WINDOW_NORMAL );// Create a window for display.
  //  namedWindow( "Threshold IMG", WINDOW_NORMAL );// Create a window for display.
  namedWindow( "LOG", WINDOW_NORMAL );// Create a window for display.
  VideoWriter output_video("output_file.avi",video.get(CV_CAP_PROP_FOURCC),200,Size(video.get(CV_CAP_PROP_FRAME_WIDTH),video.get(CV_CAP_PROP_FRAME_HEIGHT)),true);

  /*
  if (output_video.isOpened()) cout<<"Fichier de sortie créé.."<<endl;
  cout<<"Traitement... Temps de rémanence : "<<NB_remanence<<" images"<<endl;
  */
  mean_img.convertTo(mean_img,6);
  cout<<"Calculating the background..."<<endl;
  while(video.read(image[i]))
    {
      // if (i==0) mean_img=image[0].clone()*1./NB_Frame;
      // imshow( "Display window", image[i]);
      // waitKey(0);
      //aussianBlur(image[i],blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
      //imshow( "Blurred Image", blurred_img);
      //waitKey(0);
      //aplacian(blurred_img,LOG_img[i],8,1,1,0,BORDER_DEFAULT);
      //imshow( "LOG", LOG_img[i]);
      //waitKey(0);
      //mwrite("log_img.jpg",LOG_img[i]);
      //imshow( "LOG", LOG_img[i]+blurred_img);
      // waitKey(0);
      tamp=image[i].clone();
      tamp.convertTo(tamp,6);
	
      tamp=tamp*1./NB_Frame;
      if (i==0) mean_img=tamp.clone();
      else mean_img+=tamp.clone();

      i++;
    }
  cout<<"Computing LOG filter..."<<endl;
  mean_img.convertTo(mean_img,0);


  
      GaussianBlur(mean_img-image[0],blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
      Laplacian(blurred_img,LOG_img[0],CV_32F,derivative_size,1,0,BORDER_DEFAULT);
       normalize(LOG_img[0],LOG_img[0],0,1,CV_MINMAX);
       // imshow( "LOG", LOG_img[0]);
       // waitKey(0);
      
      bool satisfied=false;
      double _threshold;
      while (!satisfied)
	{
	  // imshow("LOG",LOG_img[0]);
	  cout<<"    Threshold >> ";
	  cin>>_threshold;
	  threshold(LOG_img[0],thresholded,_threshold,1,THRESH_BINARY);
	  imshow("LOG", thresholded);
	  key=cvWaitKey(0);
	  cout<<"    Satisfied ? (0 or 1) >> ";
	  cin>>satisfied;
	  cout<<"Key : "<<key<<endl;
	}
      
        cvtColor(thresholded, thresholded, CV_RGB2GRAY);

      vector<Points> *points;
      points=new vector<Points>[NB_Frame];
      find_particules(thresholded,0,points[0]);
      cout<<"nombre final :"<<points[0].size()<<endl;

      
      for (int i=0;i<points[0].size();i++)
	{	  
	  circle(LOG_img[0],Point(points[0][i].center_position()[1],points[0][i].center_position()[0]),(points[0][i].area()/2),Scalar(0xFFFF),1,8,0);
	}

     
      imshow("LOG",LOG_img[0]);
      waitKey(0);

      i=1;
      cout<<"  D&tection..."<<endl;
      while(i<NB_Frame)
	{
	  if (i%100==0) cout<<i<<"/"<<NB_Frame<<endl;
         GaussianBlur(mean_img-image[i],blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
      Laplacian(blurred_img,LOG_img[0],CV_32F,derivative_size,1,0,BORDER_DEFAULT);
       normalize(LOG_img[0],LOG_img[0],0,1,CV_MINMAX);

       threshold(LOG_img[0],thresholded,_threshold,1,THRESH_BINARY);
       cvtColor( thresholded, thresholded, CV_RGB2GRAY );
       find_particules(thresholded,0,points[i]);
       /*  
        cout<<"nombre final :"<<points[i].size()<<endl;

	double tet;
	cin>>tet;*/
       /*
       for (int j=0;j<points[i].size();j++)
	 {	  
	   circle(LOG_img[0],Point(points[i][j].center_position()[1],points[i][j].center_position()[0]),sqrt(points[i][j].area()),Scalar(0xFFFF),1,8,0);
	   }
       
       imshow("LOG",LOG_img[0]);
       waitKey(0);
       
       */
       i++;
	}
      
      cout<<"  Lien..."<<endl;
      link_particules(points,tracks,search_radius, NB_Frame);

      cout<<"OK !  Track créées : "<<tracks.size()<<endl;

      /*
      cout<<"Lenght of selected : "<<tracks[5].X.size()<<endl;
      for (unsigned int l=0;l<tracks[5].X.size();l++)
	{
	  cout<<tracks[5].X.at(l)<<" "<<tracks[5].Y.at(l)<<endl;

	  }*/
      /*
      
      for (i=0;i<NB_Frame;i++)
	{
	  cout<<"Calcul en cours..."<<endl;
	  GaussianBlur(mean_img-image[0],blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT); 
	  Laplacian(blurred_img,LOG_img[0],CV_32F,derivative_size,1,0,BORDER_DEFAULT);
	  normalize(LOG_img[0],LOG_img[0],0,1,CV_MINMAX);
	  imshow( "LOG", LOG_img[0]);
	  waitKey(0);
	}
      
      */
      
      /*
      // mean_img=mean_img.clone()/NB_Frame;
      mean_img.convertTo(mean_img,0);
      // normalize(mean_img,mean_img,0,255,CV_MINMAX);
      imshow("LOG",mean_img);
      waitKey(0);
      
 

      */
  cout<<"Écriture des fichiers..."<<endl;
  sortie<<"#Tracks_ID,X,Y,T"<<endl;
  for (unsigned int i=0;i<tracks.size();i++)
    {
      //      cout<<tracks[i].get_lenght()<<endl;
      if (tracks[i].get_lenght()>10)
	{
	  for (unsigned int j=0;j<tracks[i].get_lenght();j++)
	    {
	      sortie<<i<<","<<tracks[i].X.at(j)<<","<<tracks[i].Y.at(j)<<","<<tracks[i].Frame.at(j)<<endl;
	    }
	}
    }
  return 0;
}
