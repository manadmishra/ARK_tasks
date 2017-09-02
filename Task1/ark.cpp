#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <queue>
#include <climits>
#include <vector>
#include <stdio.h>

// {255,0,0}  = 20 -1
// {0,255,0}  = 15 -2
// {0,0,255}   = 12 -3
// {128,128,0}  = 10 -4
// {0,128,128}  = 09 -5


bool isValid(int i, int j)        //Define isValid function
{
  if( i<0 || j<0 || i > 19 || j > 19 )
    return 0;
  else
    return 1;
}

struct coordinate               //Define coordinate data type
{
  int ival;
  int jval;
};
struct pixelVal                 //Define data type for each nodes
{
  int gVal=INT_MAX;
  int cost=0;
  int Colour=0;
  coordinate parent;
  cv::Point current;
  int visited = 1;
  coordinate self;
};

class compare                  //Compare function for priority queue
{
public:
  bool operator()(const pixelVal& l, const pixelVal& r)
  {
    return l.gVal>r.gVal;
  }
};

int main()
{
  
  cv::Mat b=cv::imread("pic1.jpg",1);                   //Read image
  cv::Mat a,c,g,h;
  cv::dilate(b, c, cv::Mat(), cv::Point(-1, -1),2);     //dilate and erode to remove noise 
  cv::erode(c, a, cv::Mat(), cv::Point(-1, -1),2);

  cv::dilate(b, g, cv::Mat(), cv::Point(-1, -1),1);   
  //cv::erode(g, h, cv::Mat(), cv::Point(-1, -1),2);


  pixelVal node[20][20];                                //Define 2-D array to store nodes

  int count_s=0, count_e=0;                             //find start and end coordinates
  cv::Point start(0), end(0);

  for(int i=0; i<a.rows; i++)
    {
      for(int j=0; j<a.cols; j++)
  	{
  	  if(a.at<cv::Vec3b>(i,j)[2]==255 && a.at<cv::Vec3b>(i,j)[1]==255 && a.at<cv::Vec3b>(i,j)[0]==0)
  	    {
  	      count_s++;
  	      start.x+=j;
  	      start.y+=i;
  	    }
  	  else if(a.at<cv::Vec3b>(i,j)[2]==0 && a.at<cv::Vec3b>(i,j)[1]==255 && a.at<cv::Vec3b>(i,j)[0]==255)
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


  // printf("start=(%d,%d)\n",start.x,start.y);
  // printf("end=(%d,%d)\n",end.x,end.y);

  int e=70; //Error
  int X[2];
  
  for(int j=5,count=0;count<2;j++)        //Find separation b/w consecutive nodes
    {
      if(g.at<cv::Vec3b>(0,j)[0]<e && g.at<cv::Vec3b>(0,j)[1]<e && g.at<cv::Vec3b>(0,j)[2]<e && g.at<cv::Vec3b>(0,j+1)[0]>255-e && g.at<cv::Vec3b>(0,j+1)[1]>255-e && g.at<cv::Vec3b>(0,j+1)[2]>255-e)
	{
	  X[count]=j;
	  count++;
	}
    }

  //printf("%d\n\n\n",X[1]-X[0]);
  int width=X[1]-X[0];
  int n=b.cols/width;                    //total nodes in a row/col

  //printf("%d\n\n",n);



  int i,j,k,l,colour,p=0,q=0;                                        //Find cost for each node
  
  for(i=102,p=0;i<b.rows;i=i+width,p++)
    {      
      for(j=102,q=0;j<b.cols;j=j+width,q++)
  	{
	  colour=-1;

	  for(k=i-25;k<=i+25;k++)
  	    {
  	      for(l=j-25;l<=j+25;l++)
  		{
		  
		  if((a.at<cv::Vec3b>(k,l)[0]>=255-e) && a.at<cv::Vec3b>(k,l)[1]<=e && a.at<cv::Vec3b>(k,l)[2]<=e)
  		    {
  		      colour=1;
		      //printf("1");
		      // node[p][q].current.x=j;
		      // node[p][q].current.y=i;
		      node[p][q].cost=20;
		      node[p][q].Colour=1;

  		      continue;
		      // printf("1");
  		    }
  		  if(a.at<cv::Vec3b>(k,l)[0]<=e && a.at<cv::Vec3b>(k,l)[1]>=255-e && a.at<cv::Vec3b>(k,l)[2]<=e)
  		    {
  		      colour=2;
		      // node[p][q].current.x=j;
		      // node[p][q].current.y=i;
		      node[p][q].cost=15;
		      node[p][q].Colour=2;
		      //printf("2");
  		      continue;
		      //printf("2");
  		    }
  		  if(a.at<cv::Vec3b>(k,l)[0]<=e && a.at<cv::Vec3b>(k,l)[1]<=e && a.at<cv::Vec3b>(k,l)[2]>=255-e)
  		    {
  		      colour=3;
		      // node[p][q].current.x=j;
		      // node[p][q].current.y=i;
		      node[p][q].cost=12;
		      node[p][q].Colour=3;
		      //printf("3");
  		      continue;
		      //printf("3");
		    }
		  if((a.at<cv::Vec3b>(k,l)[0]<=128+e && a.at<cv::Vec3b>(k,l)[0]>=128-e )&& (a.at<cv::Vec3b>(k,l)[1]<=128+e && a.at<cv::Vec3b>(k,l)[1]>=128-e ) && a.at<cv::Vec3b>(k,l)[2]<=255-e)
		    {
		      colour=4;
		      // node[p][q].current.x=j;
		      // node[p][q].current.y=i;
		      node[p][q].cost=10;
		      node[p][q].Colour=e;
		      //printf("4");
		      continue;
		      //printf("4");
		    }
		  if(a.at<cv::Vec3b>(k,l)[0]<=e && (a.at<cv::Vec3b>(k,l)[1]<=128+e && a.at<cv::Vec3b>(k,l)[1]>=128-e ) && (a.at<cv::Vec3b>(k,l)[2]<=128+e && a.at<cv::Vec3b>(k,l)[2]>=128-e ))
		    {
		      colour=5;
		      // node[p][q].current.x=j;
		      // node[p][q].current.y=i;
		      node[p][q].cost=9;
		      node[p][q].Colour=5;
		      //printf("5");
		      continue;
		      //printf("5");
		    }

		}
 
	    }
	}
    }

  for(int i=0;i<=n-1;i++)                                      //Define x and y coordinates for each node
    {
      for(int j=0;j<=n-1;j++)
	{
	  node[i][j].current.x=100+j*width;
	  node[i][j].current.y=100+i*width;
	}
    }

	
	
  node[0][0].cost=0;                                         //set end node cost as 0;
  node[0][0].current.x=end.x;
  node[0][0].current.y=end.y;

  node[n-2][n-2].current.x=start.x;
  node[n-2][n-2].current.y=start.y;
  node[n-2][n-2].gVal=0;

  std::priority_queue<pixelVal, std::vector<pixelVal>, compare > pq;       //Declare priority queue
  
  for(int i=0;i<=n-1;i++)                          //Define i and j value for each nodes
    {
      for(int j=0;j<=n-1;j++)
	{
	  node[i][j].self.ival=i;
	  node[i][j].self.jval=j;
	}
    }

  pq.push(node[n-2][n-2]);                            //start end node in queue                 
  int u,v,g_top;


  while(!pq.empty() && (pq.top().self.ival != 0 || pq.top().self.jval != 0))         //Dijkstra begins
    { 
      u=pq.top().self.ival;
      v=pq.top().self.jval;
      g_top=pq.top().gVal;
      pq.pop();
      node[u][v].visited=0;

      
      
      for(int i=-1; i<=1; i++)
	{
	  for(int j=-1; j<=1; j++)
	    {
	      if(isValid(i+u,j+v))              
		{
		  if(node[i+u][j+v].visited == 1)
		    {
		     
		      if(abs(i)==abs(j))
			{
			  continue;
			}

		      if(abs(i)!=abs(j))
			{
			  if(g_top + node[i+u][j+v].cost < node[i+u][j+v].gVal )
			    {
			      node[i+u][j+v].gVal = g_top + node[i+u][j+v].cost;
			      node[i+u][j+v].parent.ival =u;
			      node[i+u][j+v].parent.jval =v;
			      pq.push(node[i+u][j+v]);

			  
     

			    }
			}
		    }

		}
	    }
	}
	   
    
      
   
    }                                                                 //Dijkstra ends
     


 
	
  coordinate temp;
  temp.ival=node[0][0].self.ival;                                    
  temp.jval=node[0][0].self.jval;
 
  node[n-2][n-2].parent.ival=18;
  node[n-2][n-2].parent.jval=18;
  
  while(temp.ival!=node[n-2][n-2].self.ival || temp.jval!=node[n-2][n-2].self.jval)   //Loop for back tracing path starting from end element
    {
     
      cv::line(g,node[temp.ival][temp.jval].current,node[node[temp.ival][temp.jval].parent.ival][node[temp.ival][temp.jval].parent.jval].current,(0,0,255),5);                      //Draw lines

      temp=node[temp.ival][temp.jval].parent;        //Redefine temp
      		
    }

  cv::imshow("grid", g);
  cv::waitKey(0);
  cv::imwrite("final_path.jpg",g);
	 
  

  return 0;
}
  	

