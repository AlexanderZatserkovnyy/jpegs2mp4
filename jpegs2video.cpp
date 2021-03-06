#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

bool pressed=false;
bool drawrect = false;

//cv::Rect bx(cv::Point(0,0),cv::Point(1920,1080));
cv::Point lu(0,0),rb(0,0);
cv::Rect bx;   
cv::Rect bx_backup;

const cv::String usage =
        "This is a video viewer for jpeg files set.\n"
        "Usage: jpegs2video\n"
        "     <path_to_jpegs>       # a path to a set of input jpeg images of the same size.\n"
        "     [-f=<fps_val>]        # fake fps for video, 20 by default\n"
        "     [-c=<clip_rectangle>] # clip subframe (ROI), whole image by default\n"
        "                           # ROI specified by left-top point and right-bottom\n"
        "                           # 0,0 corresponds to left-top of the image\n"
        "     [-t=<show_file_name>] # show jpeg filename last digits in the point,nothing by default\n"
        "                           # its may be usefull, if jpeg filnames last digits specify\n"
        "                           # time in the form ...YYYYMMDDHHMMSS or ...YYYYMMDDHHMM\n"
        "\nAvoide spaces in a parameter description.\nExamples:\n"
        "\nView jpgs video.\n"
        "   jpegs2video  './2017/7/30/*jpg'\n"
        "\nSpecify fps, rectange of interest and point for timestamps:\n"
        "   jpegs2video  './2017/7/30/*jpg' -f=10 -roi=210,350,1000,800 -t=20,40 \n\n";

void split(cv::String &s, char c, std::vector<int> &v){
    cv::String::size_type i=0, j=s.find(c);
    while(j!=cv::String::npos){
        v.push_back(std::stoi(s.substr(i,j-i)));
        i = ++j;
        j = s.find(c,j);
    }
    v.push_back(std::stoi(s.substr(i,s.length())));
}

void onMouse(int event, int x, int y, int flags, void* param)
{
    //cv::Mat &img = *((cv::Mat*)(param)); // 1st cast it back, then deref

    if(event==cv::EVENT_LBUTTONDOWN)
    {
        lu=cv::Point(x,y);
	rb=lu;
        pressed = true;
    }

    if(event==cv::EVENT_LBUTTONUP)
    {
        rb=cv::Point(x,y);
	bx=cv::Rect(lu,rb); 
        pressed=false;
    }

    if((event==cv::EVENT_MOUSEMOVE)&&pressed){
        rb=cv::Point(x,y); 
    }

    if(event==cv::EVENT_RBUTTONDOWN)
    {
        bx=bx_backup;
    }
}

int main( int argc, char** argv ) {
  //
  std::vector<cv::String> fn;
  std::vector<int> rect_v, time_v;
  cv::Mat frame,roi;
  cv::Point textPos;  //cv::Point textPos(10,20);
  cv::String input, curr, rect_s, time_s;
  int fps;

  const cv::String keys =
        "{help h usage ? |    |                              }"
        "{fps f          | 20 | fps for video                }"
        "{rect roi clip r|    | clip subframe                }"
        "{time pos t     |    | print file name last digits  }"
        "{@images        |    | path+template to jpeg images }";

  cv::CommandLineParser parser(argc, argv, keys);
  if ( parser.has("help") ){
        std::cout << usage;
        return 0;
  }
  if(!parser.has("@images")){
    std::cout << usage;
    return -1;
  }

  fps=parser.get<int>( "f" );
  input = parser.get<cv::String>("@images");

  if(parser.has("rect")){
    rect_s=parser.get<cv::String>("rect");
  }
  if(parser.has("time")){
    time_s=parser.get<cv::String>("time");
  }
  if (!parser.check())
  {
    parser.printErrors();
    return -1;
  }

  if(rect_s.length()>1){
    split(rect_s,',',rect_v);
    if(rect_v.size()==4)
      bx = cv::Rect(cv::Point(rect_v[0],rect_v[1]),cv::Point(rect_v[2],rect_v[3]));
  }
  if(time_s.length()>1){
    split(time_s,',',time_v);
    if(time_v.size()==2)
      textPos = cv::Point(time_v[0],time_v[1]);
  }
  ///////////////////////////////////////////////

  cv::glob(input,fn);
  if (!fn.size()) {
        std::cerr << "ERROR! Unable to find files\n";
        return -1;
  }

  frame = cv::imread(fn[0]);
  if (frame.empty())  return -1;

  if(rect_v.size()!=4)
    bx= cv::Rect(cv::Point(0,0),cv::Point(frame.cols,frame.rows));

  bx_backup=bx;
  cv::namedWindow( "Video", cv::WINDOW_NORMAL );
  cv::setMouseCallback( "Video", onMouse, &roi);

  int w = (int) 1000.0/((double) fps);

  for (size_t k=1; (k<fn.size()) && !frame.empty(); k++ )
  {
     roi = cv::Mat(frame, bx);    

     if(pressed) cv::rectangle(roi,lu,rb,cv::Scalar(0,0,255)); 

     if(time_v.size()==2){
       cv::String::size_type i=fn[k-1].find_last_of('.');
       cv::String s=fn[k-1].substr(i-10,2)+":"+fn[k-1].substr(i-8,2)+":"+fn[k-1].substr(i-6,2)+":"+fn[k-1].substr(i-4,2)+":"+fn[k-1].substr(i-2,2);
       cv::putText( roi, s, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,255,255), 1, cv::LINE_AA );
     }

     cv::imshow("Video", roi);
     char c = (char) cv::waitKey(w);
     if ( c==27 || c=='q' || c=='Q' ) break;
   
     frame = cv::imread(fn[k]);
   }
   
    return 0;
}
