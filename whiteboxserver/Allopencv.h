#include <string>
#ifndef ALLOPENCV_H
#define ALLOPENCV_H
#define MAX 15
class Allopencv {
public:
	void Run();
	void Imread(std::string img_str, std::string img_str2);//이미지 불러오는 부분
	void RoiImg(std::string img_str); //start imageROI
	void End_RoiImg(std::string img_str);//end imageROI
	void search(); //확인 및 endroi할 좌표 저장.(location을 이용)
	void location(int x1, int y1, int x2, int y2, int count); // 좌표 저장하는 부분
	void set_i_num(int i);
	int print_area_startx(int count);
	int print_area_starty(int count);
	int print_area_endx(int count);
	int print_area_endy(int count);
	int print_i_num();
	void Histogram();
	//ROI하기 위한 좌표들을 저장해놓는 공간으로 startx, starty은 처음에 찍힌 좌표들이며 endx,endy는 마지막에 찍힌 좌표들이다
	//배열은 0은 첫번째 ROI, 1은 두번째 ROI... 이런식의 저장하기 위해 사용.
private:
	int startx[MAX];
	int starty[MAX];
	int endx[MAX];
	int endy[MAX];
	int i_num;// 좌석매핑한 횟수
};
#endif // !ALLOPENCV_H

