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
  Mat img=imread("perception2.jpg",1);
  Mat part1=imread("part3.jpg",1);
  int total=part1.rows*part1.cols;
  int i=0,j=0,temp1=0,temp2=0,l,m,X=450,Y=450,flag=0,e=20,count=0,temp=0,rotation=0;
  Mat M,part;
  

  while(temp<=1500 && rotation<=360)
    {

      temp=0;

      rotation = 0;
      Point2f src_center(part1.cols/2.0F, part1.rows/2.0F);
      Mat rot_mat = getRotationMatrix2D(src_center, 45, 1.0);
      warpAffine(part1, part, rot_mat, part1.size());
      
      for( i=0;i<img.rows-part.rows;i++)
	{
	  for(j=0;j<img.cols-part.cols;j++)
	    {
	     
	      count=0;
	      if((abs(part.at<Vec3b>(0,70)[0]-img.at<Vec3b>(i,j+70)[0])<=e) && (abs(part.at<Vec3b>(0,70)[1]-img.at<Vec3b>(i,j+70)[1])<=e) && (abs(part.at<Vec3b>(0,70)[2]-img.at<Vec3b>(i,j+70)[2])<=e)) 
		{
		 
		  for(l=0;l<part.rows;l++)
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
	    
	     
	      if(count>=1500)
		{
		  temp=count;
		  X=i;
		  Y=j;
		  break;

		}
	    }
	   
	} 
      rotation+=45;

    }

  printf("%d\n",temp);

  printf("%d %d\n",X,Y);
  cv::Point A,B,C,D;

 
  A.x=Y;
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
  imshow("rotate_theta",part);
  imshow("rotate0",part1);

  // imwrite("task2_3.jpg",img);
  waitKey(0);

  return 0;
}
