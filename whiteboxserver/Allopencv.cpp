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
//string img_str("startimage.jpg");// 처음 이미지
//string img_str2("Endimage.jpg");// 마지막 이미지 
bool mouse_is_pressing = false;
int start_x = -1, start_y = -1;
Mat img_color;
static int i = 0;
int startx[10], starty[10], endx[10], endy[10];
Allopencv allopencv;

void Allopencv::Run() {
	Socket soc;
	cout << "socket 통신 실행" << endl;
	soc.Run_socket();
}
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

void mouse_callback(int event, int x, int y, int flags, void *userdata) {
	Mat img_result = img_color.clone();

	if (event == EVENT_LBUTTONDOWN) {
		mouse_is_pressing = true;
		cout << "처음" << start_x << "," << start_y << "," << x << "," << y << endl;
		start_x = x;
		start_y = y;
		startx[i] = (int)x;
		starty[i] = (int)y;
		cout << "처음2," << start_x << "," << start_y << "," << x << "," << y << endl;
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

	}

}

void Allopencv::set_i_num(int count) {
	i_num = count;
}
void Allopencv::location(int x1, int y1, int x2, int y2, int count) {

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
	}
}

void Allopencv::RoiImg(string img_str) {
	img_color = imread(img_str, IMREAD_COLOR);
	imshow("img_color", img_color);
	setMouseCallback("img_color", mouse_callback);
	waitKey(0);
	destroyAllWindows();

}
void Allopencv::search() {
	search_2();
	for (int d = 0; d < i; d++) {
		cout << d << "번째:" << allopencv.print_area_startx(d) << "," << allopencv.print_area_starty(d) << "," << allopencv.print_area_endx(d) << "," << allopencv.print_area_endy(d) << endl;
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
		//cout << "Correl distance(큰값이 유사) = " << dCorrel << endl;
		//cout << "Correl distance(큰값이 유사) = " << check_dC << endl;
	}
	//imshow("srcimage", srcImage[0]);
	//imshow("srcimage2", srcImage[1]);
	waitKey(0);
	destroyAllWindows();

}