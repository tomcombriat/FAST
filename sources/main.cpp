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


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include "point.hpp"
#include "find_particules.hpp"
#include <vector>
#include <cmath>
#include "track.hpp"
#include "link_particules.hpp"
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>



using namespace cv;
using namespace std;

//global variable
int _threshold,area_max=1000,area_min=0,N_dilate=0,N_erode=0;
double  max_threshold = 255, min_threshold = 0;
Mat thresholded,LOG_img,img;

vector<Points> * points;   //This will contain all the points detected and will be passed to the link_particles function. Has to be global because of the OpenCV visualisation





void threshold_TB(int, void *)   //Trackbar for the threshold
{
  Mat tamp,element=getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1));
  threshold(LOG_img,thresholded,_threshold, max_threshold,THRESH_BINARY);
  dilate(thresholded,tamp,element,Point(-1,-1),N_dilate,BORDER_DEFAULT);
  erode(tamp,tamp,element,Point(-1,-1),N_erode,BORDER_DEFAULT);
  imshow("Display", tamp);
  points[0].clear();
  find_particules(tamp,0,points[0]);    
}


void area_TB_min(int, void *)   //Trackbar for the area min of a particle
{

  Mat tamp=img.clone();

  for (int i=0;i<points[0].size();i++)
    {

      if (points[0][i].area()>area_min && points[0][i].area()<area_max)
	{
	  circle(tamp,Point(points[0][i].center_position()[1],points[0][i].center_position()[0]),(sqrt(points[0][i].area())),Scalar(0xFFFF),1,8,0);  //Draw a blue circle on image when a particle is in the range of wanted area
	}
    }      
  imshow("Display",tamp);
}


void area_TB_max(int, void *)  //Idem for the area max
{

  Mat tamp=img.clone();
  //cout<<   "S="<<points[0].size()<<endl;
  for (int i=0;i<points[0].size();i++)
    {
      if (points[0][i].area()>area_min && points[0][i].area()<area_max)
	{
	  circle(tamp,Point(points[0][i].center_position()[1],points[0][i].center_position()[0]),(sqrt(points[0][i].area())),Scalar(0xFFFF),1,8,0);
	}
    }      
  imshow("Display",tamp);
}






void dilate_TB(int, void *)
{
  Mat tamp;
  Mat element=getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1));

  dilate(thresholded,tamp,element,Point(-1,-1),N_dilate,BORDER_DEFAULT);
  erode(tamp,tamp,element,Point(-1,-1),N_erode,BORDER_DEFAULT);
  imshow("Display", tamp);
  points[0].clear();
  find_particules(tamp,0,points[0]);  
}





void erode_TB(int, void *)
{
  Mat tamp;
  Mat element=getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1));

  erode(thresholded,tamp,element,Point(-1,-1),N_erode,BORDER_DEFAULT);
  dilate(tamp,tamp,element,Point(-1,-1),N_dilate,BORDER_DEFAULT);
  imshow("Display", tamp);
  points[0].clear();
  find_particules(tamp,0,points[0]);  
}






