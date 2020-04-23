#include <string>
#ifndef ALLOPENCV_H
#define ALLOPENCV_H
#define MAX 15
class Allopencv {
public:
	void Run();
	void Imread(std::string img_str, std::string img_str2);//�̹��� �ҷ����� �κ�
	void RoiImg(std::string img_str); //start imageROI
	void End_RoiImg(std::string img_str);//end imageROI
	void search(); //Ȯ�� �� endroi�� ��ǥ ����.(location�� �̿�)
	void location(int x1, int y1, int x2, int y2, int count); // ��ǥ �����ϴ� �κ�
	void set_i_num(int i);
	int print_area_startx(int count);
	int print_area_starty(int count);
	int print_area_endx(int count);
	int print_area_endy(int count);
	int print_i_num();
	void Histogram();
	//ROI�ϱ� ���� ��ǥ���� �����س��� �������� startx, starty�� ó���� ���� ��ǥ���̸� endx,endy�� �������� ���� ��ǥ���̴�
	//�迭�� 0�� ù��° ROI, 1�� �ι�° ROI... �̷����� �����ϱ� ���� ���.
private:
	int startx[MAX];
	int starty[MAX];
	int endx[MAX];
	int endy[MAX];
	int i_num;// �¼������� Ƚ��
};
#endif // !ALLOPENCV_H

