#include <iostream>
#include <WinSock2.h>
#include "Socket.h"
#include "Allopencv.h"

#pragma comment(lib, "ws2_32")

#define PORT 8888
#define PACKET_SIZE 1024
using namespace std;
Allopencv all;
string img_str("startimage.jpg");// 처음 이미지
string img_str2("Endimage.jpg");// 마지막 이미지 
void Socket::set_inum(int count) {
	i_num_socket = count;
}
int Socket::get_inum() {
	return i_num_socket;
}
void Socket::Run_socket() {
	int d = 0;
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
//	cout << "print_i_num:"<< i_count << endl;
	while (true) {
		d++;
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
			continue;
		}
		else if (strcmp(cBuffer, "EndROI") == 0) {
			cout << "endROI실행" << endl;
			all.End_RoiImg(img_str2);
			continue;
		}
		else if (strcmp(cBuffer, "His") == 0) {
			cout << "histogram 실행" << endl;
			all.Histogram();
		}
		else if (strcmp(cBuffer, "load") == 0) {
			cout << "이미지 불러오기 실행" << endl;
			all.Imread(img_str, img_str2);
		}
		else {
			cout << "잘못된 접근" << endl;
			continue;

		}

		send(hClient, cMsg, strlen(cMsg), 0);
		closesocket(hClient);

	}

	closesocket(hListen);
	//클라이언트 측으로부터 정보를 받아오고 출력, 클라이언트에 정보 전송

	WSACleanup();//소켓을 활용하는것은 startup과 cleanup 사이에 작성. 생성자와 소멸자 같은 개념
}
