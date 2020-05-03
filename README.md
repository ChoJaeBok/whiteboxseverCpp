## whiteboxseverCpp   
![시스템구성도(수정2)](https://user-images.githubusercontent.com/60215726/80072142-ca073700-8580-11ea-8667-8c79b89a1a78.png) 
 졸업작품을 주제로 한 whitebox(https://github.com/ChoJaeBok/whiteboxproject) 에서 서버와 영상처리를 담당하는 부분에 대한 것을 python 버전에서 C++버전으로 다시 만든 것입니다. 또한 python을 진행하였을 당시에는 라즈베리파이 b3에서 진행하였지만 C++버전은 컴퓨터(윈도우)환경에서 진행하였습니다. 또한 클라이언트는 안드로이드가 아닌 같은 C++로 제작하여 진행하였습니다.    
기존에 만들었던 작품에서 진행하는 순서는 비슷하나 부족했던 부분들은 추가할 예정입니다.   
(클라이언트에서 서버로 이미지 파일을 전송한 다음 서버에서는 그 이미지 파일을 이용하는 방식을 추가하였습니다.)
 

### 1.코드
 현재 진행된 python 코드에서 C++로 만든 것이며 추가 된 부분은 공부 중에 있어 업로드를 하지 못하였습니다.
 opencv버전은 openv 4.2.0이며 visual studio 2017을 사용하였습니다.  
#### 1) opencv에 대한 코드 
```cpp
#include <string>
#ifndef ALLOPENCV_H
#define ALLOPENCV_H
#define MAX 15
class Allopencv {
public:
	void Imread(std::string img_str, std::string img_str2);//이미지 불러오는 부분
	void RoiImg(std::string img_str); //start imageROI
	void End_RoiImg(std::string img_str);//end imageROI
	void search(); //확인 및 endroi할 좌표 저장.(location을 이용)
	void location(int x1, int y1, int x2, int y2, int count); // 좌표 저장하는 부분
	void set_i_num(int i);
	//아래는 private에 있는 좌표와 i_num을 접근하기 위함.
	int print_area_startx(int count);
	int print_area_starty(int count);
	int print_area_endx(int count);
	int print_area_endy(int count);
	int print_i_num();
	void Histogram();//histogram 하기위한.

private:
	int startx[MAX];//이미지에서 왼쪽 위 찍은 x좌표
	int starty[MAX];//이미지에서 왼쪽 위 찍은 y좌표
	int endx[MAX];//이미지에서 오른쪽 아래 찍은 x좌표
	int endy[MAX];//이미지에서 오른쪽 아래 찍은 y좌표
	int i_num;// 좌석매핑한 횟수
 //ROI하기 위한 좌표들을 저장해놓는 공간으로 startx, starty은 처음에 찍힌 좌표들이며 endx,endy는 마지막에 찍힌 좌표들입니다.
	//배열은 0은 첫번째 ROI, 1은 두번째 ROI... 이런식의 저장하기 위해 사용.
};
#endif // !ALLOPENCV_H
```
opencv를 담당하는 Allopencv.h 헤더파일입니다.
###### (1) ROI
ROI로는 첫 이미지에서 ROI를 해주는 RoiImg와 비교할 이미지에서 Roi를 해주는 End_RoiImg가 있습니다. 
RoiImg에서는 좌표를 구할 때 마우스 드래그를 구현하는 방식으로 진행하였습니다.   
```cpp
bool mouse_is_pressing = false;
int start_x = -1, start_y = -1;
Mat img_color;
static int i = 0;
int startx[15], starty[15], endx[15], endy[15];
void Allopencv::RoiImg(string img_str) {
	img_color = imread(img_str, IMREAD_COLOR);
	imshow("img_color", img_color);
	setMouseCallback("img_color", mouse_callback);
	//setMouseCallback 함수는 위의 mousse_callback함수를 이용해 드래그를 하는 역할로
	//좌표를 구한 뒤 ROI를 한 후 이미지를 저장하게 하였습니다. 
	waitKey(0);
	destroyAllWindows();

}
```
여기서 이미지를 불러와 setMouseCallback에서 mouse_callback을 통하여 드래그를 하고 좌표를 구합니다. 
```cpp
void mouse_callback(int event, int x, int y, int flags, void *userdata) {
	Mat img_result = img_color.clone();

	if (event == EVENT_LBUTTONDOWN) {
  //첫 마우스 클릭할 때
		mouse_is_pressing = true;
		start_x = x;
		start_y = y;
		startx[i] = (int)x;
		starty[i] = (int)y;
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (mouse_is_pressing) {
   //마우스 움직이는동안. 
			rectangle(img_result, Point(start_x, start_y), Point(x, y), Scalar(0, 255, 0), 3);
			imshow("img_color", img_result);
		}
	}
	else if (event == EVENT_LBUTTONUP) {
  //마우스 클릭이 올라갔을때입니다. 
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
```
여기서 마우스 눌렀을 때 startx와 starty가 저장되며 마우스를 올렸을 때 endx와 endy가 저장됩니다. 그리고 imwrite를 통해 startroi+ 눌린 횟수 의 이름으로 이미지 jpg 파일로 저장하게 됩니다.  실행 화면 입니다.
![ROI](https://user-images.githubusercontent.com/60215726/80211852-6872c580-8671-11ea-944d-c3133e323cfe.png)

이후 첫이미지에서 추출된 End_Roi에서 사용하기 위해 좌표를 저장을 하기 위해 location 함수를 이용했습니다. 
```cpp
Allopencv allopencv;
void Allopencv::location(int x1, int y1, int x2, int y2, int count) {
	//좌표를 저장하기 위한 함수.
	startx[count] = x1;
	starty[count] = y1;
	endx[count] = x2;
	endy[count] = y2;
}
void search_2() {
	allopencv.set_i_num(i);// 좌석 매핑한 횟수 저장.
	for (int d = 0; d < i; d++) {
		cout << d << "번째(for):" << startx[d] << "," << starty[d] << "," << endx[d] << "," << endy[d] << endl;
		allopencv.location(startx[d], starty[d], endx[d], endy[d], d);
		//첫 이미지에서 좌표를 구했을 때 같은 좌표로 EndROI때 사용하기 위해 저장하는 것입니다. 
  //마지막의 d는 배열에서의 인덱스로서 첫번째 이미지의 좌표, 두번째 이미지의 좌표,... 로 구분하기 위해 사용하였습니다. 
	}
}
 void Allopencv::search() {
	search_2();
	for (int d = 0; d < i; d++) {
		cout << d << "번째:" << allopencv.print_area_startx(d) << "," << allopencv.print_area_starty(d) << "," << allopencv.print_area_endx(d) << "," << allopencv.print_area_endy(d) << endl;
		//좌표가 이상없이 저장되었는지 확인하기 위해 사용하였습니다. 
	 }
 }
```
search 함수와 search_2함수는 기존의 있는 좌표들이 저장이 정확하게 되었는지 확인하기 위해 추가하였습니다.
이후에 End_RoiImg를 실행하여 비교할 이미지에서 ROI를 하였습니다. 
```cpp
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
```
![EndROI](https://user-images.githubusercontent.com/60215726/80211855-69a3f280-8671-11ea-8db2-929f2ee4678b.png)

 ![save](https://user-images.githubusercontent.com/60215726/80212260-1ed6aa80-8672-11ea-9bc9-3d5437251167.PNG)   
 EndRoi 실행 화면이며 아래와 같이 Endroi를 저장한 후 폴더에 저장된 이미지입니다. startroi는 ROI 함수에서 시작할 때 저장됩니다.    
    
###### (2) Histogram
히스토그램을 사용하는 함수로 아이가 탐지가 되었는지를 구별하기 위해 사용하였습니다.   
처음 이미지와 비교할 이미지의 각각의 calcHist 명령어를 통해 히스토그램을 계산을 하고 CompareHist 명령어를 통하여 비교한 수치 값과 기준점을 비교하여 아이가 탐지 여부를 파악하였습니다.   
```cpp
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
  //여기 부분은 dCorrel은 1이 나오게 되면 이미지가 같은 것임을 나타내고 
  //낮으면 낮아질수록 유사도가 떨어지게 됩니다. 
  //check_dc는 10*dCorrel로 7.0 이상이면 이미지끼리 많은 유사를 나타나기 때문에 no detech를 정의하였고
  //7.0보다 낮게 되면 아이나 물건이 있다는 것을 하여 detech로 정의하였습니다. 
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
```
실행하게 되면 아래 이미지에서 Recv Msg: his의 아래 histogram 실행이 되며 결과를 나타냅니다. 
![his](https://user-images.githubusercontent.com/60215726/80211856-6ad51f80-8671-11ea-9c4c-22abf169a003.png)

   
###### (3) Allopencv.cpp 전체 코드 
```cpp
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
```
   
#### 1) socket에 대한 코드    
##### (1) server
```cpp
//Socket.h
#ifndef SOCLET_H
#define SOCLET_H

class Socket {
public:
	void Run_socket();//소켓통신을 위한.
	
};

#endif
```
위에는 Socket.h로 아래는 Socket.cpp 입니다. 
```cpp
#include <iostream>
#include <WinSock2.h>
#include "Socket.h"
#include "Allopencv.h"

#pragma comment(lib, "ws2_32")

#define PORT 8888
#define PACKET_SIZE 1024
using namespace std;
Allopencv all;
//현재는 이렇게 이미지를 저장된 것을 사용하지만 추후에 이미지를 통신으로 받아와 할 예정입니다. 
string img_str("startimage.jpg");// 처음 이미지
string img_str2("Endimage.jpg");// 마지막 이미지 
void Socket::Run_socket() {
	WSADATA wsaData;//windows의 소켓초기화정보를 저장하기 위한 구조체. 이미 선언되어있는 구조체 
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = AF_INET;
	tListenAddr.sin_port = htons(PORT);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	//bind(소켓, 소켓 구성요소 구조체의 주소, 그 구조체의 크기);
	listen(hListen, SOMAXCONN);
	//연결을 수신하는 상태로 소켓의 상태를 변경한다. 즉, 소켓을 접속대기 상태로 만들어준다.
	//SOMAXCONN은 한꺼번에 요청 가능한 최대 접속승인 수를 의미.
	
	while (true) {
		SOCKADDR_IN tClntAddr = {};
		int iClntSize = sizeof(tClntAddr);
		SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);
		//클라이언트 측 소켓 생성 및 정보를 담을 구조체 생성 및 값 할당, 클라이언트가 접속 요청하면 승인해주는 역할
		//accept(소켓, 소켓 구성요소 주소체의 주소, 그 구조체의 크기를 담고있는 변수의 주소);
		//이 함수를 이용하여 접속 요청을 수락. 이 함수는 동기화된 방식으로 동작된다.
		//즉 요청이 들어오기 전까지 이 함수를 안빠져나옴.

		char cBuffer[PACKET_SIZE] = {};

		//클라이언트측 정보를 수신하기 위해 기존에 정의해둔 패킷 크기만큼 버퍼를 생성
		recv(hClient, cBuffer, PACKET_SIZE, 0);
		//recv(소켓, 수신 정보를 담을 배열주소, 그 배열의 크기, flag);
		cout << "Recv Msg: " << cBuffer << endl;
		char cMsg[] = "StartROI";
		if (strcmp(cBuffer, "ROI") == 0) {
			strcpy_s(cMsg, "StartROI");
			send(hClient, cMsg, strlen(cMsg), 0);
			cout << "ROI실행" << endl;
			all.RoiImg(img_str);
			cout << "확인" << endl;
			all.search();
			send(hClient, cMsg, strlen(cMsg), 0);
		}
		else if (strcmp(cBuffer, "EndROI") == 0) {
			strcpy_s(cMsg, "EndROI");
			cout << "endROI실행" << endl;
			all.End_RoiImg(img_str2);
			send(hClient, cMsg, strlen(cMsg), 0);
		}
		else if (strcmp(cBuffer, "His") == 0) {
			strcpy_s(cMsg, "His");
			cout << "histogram 실행" << endl;
			all.Histogram();
			send(hClient, cMsg, strlen(cMsg), 0);
		}
		else if (strcmp(cBuffer, "load") == 0) {
			strcpy_s(cMsg, "load");
			cout << "이미지 불러오기 실행" << endl;
			all.Imread(img_str, img_str2);
			send(hClient, cMsg, strlen(cMsg), 0);
		}
		else if (strcmp(cBuffer, "break") == 0) {
			strcpy_s(cMsg, "break");
			cout << "서버 종료" << endl;
			send(hClient, cMsg, strlen(cMsg), 0);
			break;
		}
		else {
			cout << "잘못된 접근" << endl;

		}
		
		closesocket(hClient);
	}

	closesocket(hListen);
	//클라이언트 측으로부터 정보를 받아오고 출력, 클라이언트에 정보 전송

	WSACleanup();//소켓을 활용하는것은 startup과 cleanup 사이에 작성. 생성자와 소멸자 같은 개념
}
```
서버를 담당하는 부분입니다. 
##### (2) client
```cpp
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#pragma comment(lib, "ws2_32")
#define PORT 8888
#define PACKET_SIZE 1024
#define SERVER_IP "IP이므로 가렸습니다"
using namespace std;

int main()
{
	int select_i = 0;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	
	
	
	while (true) {
		SOCKET hSocket;
		hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		SOCKADDR_IN tAddr = {};
		tAddr.sin_family = AF_INET;
		tAddr.sin_port = htons(PORT);
		tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

		connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));
		//connect(소켓, 소켓구성요소 구조체의 주소, 그 구조체의 크기);
		//connect 함수는 지정된 소켓에 연결을 설정해준다. 서버에 연결하기위해 connect함수를 사용
		cout << "숫자 지정:" << endl;

		cin >> select_i;
		char cMsg[] = "EndROI";
		char cBuffer[PACKET_SIZE] = {};
		if (select_i == 1) {
			strcpy_s(cMsg, "ROI");
			send(hSocket, cMsg, strlen(cMsg), 0);
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer<<endl;

		}
		else if (select_i == 2) {
			cout << cMsg<< endl;
			strcpy_s(cMsg, "EndROI");
			send(hSocket, cMsg, strlen(cMsg), 0);
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else if (select_i == 3) {
			strcpy_s(cMsg, "His");
			send(hSocket, cMsg, strlen(cMsg), 0);
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else if (select_i == 4) {
			strcpy_s(cMsg, "load");
			send(hSocket, cMsg, strlen(cMsg), 0);
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else {
			strcpy_s(cMsg, "break");
			send(hSocket, cMsg, strlen(cMsg), 0);
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
			break;
		}
		cout << select_i << endl;
		closesocket(hSocket);
	}


	
	//클라이언트측 코드, 소켓 구성요소 구조체에 접속할 서버의 ip를 적어준다. 클라이언트에서는 bind함수 대신 connect함수를 사용
	WSACleanup();
	return 0;

}
```

자바 클라이언트 대신 여기서는 같은 C++로 클라이언트를 구성하였습니다. 

### 2.코드(새로 추가한 부분)
소켓 통신을 하는 부분에서 클라이언트에서는 이미지를 전송하는 방식에 대해서 실력이 부족하여 구현하지 못했습니다.    
여기서는 클라이언트가 이미지를 통신으로 서버로 파일을 전송해서 서버에서는 그 파일에 대한 것을 ROI와 Histogram 등 이미지에 대한 처리하는 방식을 추가한 부분입니다.    

#### 1) 서버
```cpp
//Socket.h
public:
	void insertimg(string img1);// 이미지 변수 채우기 위한.
	string print_firimg();
	string print_secimg();
	string print_noimg();
private:
	string firstimg;
	string secondimg;
	string noimg;
```
지난 코드와 다르게 여기서는 파일명에 대한 변수를 추가하였고 보내오는 파일 중 이름을 처음 이미지인 start가 포함된 이미지와 비교할 이미지 End가 포함된 이미지로 구별하도록 설정하였는데 그 외에 포함되지 않은 이미지는 noimg로 구분하여 다시 수정하여 보내도록 하도록 설정하였습니다.    
간단하게 생각하면 start를 포함하는 이미지 명은 firstimg에 들어가고 End가 포함된 이미지 명은 secondimg에 들어가도록 한 것이며, 구별이 안 되는 이미지 명은 noimg에 들어가서 오류를 검출하기 위해 사용한 것입니다. 

```cpp
//Socket.cpp
#define PACKET_SIZE 4096
int datReceive(SOCKET s, char* buf, int len, int flags) {
	//데이터 수신 함수
	int received;
	char *ptr = buf;
	int left = len;
	//남아있는 데이터가 있는 경우 반복적으로 시행
	while (left > 0) {
		received = recv(s, ptr, left, flags);
		//더이상 데이터를 받아오지 못하는 경우
		if (received == 0) {
			break;
		}
		left -= received;
		ptr += received;
	}
	//더 받을 수 있는 데이터 길이를 반환 (데이터를 한번이라도 받았으면 0이 아닌 수를 반환하게 됨.
	return (len - left);
}

void Socket::insertimg(string img1) {
	if (img1.find("start") != string::npos) {
		//start라는 문자가 있을 경우
		firstimg = img1;
	}
	else if (img1.find("End") != string::npos) {
		//End라는 문자가 있을 경우 
		secondimg = img1;
	}
	else {
		noimg = img1;
	}

}
string Socket::print_firimg() {
	return firstimg;
}
string Socket::print_secimg() {
	return secondimg;
}
string Socket::print_noimg() {
	return noimg;
}
```
처음에 지정했던 PACKET_SIZE는 소켓통신만 진행했을 때는 1024로 하였지만 이미지 파일 보낼 때는 더 크게 4096으로 지정해주고 진행하였습니다. 
```cpp
//Socket.cpp 중 void Socket::Run_socket()에서 while문에 추가된 if문입니다.
if (strcmp(cBuffer, "Down") == 0) {
			cout << "클라이언트 접속 : IP=" << inet_ntoa(tClntAddr.sin_addr) << ", PORT=" << ntohs(tClntAddr.sin_port) << endl;
			//파일 이름 받기
			char filename[256];
			ZeroMemory(filename, 256);
			datReceive(hClient, filename, 256, 0);
			cout << "받을 파일 이름 : " << filename << endl;
			socc.insertimg(filename);
			if (socc.print_noimg() != "") {
				//이미지 보내올 때 잘못된 이미지는 사용하지 않도록 하기위해 구현한 부분입니다. 
				cout << "잘못된 이미지 파일입니다. 처음이미지는 start를 붙여주시고 비교할 이미지는 End를 붙여주세요!" << endl;
				socc.insertimg("");
				continue;
			}
			//파일 크기 받기
			int totalbytes;
			datReceive(hClient, (char *)&totalbytes, sizeof(totalbytes), 0);
			//받을 파일 크기 출력
			cout << "받을 파일 크기 :" << totalbytes << endl;
			//파일 열기
			FILE *fp = fopen(filename, "wb");
			//파일 데이터 받기
			int numtotal = 0;
			while (1) {
				retval = datReceive(hClient, buf, PACKET_SIZE, 0);
				//더 받을 데이터가 없을때
				if (retval == 0) {
					break;
				}
				else {
					fwrite(buf, 1, retval, fp);
					//받을데이터 크기만큼 변수에 더해줌
					numtotal += retval;
				}
			}
			fclose(fp);

			//전송 결과
			if (numtotal == totalbytes) {
				cout << "파일 성공" << endl;
			}
			else { cout << "파일 실패" << endl; }
		}
```
이미지 파일을 받기 위해서 추가된 부분입니다. 아래는 추가 후에 Socket.cpp의 전체 코드 입니다.   
```cpp
#include <iostream>
#include <WinSock2.h>
#include "Socket.h"
#include "Allopencv.h"


#define PORT 8888
#define PACKET_SIZE 4096
//경고 제어
#pragma warning(disable:4996)
//원속 사용을 위한 라이브러리 링킹
#pragma comment(lib, "ws2_32")
using namespace std;
Allopencv all;
Socket socc;
int datReceive(SOCKET s, char* buf, int len, int flags) {
	//데이터 수신 함수
	int received;
	char *ptr = buf;
	int left = len;
	//남아있는 데이터가 있는 경우 반복적으로 시행
	while (left > 0) {
		received = recv(s, ptr, left, flags);
		//더이상 데이터를 받아오지 못하는 경우
		if (received == 0) {
			break;
		}
		left -= received;
		ptr += received;
	}
	//더 받을 수 있는 데이터 길이를 반환 (데이터를 한번이라도 받았으면 0이 아닌 수를 반환하게 됨.
	return (len - left);
}
void Socket::insertimg(string img1) {
	if (img1.find("start") != string::npos) {
		//start라는 문자가 있을 경우
		firstimg = img1;
	}
	else if (img1.find("End") != string::npos) {
		//End라는 문자가 있을 경우 
		secondimg = img1;
	}
	else {
		noimg = img1;
	}

}
string Socket::print_firimg() {
	return firstimg;
}
string Socket::print_secimg() {
	return secondimg;
}
string Socket::print_noimg() {
	return noimg;
}
void Socket::Run_socket() {

	//데이터 크기를 담을 변수
	int retval;

	WSADATA wsaData;//windows의 소켓초기화정보를 저장하기 위한 구조체. 이미 선언되어있는 구조체 
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tListenAddr = {};
	ZeroMemory(&tListenAddr, sizeof(tListenAddr));
	tListenAddr.sin_family = AF_INET;
	tListenAddr.sin_port = htons(PORT);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	//bind(소켓, 소켓 구성요소 구조체의 주소, 그 구조체의 크기);
	listen(hListen, SOMAXCONN);
	//연결을 수신하는 상태로 소켓의 상태를 변경한다. 즉, 소켓을 접속대기 상태로 만들어준다.
	//SOMAXCONN은 한꺼번에 요청 가능한 최대 접속승인 수를 의미.
	//데이터 통신에 사용할 변수 
	SOCKET hClient;
	SOCKADDR_IN tClntAddr;
	int iClntSize;
	char buf[PACKET_SIZE];
	while (true) {
		tClntAddr = {};
		iClntSize = sizeof(tClntAddr);
		hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);
		//클라이언트 측 소켓 생성 및 정보를 담을 구조체 생성 및 값 할당, 클라이언트가 접속 요청하면 승인해주는 역할
		//accept(소켓, 소켓 구성요소 주소체의 주소, 그 구조체의 크기를 담고있는 변수의 주소);
		//이 함수를 이용하여 접속 요청을 수락. 이 함수는 동기화된 방식으로 동작된다.
		//즉 요청이 들어오기 전까지 이 함수를 안빠져나옴.

		char cBuffer[PACKET_SIZE] = {};
		//클라이언트측 정보를 수신하기 위해 기존에 정의해둔 패킷 크기만큼 버퍼를 생성
		recv(hClient, cBuffer, PACKET_SIZE, 0);
		//recv(소켓, 수신 정보를 담을 배열주소, 그 배열의 크기, flag);
		cout << "Recv Msg: " << cBuffer << endl;
		char cMsg[] = "StartROI  ";

		if (strcmp(cBuffer, "Down") == 0) {
			cout << "클라이언트 접속 : IP=" << inet_ntoa(tClntAddr.sin_addr) << ", PORT=" << ntohs(tClntAddr.sin_port) << endl;
			//파일 이름 받기
			char filename[256];
			ZeroMemory(filename, 256);
			datReceive(hClient, filename, 256, 0);
			cout << "받을 파일 이름 : " << filename << endl;
			socc.insertimg(filename);
			if (socc.print_noimg() != "") {
				//이미지 보내올 때 잘못된 이미지는 사용하지 않도록 하기위해 구현한 부분입니다. 
				cout << "잘못된 이미지 파일입니다. 처음이미지는 start를 붙여주시고 비교할 이미지는 End를 붙여주세요!" << endl;
				socc.insertimg("");
				continue;
			}
			//파일 크기 받기
			int totalbytes;
			datReceive(hClient, (char *)&totalbytes, sizeof(totalbytes), 0);
			//받을 파일 크기 출력
			cout << "받을 파일 크기 :" << totalbytes << endl;
			//파일 열기
			FILE *fp = fopen(filename, "wb");
			//파일 데이터 받기
			int numtotal = 0;
			while (1) {
				retval = datReceive(hClient, buf, PACKET_SIZE, 0);
				//더 받을 데이터가 없을때
				if (retval == 0) {
					break;
				}
				else {
					fwrite(buf, 1, retval, fp);
					//받을데이터 크기만큼 변수에 더해줌
					numtotal += retval;
				}
			}
			fclose(fp);

			//전송 결과
			if (numtotal == totalbytes) {
				cout << "파일 성공" << endl;
			}
			else { cout << "파일 실패" << endl; }
		}

		else if (strcmp(cBuffer, "ROI") == 0) {
			strcpy_s(cMsg, "StartROI");
			send(hClient, cMsg, strlen(cMsg), 0);
			cout << "ROI실행" << endl;
			all.RoiImg(socc.print_firimg());
			cout << "확인" << endl;
			all.search();
			send(hClient, cMsg, strlen(cMsg), 0);
			
		}
		else if (strcmp(cBuffer, "EndROI") == 0) {
			strcpy_s(cMsg, "EndROI");
			cout << "endROI실행" << endl;
			all.End_RoiImg(socc.print_secimg());
			send(hClient, cMsg, strlen(cMsg), 0);
			
		}
		else if (strcmp(cBuffer, "His") == 0) {
			strcpy_s(cMsg, "His");
			cout << "histogram 실행" << endl;
			all.Histogram();
			send(hClient, cMsg, strlen(cMsg), 0);
			
		}
		else if (strcmp(cBuffer, "load") == 0) {
			strcpy_s(cMsg, "load");
			cout << "이미지 불러오기 실행" << endl;
			all.Imread(socc.print_firimg(), socc.print_secimg());
			send(hClient, cMsg, strlen(cMsg), 0);
			
		}
		else if (strcmp(cBuffer, "break") == 0) {
			strcpy_s(cMsg, "break");
			cout << "서버 종료" << endl;
			send(hClient, cMsg, strlen(cMsg), 0);
			break;
		}
		else {
			cout << "잘못된 접근" << endl;
			

		}

		closesocket(hClient);
	}

	closesocket(hListen);
	//클라이언트 측으로부터 정보를 받아오고 출력, 클라이언트에 정보 전송

	WSACleanup();//소켓을 활용하는것은 startup과 cleanup 사이에 작성. 생성자와 소멸자 같은 개념
}

```
#### 2) 클라이언트 
클라이언트에서 추가된 이미지(파일) 전송하는 코드입니다.
```cpp
			strcpy_s(cMsg, "Down");
			send(hSocket, cMsg, strlen(cMsg), 0);
			//파일 열기
			//보낼 파일 설정
			char myFile[256] = "startimage.jpg";
			FILE *fp = fopen(myFile, "rb");

			//파일 이름 변수
			char filename[256];
			ZeroMemory(filename, 256);
			sprintf(filename, myFile);

			//소켓으로 파일 이름을 전송
			send(hSocket, filename, 256, 0);

			//파일크기 얻기
			fseek(fp, 0, SEEK_END);
			int totalbytes = ftell(fp);

			//파일 크기 소켓으로 보내기
			send(hSocket, (char *)&totalbytes, sizeof(totalbytes), 0);
			//파일 데이터 전송에 사용할 변수
			char buf[PACKET_SIZE];
			int numread;
			int numtotal = 0;

			//파일 포인터를 제일 앞으로 이동
			rewind(fp);
			//반복적으로 파일 데이터 보내기
			while (1) {
				//파일의 내용을 버퍼에 담음
				numread = fread(buf, 1, PACKET_SIZE, fp);

				//파일 데이터가 조금이라도 남은 경우
				if (numread > 0) {
					send(hSocket, buf, numread, 0);
					numtotal += numread;
				}

				//파일을 모두 전송한 경우
				else if (numread == 0 && numtotal == totalbytes) {
					cout << "총" << numtotal << "바이트 파일전송을 완료" << endl;
					break;
				}
			}
```
여기서 이미지 파일 명인 myfile은 따로 입력 받아서 하는 것이 아닌 if문으로 3개의 경우로 만들어서 start가 포함된 것과 End가 포함된 것 그리고
분류가 안되는 이름을 넣어서 시도를 했습니다.    
C++로 진행하는 소켓통신 부분은 ([참조](https://kevinthegrey.tistory.com/26) 여기에서 많은 도움을 받았습니다. 
