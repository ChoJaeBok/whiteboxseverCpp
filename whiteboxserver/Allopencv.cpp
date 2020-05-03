#include <iostream>
#include <string>
#include "Allopencv.h"
#include "Socket.h"
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;
bool mouse_is_pressing = false;
int start_x = -1, start_y = -1;
Mat img_color;
static int i = 0;
int startx[15], starty[15], endx[15], endy[15];
Allopencv allopencv;

void Allopencv::Imread(string img_str, string img_str2) {

	//cv::imread(_);
	try
	{
		Mat img = imread(img_str);
		Mat img2 = imread(img_str2);
		cout << "성공" << endl;

		namedWindow("startimage");//startimage라는 이름의 창을 만듬.
		namedWindow("endimage");//endimage라는 이름의 창을 만듬.
		imshow("startimage", img);//startimage라는 창에다가.
		imshow("endimage", img2);//endimage라는 창에다가.
		waitKey();//사용자의 키보드 입력을 기다리는 함수.
	}
	catch (const std::exception&)
	{
		cout << "Image load failed" << endl;
	}
	waitKey(0);
	destroyAllWindows();
}
//mouse_claaback으로 컴퓨터(윈도우)환경에서 진행하여 드래그를 방식으로 진행하였습니다.
void mouse_callback(int event, int x, int y, int flags, void *userdata) {
	Mat img_result = img_color.clone();

	if (event == EVENT_LBUTTONDOWN) {
		mouse_is_pressing = true;
		cout << "처음" << start_x << "," << start_y << "," << x << "," << y << endl;
		start_x = x;
		start_y = y;
		startx[i] = (int)x;
		starty[i] = (int)y;
		cout << "처음2" << start_x << "," << start_y << "," << x << "," << y << endl;
		//circle(img_result, Point(x, y), 10, Scalar(0, 255, 0), -1);
		//imshow("img_color", img_result);
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (mouse_is_pressing) {
			rectangle(img_result, Point(start_x, start_y), Point(x, y), Scalar(0, 255, 0), 3);
			imshow("img_color", img_result);
		}
	}
	else if (event == EVENT_LBUTTONUP) {

		mouse_is_pressing = false;
		endx[i] = (int)x;
		endy[i] = (int)y;
		cout << i << "번째:" << startx[i] << "," << starty[i] << "," << endx[i] << "," << endy[i] << endl;

		i++;
		Mat img_roi(img_color, Rect(start_x, start_y, x - start_x, y - start_y));
		cout << "마지막" << start_x << "," << start_y << "," << x << "," << y << endl;
		img_roi.copyTo(img_result(Rect(start_x, start_y, x - start_x, y - start_y)));
		imshow("img_color", img_result);
		imshow("img_roi", img_roi);
		imwrite("startroi" + to_string(i) + ".jpg", img_roi);//roi 해서 저장하는 부분.
		//이미지 ROI를 한 상태로 저장하는 방식입니다.  
		//이 방식을 택한 이유는 저장한 상태에서 histogram을 할 때
		//저장된 이미지를 순서대로 불러와 비교를 하기 위해 선택하였습니다. 
	}

}

void Allopencv::set_i_num(int count) {
	i_num = count;
}
void Allopencv::location(int x1, int y1, int x2, int y2, int count) {
	//좌표를 저장하기 위한 함수.
	startx[count] = x1;
	starty[count] = y1;
	endx[count] = x2;
	endy[count] = y2;
}
void search_2() {
	allopencv.set_i_num(i);
	for (int d = 0; d < i; d++) {
		cout << d << "번째(for):" << startx[d] << "," << starty[d] << "," << endx[d] << "," << endy[d] << endl;
		allopencv.location(startx[d], starty[d], endx[d], endy[d], d);
		//첫 이미지에서 좌표를 구했을 때 같은 좌표로 EndROI때 사용하기 위해 저장하는 것입니다. 
	}
}

void Allopencv::RoiImg(string img_str) {
	img_color = imread(img_str, IMREAD_COLOR);
	imshow("img_color", img_color);
	setMouseCallback("img_color", mouse_callback);
	//setMouseCallback 함수는 위의 mousse_callback함수를 이용해 드래그를 하는 역할로
	//좌표를 구한 뒤 ROI를 한 후 이미지를 저장하게 하였습니다. 
	waitKey(0);
	destroyAllWindows();

}
void Allopencv::search() {
	search_2();
	for (int d = 0; d < i; d++) {
		cout << d << "번째:" << allopencv.print_area_startx(d) << "," << allopencv.print_area_starty(d) << "," << allopencv.print_area_endx(d) << "," << allopencv.print_area_endy(d) << endl;
		//좌표가 이상없이 저장되었는지 확인하기 위해 사용하였습니다. 
	}
}

//이부분은 private에 있는 변수 저장해놓은것들을 리턴해오기 위함.
int Allopencv::print_area_startx(int count) {
	return startx[count];
}
int Allopencv::print_area_starty(int count) {
	return starty[count];
}
int Allopencv::print_area_endx(int count) {
	return endx[count];
}
int Allopencv::print_area_endy(int count) {
	return endy[count];
}
int Allopencv::print_i_num() {
	return i_num;
}


void Allopencv::End_RoiImg(string img_str) {
	//End_Roi로 저장해놓은 좌표들을 토대로 마지막 이미지에서 이용해 
	//ROI를 하여 다시 저장하는 방식입니다. 
	Mat img_end = imread(img_str);
	namedWindow("endimage");
	imshow("endimage", img_end);
	cout << "print_i_num:" << allopencv.print_i_num() << endl;
	for (int d = 0; d < i; d++) {
		cout << d << "번째:" << allopencv.print_area_startx(d) << "," << allopencv.print_area_starty(d) << "," << allopencv.print_area_endx(d) << "," << allopencv.print_area_endy(d) << endl;
		Mat img_roi(img_end, Rect(allopencv.print_area_startx(d), allopencv.print_area_starty(d), allopencv.print_area_endx(d) - allopencv.print_area_startx(d), allopencv.print_area_endy(d) - allopencv.print_area_starty(d)));
		imwrite("Endroi" + to_string(d + 1) + ".jpg", img_roi);//roi 해서 저장하는 부분.
	}
	waitKey();//사용자의 키보드 입력을 기다리는 함수.
	destroyAllWindows();
}


void Allopencv::Histogram() {
	Mat srcImage[2];
	for (int count = 0; count < i; count++) {
		//for문을 이용한 것은 좌표매핑한 횟수만큼 자동으로 실행하고 
		//비교를 하기 위해 사용하였습니다. 
		srcImage[0] = imread("startroi" + to_string(count + 1) + ".jpg", IMREAD_GRAYSCALE);
		srcImage[1] = imread("Endroi" + to_string(count + 1) + ".jpg", IMREAD_GRAYSCALE);

		int histSize = 256;
		float range[] = { 0,255 };
		const float *ranges[] = { range };
		Mat hist[2];
		calcHist(&srcImage[0], 1, 0, Mat(), hist[0], 1, &histSize, ranges);
		calcHist(&srcImage[1], 1, 0, Mat(), hist[1], 1, &histSize, ranges);
		double dCorrel = compareHist(hist[0], hist[1], HISTCMP_CORREL);
		double check_dC = dCorrel * 10;
		if (check_dC > 7.0) {
			cout << "no detech" << check_dC << endl;
		}
		else if (check_dC <= 7.0) {
			cout << "detech" << check_dC << endl;
		}
		
	}
	waitKey(0);
	destroyAllWindows();

}