#include <iostream>
#include <WinSock2.h>
#include "Socket.h"
#include "Allopencv.h"

#pragma comment(lib, "ws2_32")

#define PORT 8888
#define PACKET_SIZE 1024
using namespace std;
Allopencv all;
string img_str("startimage.jpg");// ó�� �̹���
string img_str2("Endimage.jpg");// ������ �̹��� 
void Socket::set_inum(int count) {
	i_num_socket = count;
}
int Socket::get_inum() {
	return i_num_socket;
}
void Socket::Run_socket() {
	int d = 0;
	WSADATA wsaData;//windows�� �����ʱ�ȭ������ �����ϱ� ���� ����ü. �̹� ����Ǿ��ִ� ����ü 
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = AF_INET;
	tListenAddr.sin_port = htons(PORT);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	//bind(����, ���� ������� ����ü�� �ּ�, �� ����ü�� ũ��);
	listen(hListen, SOMAXCONN);
	//������ �����ϴ� ���·� ������ ���¸� �����Ѵ�. ��, ������ ���Ӵ�� ���·� ������ش�.
	//SOMAXCONN�� �Ѳ����� ��û ������ �ִ� ���ӽ��� ���� �ǹ�.
//	cout << "print_i_num:"<< i_count << endl;
	while (true) {
		d++;
		SOCKADDR_IN tClntAddr = {};
		int iClntSize = sizeof(tClntAddr);
		SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);
		//Ŭ���̾�Ʈ �� ���� ���� �� ������ ���� ����ü ���� �� �� �Ҵ�, Ŭ���̾�Ʈ�� ���� ��û�ϸ� �������ִ� ����
		//accept(����, ���� ������� �ּ�ü�� �ּ�, �� ����ü�� ũ�⸦ ����ִ� ������ �ּ�);
		//�� �Լ��� �̿��Ͽ� ���� ��û�� ����. �� �Լ��� ����ȭ�� ������� ���۵ȴ�.
		//�� ��û�� ������ ������ �� �Լ��� �Ⱥ�������.

		char cBuffer[PACKET_SIZE] = {};

		//Ŭ���̾�Ʈ�� ������ �����ϱ� ���� ������ �����ص� ��Ŷ ũ�⸸ŭ ���۸� ����
		recv(hClient, cBuffer, PACKET_SIZE, 0);
		//recv(����, ���� ������ ���� �迭�ּ�, �� �迭�� ũ��, flag);
		cout << "Recv Msg: " << cBuffer << endl;
		char cMsg[] = "StartROI";
		if (strcmp(cBuffer, "ROI") == 0) {
			strcpy_s(cMsg, "StartROI");
			send(hClient, cMsg, strlen(cMsg), 0);
			cout << "ROI����" << endl;
			all.RoiImg(img_str);
			cout << "Ȯ��" << endl;
			all.search();
			continue;
		}
		else if (strcmp(cBuffer, "EndROI") == 0) {
			cout << "endROI����" << endl;
			all.End_RoiImg(img_str2);
			continue;
		}
		else if (strcmp(cBuffer, "His") == 0) {
			cout << "histogram ����" << endl;
			all.Histogram();
		}
		else if (strcmp(cBuffer, "load") == 0) {
			cout << "�̹��� �ҷ����� ����" << endl;
			all.Imread(img_str, img_str2);
		}
		else {
			cout << "�߸��� ����" << endl;
			continue;

		}

		send(hClient, cMsg, strlen(cMsg), 0);
		closesocket(hClient);

	}

	closesocket(hListen);
	//Ŭ���̾�Ʈ �����κ��� ������ �޾ƿ��� ���, Ŭ���̾�Ʈ�� ���� ����

	WSACleanup();//������ Ȱ���ϴ°��� startup�� cleanup ���̿� �ۼ�. �����ڿ� �Ҹ��� ���� ����
}
