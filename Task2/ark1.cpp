#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <queue>
#include <climits>
#include <vector>
#include <stdio.h>
using namespace std;
using namespace cv;


int main()
{
  Mat img=imread("perception2.jpg",1);                                                    //Declarations and initializations
  Mat part=imread("part2.jpg",1);
  int total=part.rows*part.cols;
  int i=0,j=0,temp1=0,temp2=0,l,m,X=450,Y=450,flag=0,e=20,count=0,temp=0;

  for( i=0;i<img.rows-part.rows;i++)                                                     //Loop for image traversal
    {
      for(j=0;j<img.cols-part.cols;j++)
	{
	  count=0;
	  
	  if((abs(part.at<Vec3b>(0,0)[0]-img.at<Vec3b>(i,j)[0])<=e) && (abs(part.at<Vec3b>(0,0)[1]-img.at<Vec3b>(i,j)[1])<=e) && (abs(part.at<Vec3b>(0,0)[2]-img.at<Vec3b>(i,j)[2])<=e))                                             //Check if pixel matches with first pixel of part
	    {
	      for(l=0;l<part.rows;l++)                                        //Check for no. of pixels that match in corresponding kernel
		{
		  for(m=0;m<part.cols;m++)
		    {
	 
		      if((abs(part.at<Vec3b>(l,m)[0]-img.at<Vec3b>(i+l,j+m)[0])<=e) && (abs(part.at<Vec3b>(l,m)[1]-img.at<Vec3b>(i+l,j+m)[1])<=e) && (abs(part.at<Vec3b>(l,m)[2]-img.at<Vec3b>(i+l,j+m)[2])<=e))
			{
			  count++;                                           
			}
		    }
		
		}
	    }
	    
	  if(count>=total-5000)                                            //Count thresholding
	    {
	      temp=count;
	      X=i;
	      Y=j;
	      break;

	    }
	}
	   
    } 

 
  printf("%d\n",temp);

  printf("%d %d\n",X,Y);
  cv::Point A,B,C,D;

  A.x=Y;                                                                      //Draw lines around detected section
  A.y=X; 
  B.x=Y;
  B.y=X+part.rows;
  C.x=Y+part.cols;
  C.y=X+part.rows;
  D.x=Y+part.cols;
  D.y=X;

  cv::line(img,A,B,(0,255,0),3,8,0); 
  cv::line(img,B,C,(0,255,0),3,8,0);
  cv::line(img,C,D,(0,255,0),3,8,0);
  cv::line(img,D,A,(0,255,0),3,8,0);


  imshow("final",img);
  imshow("part",part);
  waitKey(0);

  return 0;
}


	 
	     
