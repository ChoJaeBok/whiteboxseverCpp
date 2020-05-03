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
