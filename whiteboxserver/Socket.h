#ifndef SOCLET_H
#define SOCLET_H

using namespace std;
class Socket {
public:
	void Run_socket();//��������� ����.
	void insertimg(string img1);// �̹��� ���� ä��� ����.
	string print_firimg();
	string print_secimg();
	string print_noimg();
private:
	string firstimg;
	string secondimg;
	string noimg;
};

#endif