int main( int argc, char** argv )
{

  system("clear");
  try{
    system("mkdir archive");   //perhaps the directory is already created (p.e. if Fast was killed before the end of its execution (*PAN*)
	   }
  catch(...)  
    {}

  if( argc < 3)
    {
      cout <<"\n\nUsage: ./FAST Video to load output_File -arg" << endl;
      cout<<"\n -arg can be -test if you want a complete display of found particles to check your parameters. This mode is not suitable for a real calculation.\n\n -arg can also be -low-ram if you the software to be less ram comsuptive (but a little slower…), or -no-BG if you do not want FAST to compute a background (if your particles are moving very slowy), or -inv if you want to detect white particles. All these options are compatible."<<endl<<endl;
      return -1;
    }


  cout<<"\n\n     Copyright 2014,2015,2016 Thomas Combriat (LIPhy - Grenoble - France)\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\n This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\n You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n";


  bool mode_test=false;
  bool mode_low_ram=false;
  bool mode_inv=false;
  bool mode_no_BG=false;

  /* Check for passed arguments */
  if (argc>3)
    {
      for (unsigned int i=3; i<argc;i++)
	{
	  if (std::string(argv[i])=="-test")
	    {
	      mode_test=true;
	      cout<<  "\n     * TEST MODE ENABLE *\n";
	    }
	  if (std::string(argv[i])=="-low-ram") 
	    {
	      mode_low_ram=true;
	      cout<<  "\n     * LOW-RAM MODE ENABLE *\n";
	    }
	  if (std::string(argv[i])=="-inv") 
	    {
	      mode_inv=true;
	      cout<<  "\n     * ALL IMAGES WILL BE INVERTED *\n";
	    }
	  if (std::string(argv[i])=="-no-BG") 
	    {
	      mode_no_BG=true;
	      cout<<  "\n     * NO BACKGROUD WILL BE USED *\n";
	    }
	  if (std::string(argv[i])=="-16bits") 
	    {
	      max_threshold = 65536;
	      cout<<  "\n     * 16 BITS MODE *\n"; 
	    }
	}     
    }

  
  cout<<"\n\n    Welcome to FAST (a Fast And Simple Tracker), developped at the LIPhy (Grenoble - France).\n    This program is distributed under the licence GNU GPLv3.\n\n\n";
  string argv1_old;
  fstream log_i("log.txt",ios::in);
  log_i>>argv1_old;


  
  /****************ARCHIVE************/
  std::ofstream ofs("archive/ar");
  boost::archive::text_oarchive oa(ofs);

  /**************ARCHIVE***********/


 



  fstream sortie(argv[2],ios::out);
  double search_radius=0,flow_x=0,flow_y=0;
  vector<Track> tracks;
  char key;
  int gap;
  unsigned int i=0,kernel_size=3,derivative_size,strategy,flow_remover;
  Mat blurred_img,mean_img,frame32f,tamp,input_bg,element=getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1));

  cout<<"  Press enter to continue...\n\n";
  cin.ignore();





  VideoCapture video(argv[1]);

  if (video.isOpened())
    {
      cout<<"Source file opened!"<<endl<<"     Infos :"<<endl;
      cout<<"w="<<video.get(CV_CAP_PROP_FRAME_WIDTH)<<"  H="<<video.get(CV_CAP_PROP_FRAME_HEIGHT)<<"  FPS="<<video.get(CV_CAP_PROP_FPS)<<endl;      
      video.set(CV_CAP_PROP_FRAME_WIDTH,video.get(CV_CAP_PROP_FRAME_WIDTH)+10);
    }
  else
    {
      cout<<"An error occured while opening the source file... \nNow exiting"<<endl;
      return 0;
    }

  

  int NB_Frame=video.get(CV_CAP_PROP_FRAME_COUNT); 
  cout<<NB_Frame<<" images to analyse"<<endl;
  points=new vector<Points>[NB_Frame];





  
  if (!mode_no_BG)
    {
      bool new_background=true;
      if (argv[1]==argv1_old) // Check if the video video to analyse is the same than the previous video… In this case don't need to calculate a second time the same background
	{
	  new_background=false;  
	  cout<<"\n   An old background for your video has been detected!\n";
	}

      mean_img.convertTo(mean_img,6);
      if( new_background)
	{
	  cout<<"Calculating the background..."<<endl;
	  while(video.read(img)) //read images until the end of file
	    {
	      if (i%100==0)
		{
		  cout<<"\r"<<i<<"/"<<NB_Frame;
		  fflush(stdout);
		}

	      tamp=img.clone();  // Warning, a simple assignation tamp=img; do not work because OCV does not make a copy in this case.
	      if (mode_inv) bitwise_not(tamp,tamp); // Invert image if mode -inv enable
	      tamp.convertTo(tamp,6);  // convert to grayscale 32bit float
	       

	      //Computing the mean of all images
	      tamp=tamp*1./NB_Frame;  
	      if (i==0) mean_img=tamp.clone();
	      else mean_img+=tamp.clone();
	      i++;
	    }

	  //Writing the background to a file in order to reuse it
	  vector<int> compression_params;
	  compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	  compression_params.push_back(100);
	  imwrite ("background.jpg",mean_img,compression_params);

	}
      else   //read already calculted background
	{
	  input_bg=imread("background.jpg",-1);
	  mean_img=input_bg.clone();
	}
      mean_img.convertTo(mean_img,0);
    }

  
  fstream log("log.txt",ios::out); //will store the parameters of the calculation. Will allow next instance of the program to know if it has to compute a new bg or no.
  log<<argv[1]<<endl;




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
  int c= 0;
  video2.read(img);
  //imshow("Dis", img);
  
  if (mode_inv && !mode_no_BG)  bitwise_not(img,img);
  if (!mode_inv && mode_no_BG)  bitwise_not(img,img);  //As any background is substracted, the image is not in the good dynamic
  img.convertTo(img, CV_32F);
  if (mode_no_BG) GaussianBlur(img,blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
  else
    {
      mean_img.convertTo(mean_img,CV_32F);
      GaussianBlur(mean_img-img,blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
    }
 
  cvtColor(blurred_img, blurred_img, CV_RGB2GRAY);   //convert to Grayscale for laplacian calculation
  Laplacian(blurred_img,LOG_img,CV_32F,derivative_size,-1,120,BORDER_DEFAULT);
 


  cout << "\n  Use the cursor on the image to set the threshold (press enter when you are done)\n";


  /* Finding the maximum of the image */
  minMaxLoc(LOG_img, &min_threshold, &max_threshold);
  namedWindow( "Display", WINDOW_NORMAL);// Create a window for display.
  createTrackbar("Threshold_set", "Display", &_threshold, max_threshold, threshold_TB );
  createTrackbar("Dilate", "Display", &N_dilate, 4, dilate_TB );
  //createTrackbar("Erode", "Display", &N_erode, 4, erode_TB );
 
  threshold_TB(_threshold,0);



  while (c==0) c=waitKey(0);   //1048586 is the "enter" key code 

  c=0;







  cout<<"\n  First image characteristics: "<<endl;
  cout<<"    - number of detected particles: "<<points[0].size()<<endl;
  cout<<"    - mean size/std: "<<points_mean(points[0])<<"  "<<points_std(points[0])<<endl;

  imshow("Display",img);
  cout << "\n  Use the cursor on the image to set the min/max size of the particles (press enter when you are done)\n";
  createTrackbar("Area min", "Display", &area_min, 1000, area_TB_min);
  createTrackbar("Area max", "Display", &area_max, 1000, area_TB_max);
  
  
  while (c==0) c=waitKey(0);
  destroyAllWindows();
  
  
  cout<<"\n     Do you want to use the predictive tracker? (Useful if your particles are more or less deterministic) (0 or 1) >> ";
  cin>>strategy;
  cout<<"\n     Do you want to remove a flow velocity? (If you know the global velocity of the flow) (0 or 1) >> ";
  cin>>flow_remover;

  if (flow_remover)
    {
      cout<<"        X flow velocity (in pixels/frame) >> ";
      cin>>flow_x;
      cout<<"        Y flow velocity (in pixels/frame) >> ";
      cin>>flow_y;
    }
  
  cout<<"\n     Gap closing (null or positive integer) >> ";
  cin>>gap;
 
  i=1;
  cout<<"  Detection..."<<endl;
  while(i<NB_Frame)
    {
      video2.read(img);
      if (mode_inv && !mode_no_BG)  bitwise_not(img,img);
      if (!mode_inv && mode_no_BG)  bitwise_not(img,img);  //As any background is substracted, the image is not in the good dynamic
      if (i%100==0)
	{
	  cout<<"\r"<<i<<"/"<<NB_Frame;
	  fflush(stdout);
	}

      points[i].reserve(points[i-1].size());  //for speed purposes
      img.convertTo(img, CV_32F);
      if (mode_no_BG) GaussianBlur(img,blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
      else GaussianBlur(mean_img-img,blurred_img,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
      cvtColor(blurred_img, blurred_img, CV_RGB2GRAY);
      Laplacian(blurred_img,LOG_img,CV_32F,derivative_size,-1,120,BORDER_DEFAULT);
      threshold(LOG_img,thresholded,_threshold,max_threshold,THRESH_BINARY);
      //erode(thresholded,thresholded,element,Point(-1,-1),N_erode,BORDER_DEFAULT);
      dilate(thresholded,thresholded,element,Point(-1,-1),N_dilate,BORDER_DEFAULT);


      find_particules(thresholded,0,points[i]);

      if (mode_test) //update the image in visualisation (soooo slow…)
	{  
	  tamp=img.clone();
	  for (int j=0;j<points[i].size();j++)
	    {
	      if (points[i][j].area()>area_min && points[i][j].area()<area_max)
		{
		  circle(tamp,Point(points[i][j].center_position()[1],points[i][j].center_position()[0]),sqrt(points[i][j].area()),Scalar(0xFFFF),1,8,0);
		}}

	  //imshow("Display",tamp);
	  imshow("Display",tamp);
	  cv::waitKey(50);
	  cout<<"  Frame N: "<<i<<"  Found: "<<points[i].size()<<" particles"<<endl;
	}
  
      if (mode_low_ram)
	{
	    
	  oa << points[i]; //load  points in archive (FIFO)
	    

	  vector<Points> tamp_vec;
	  points[i].swap(tamp_vec);  //empty vector to free memory

	  if (i%100==0) ofs.flush(); //actually write in file (not done for every frame for performances purposes
	}
      i++;
    }   /////END OF WHILE ON FRAMES
  

  ofs.close();  //close archive

  //Writing log
  log<<"\nThreshold : "<<_threshold<<"\nArea min/max : "<<area_min<<"/"<<area_max<<endl;



    sortie<<"#Tracks_ID X Y T size"<<endl;
  
  cout<<endl<<endl<<"  Linking particles..."<<endl;
  
  if (mode_low_ram)
    {
  std::ifstream ifs("archive/ar");  //open archive for linking
  boost::archive::text_iarchive ia(ifs);
  link_particules(points,tracks,search_radius, NB_Frame,area_min,area_max,gap,0,flow_y,flow_x,ia,mode_low_ram);
    }
  

  if (!mode_low_ram) link_particules(points,tracks,search_radius, NB_Frame,area_min,area_max,gap,0,flow_y,flow_x);


  cout<<endl<<endl<<"Done!  "<<tracks.size()<<" tracks have been created!"<<endl;


  cout<<"Writing files..."<<endl;

  for (unsigned int i=0;i<tracks.size();i++)
    {
      if (tracks[i].get_lenght()>1)
	{
	  for (unsigned int j=0;j<tracks[i].get_lenght();j++)
	    {

	      sortie<<i<<" "<<tracks[i].Y.at(j)<<" "<<tracks[i].X.at(j)<<" "<<tracks[i].Frame.at(j)<<" "<<tracks[i].size_P.at(j)<<endl;
	    } 
	}
    }
  system("rm -r archive");
  cout<<"\n Now exiting..."<<endl;
  return 0;
}
