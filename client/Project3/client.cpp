#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>

//경고 제어
#pragma warning(disable:4996)
//원속 사용을 위한 라이브러리 링킹
#pragma comment(lib, "ws2_32")
#define PORT 8888
#define PACKET_SIZE 4096
#define SERVER_IP "172.30.1.37"
using namespace std;
static int i = 0;
int main()
{
	int select_i = 0;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return -1;
	}



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
		//보낼 파일 설정

		cin >> select_i;
		char cMsg[] = "EndROI";
		char cBuffer[PACKET_SIZE] = {};
		if (select_i == 0) {
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
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else if (select_i == 10) {
			strcpy_s(cMsg, "Down");
			send(hSocket, cMsg, strlen(cMsg), 0);
			//파일 열기
			//보낼 파일 설정
			char myFile[256] = "123.jpg";
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
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else if (select_i == 1) {
			strcpy_s(cMsg, "Down");
			send(hSocket, cMsg, strlen(cMsg), 0);
			//파일 열기
			//보낼 파일 설정
			char myFile[256] = "Endimage.jpg";
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
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else if (select_i == 2) {
			strcpy_s(cMsg, "ROI");
			send(hSocket, cMsg, strlen(cMsg), 0);
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;

		}
		else if (select_i == 3) {
			cout << cMsg << endl;
			strcpy_s(cMsg, "EndROI");
			send(hSocket, cMsg, strlen(cMsg), 0);
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else if (select_i == 4) {
			strcpy_s(cMsg, "His");
			send(hSocket, cMsg, strlen(cMsg), 0);
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else if (select_i == 5) {
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