#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

const cv::String usage = 
	"This is a convertor for jpeg file set to a mp4 file.\n"
        "Usage: jpegs2mp4\n"
        "     <path_to_jpegs>       # a path to a set of input jpeg images of the same size.\n"
        "     <output_file_name>    # the output mp4 file path\n"
        "     [-f=<fps_val>]        # fps for output video, 20 by default\n"
        "     [-c=<clip_rectangle>] # clip subframe (ROI), whole image by default\n"
	"                           # ROI specified by left-top point and right-bottom\n"
	"                           # 0,0 corresponds to left-top of the image\n"
        "     [-t=<show_file_name>] # show jpeg filename last digits in the point,nothing by default\n"
        "                           # its may be usefull, if jpeg filnames last digits specify\n"
	"                           # time in the form ...YYYYMMDDHHMMSS or ...YYYYMMDDHHMM\n"
	"\nAvoide spaces in a parameter description.\nExamples:\n"
        "\nConvert jpgs to a mp4 file.\n"
        "   jpegs2mp4  './2017/7/30/*jpg' day.mp4\n"
        "\nSpecify fps, rectange of interest and point for timestamps:\n"
	"   jpegs2mp4  './2017/7/30/*jpg' day.mp4 -f=5 -roi=210,350,1000,1200 -t=20,40 \n\n";
	

void split(cv::String &s, char c, std::vector<int> &v){
    cv::String::size_type i=0, j=s.find(c);
    while(j!=cv::String::npos){
    	v.push_back(std::stoi(s.substr(i,j-i)));
	i = ++j;
	j = s.find(c,j);
    }
    v.push_back(std::stoi(s.substr(i,s.length())));
}


int main( int argc, char** argv ) {
  //
  std::vector<cv::String> fn;
  std::vector<int> rect_v, time_v;
  cv::Mat frame,roi;
  cv::Rect bx;   //cv::Rect bx(cv::Point(0,400),cv::Point(1920,800));
  cv::Point textPos;  //cv::Point textPos(10,20);
  cv::VideoWriter writer;
  cv::String input, output, curr, rect_s, time_s;
  int fps;

  const cv::String keys =
        "{help h usage ? |    |                              }"
        "{fps f          | 20 | fps for output video         }"
        "{rect roi clip r|    | clip subframe                }"
        "{time pos t     |    | print file name last digits  }"
	"{@images        |    | path+template to jpeg images }"
        "{@output        |    | path to result mp4 file      }";

  cv::CommandLineParser parser(argc, argv, keys);
  if ( parser.has("help") ){
        std::cout << usage;
        return 0;
  }
  if(!parser.has("@images") || !parser.has("@output") ){
    std::cout << usage;
    return -1;
  }
  
  fps=parser.get<int>( "f" );
  input = parser.get<cv::String>("@images");
  output = parser.get<cv::String>("@output");

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
    bx=	cv::Rect(cv::Point(0,0),cv::Point(frame.cols,frame.rows));
 
  //writer.open( output, CV_FOURCC('A','V','C','4'), fps, bx.size() );
  writer.open( output, 0x00000021, fps, bx.size() );

  for (size_t k=1; (k<fn.size()) && !frame.empty(); k++ )
  {
     if(rect_v.size()==4)
       roi = cv::Mat(frame, bx);
     else 
       roi = frame;
     
     if(time_v.size()==2){
       cv::String::size_type i=fn[k-1].find_last_of('.');
       cv::String s=fn[k-1].substr(i-10,2)+":"+fn[k-1].substr(i-8,2)+":"+fn[k-1].substr(i-6,2)+":"+fn[k-1].substr(i-4,2)+":"+fn[k-1].substr(i-2,2);
       cv::putText( roi, s, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,255,255), 1, cv::LINE_AA );
     }

     writer << roi;
   
     frame = cv::imread(fn[k]);
   }
   
   return 0;
}
