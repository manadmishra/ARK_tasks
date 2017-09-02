#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <queue>
#include <climits>
#include <vector>
//#define DCOST 14
//#define COST 10
bool isValid(int i, int j, cv::Mat img)
{
  if( i<0 || j<0 || i >= img.rows-1 || j >= img.cols-1 )
    return 0;
  else
    return 1;
}

struct pixelVal
{
  int gVal=INT_MAX/2;
  int hVal=INT_MAX/2;
  int fVal=INT_MAX;
  cv::Point parent;
  cv::Point current;
  int visited = 1;
};
void updatefVal(pixelVal &a)
{
  a.fVal = a.gVal + a.hVal;
  return;
}
class compare
{
public:
  bool operator()(const pixelVal& l, const pixelVal& r)
  {
    return l.gVal>r.gVal;
  }
};

int main(int argc, char **argv)
{
  std::cout<<"Hi\n";
  cv::Mat a=cv::imread(argv[1],1);
  std::cout<<"There\n";
  cv::Mat b;
  int count_s=0, count_e=0;
  cv::Point start(0), end(0);
  for(int i=0; i<a.rows; i++)
    {
      for(int j=0; j<a.cols; j++)
	{
	  if(a.at<cv::Vec3b>(i,j)[2]==255 && a.at<cv::Vec3b>(i,j)[1]==0 && a.at<cv::Vec3b>(i,j)[0]==0)
	    {
	      count_s++;
	      start.x+=j;
	      start.y+=i;
	    }
	  else if(a.at<cv::Vec3b>(i,j)[2]==0 && a.at<cv::Vec3b>(i,j)[1]==255 && a.at<cv::Vec3b>(i,j)[0]==0)
	    {
	      count_e++;
	      end.x+=j;
	      end.y+=i;
	    }
	}
    }
  start.x/=static_cast<float>(count_s);
  start.y/=static_cast<float>(count_s);
  end.x/=static_cast<float>(count_e);
  end.y/=static_cast<float>(count_e);

  cv::dilate(a, b, cv::Mat(), cv::Point(-1, -1));

  pixelVal A[200][200];
  //define Heurestics
  for(int i=0; i<a.rows; i++)
    {
      for(int j=0; j<a.cols; j++)
	{
	  A[i][j].hVal = 10*abs(i-end.y)+10*abs(j-end.x);
	  A[i][j].current.y=i;
	  A[i][j].current.x=j;
	}
    }

  std::priority_queue<pixelVal, std::vector<pixelVal>, compare > pq;
  
  A[start.y][start.x].gVal=0;
  A[start.y][start.x].fVal=0;
  //A[start.y][start.x].parent.y=start.y;
  //A[start.y][start.x].parent.x=start.x;
  pq.push(A[start.y][start.x]);
  std::cout<<"A-star loop begins\n";
  while(!pq.empty() && (pq.top().current.x != end.x || pq.top().current.y != end.y))
    {
      for(int i=pq.top().current.y-1, l=-1; i<=pq.top().current.y+1 && l<=1; i++, l++)
      	{
      	  for(int j=pq.top().current.x-1, m=-1; j<=pq.top().current.x+1 && m<=1; j++, m++)
      	    {
      	      if(isValid(i,j,a))               //most important line of code
		{
		  if(A[i][j].visited == 1 && !((b.at<cv::Vec3b>(i,j)[0]==255) && (b.at<cv::Vec3b>(i,j)[1]==255) && (b.at<cv::Vec3b>(i,j)[2]==255)))
		    {
		      if(l==0 && m==0)
			{
			  updatefVal(A[i][j]);
			  continue;
			}
		      if(abs(l)==abs(m))
			{
			  if(pq.top().gVal + DCOST < A[i][j].gVal )
			    {
			      A[i][j].gVal = pq.top().gVal + DCOST;
			      A[i][j].parent=pq.top().current;
			      pq.push(A[i][j]);
			    }
			}
		      else
			{
			  if(pq.top().gVal + COST < A[i][j].gVal )
			    {
			      A[i][j].gVal = pq.top().gVal + COST;
			      A[i][j].parent=pq.top().current;
			      pq.push(A[i][j]);
			    }
			}
		      updatefVal(A[i][j]);
		
		
		
		    }
		}
	    }
      	}
      A[pq.top().current.y][pq.top().current.x].visited=0;
      //std::cout<<"Next popped is: "<<pq.top().current;
      a.at<cv::Vec3b>(pq.top().current.y,pq.top().current.x)[0]=255;
      cv::imshow("Path!", a);
      cv::waitKey(1);
      pq.pop();
    }
  std::cout<<"Out of loop\n";
  cv::Point daddy=A[end.y][end.x].current;
  while(daddy!=A[start.y][start.x].current)
    {
      a.at<cv::Vec3b>(daddy.y, daddy.x)[1]=255;
      daddy=A[daddy.y][daddy.x].parent;
    }
  cv::imshow("Path!", a);
  cv::waitKey(0);
  
}
		
		
	       
		
		
		
