#ifndef SOCLET_H
#define SOCLET_H

using namespace std;
class Socket {
public:
	void Run_socket();//소켓통신을 위한.
	void insertimg(string img1);// 이미지 변수 채우기 위한.
	string print_firimg();
	string print_secimg();
	string print_noimg();
private:
	string firstimg;
	string secondimg;
	string noimg;
};

#endif