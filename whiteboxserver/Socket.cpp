#include <iostream>
#include <WinSock2.h>
#include "Socket.h"
#include "Allopencv.h"


#define PORT 8888
#define PACKET_SIZE 4096
//��� ����
#pragma warning(disable:4996)
//���� ����� ���� ���̺귯�� ��ŷ
#pragma comment(lib, "ws2_32")
using namespace std;
Allopencv all;
Socket socc;
int datReceive(SOCKET s, char* buf, int len, int flags) {
	//������ ���� �Լ�
	int received;
	char *ptr = buf;
	int left = len;
	//�����ִ� �����Ͱ� �ִ� ��� �ݺ������� ����
	while (left > 0) {
		received = recv(s, ptr, left, flags);
		//���̻� �����͸� �޾ƿ��� ���ϴ� ���
		if (received == 0) {
			break;
		}
		left -= received;
		ptr += received;
	}
	//�� ���� �� �ִ� ������ ���̸� ��ȯ (�����͸� �ѹ��̶� �޾����� 0�� �ƴ� ���� ��ȯ�ϰ� ��.
	return (len - left);
}
void Socket::insertimg(string img1) {
	if (img1.find("start") != string::npos) {
		//start��� ���ڰ� ���� ���
		firstimg = img1;
	}
	else if (img1.find("End") != string::npos) {
		//End��� ���ڰ� ���� ��� 
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

	//������ ũ�⸦ ���� ����
	int retval;

	WSADATA wsaData;//windows�� �����ʱ�ȭ������ �����ϱ� ���� ����ü. �̹� ����Ǿ��ִ� ����ü 
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tListenAddr = {};
	ZeroMemory(&tListenAddr, sizeof(tListenAddr));
	tListenAddr.sin_family = AF_INET;
	tListenAddr.sin_port = htons(PORT);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	//bind(����, ���� ������� ����ü�� �ּ�, �� ����ü�� ũ��);
	listen(hListen, SOMAXCONN);
	//������ �����ϴ� ���·� ������ ���¸� �����Ѵ�. ��, ������ ���Ӵ�� ���·� ������ش�.
	//SOMAXCONN�� �Ѳ����� ��û ������ �ִ� ���ӽ��� ���� �ǹ�.
	//������ ��ſ� ����� ���� 
	SOCKET hClient;
	SOCKADDR_IN tClntAddr;
	int iClntSize;
	char buf[PACKET_SIZE];
	while (true) {
		tClntAddr = {};
		iClntSize = sizeof(tClntAddr);
		hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);
		//Ŭ���̾�Ʈ �� ���� ���� �� ������ ���� ����ü ���� �� �� �Ҵ�, Ŭ���̾�Ʈ�� ���� ��û�ϸ� �������ִ� ����
		//accept(����, ���� ������� �ּ�ü�� �ּ�, �� ����ü�� ũ�⸦ ����ִ� ������ �ּ�);
		//�� �Լ��� �̿��Ͽ� ���� ��û�� ����. �� �Լ��� ����ȭ�� ������� ���۵ȴ�.
		//�� ��û�� ������ ������ �� �Լ��� �Ⱥ�������.

		char cBuffer[PACKET_SIZE] = {};
		//Ŭ���̾�Ʈ�� ������ �����ϱ� ���� ������ �����ص� ��Ŷ ũ�⸸ŭ ���۸� ����
		recv(hClient, cBuffer, PACKET_SIZE, 0);
		//recv(����, ���� ������ ���� �迭�ּ�, �� �迭�� ũ��, flag);
		cout << "Recv Msg: " << cBuffer << endl;
		char cMsg[] = "StartROI  ";

		if (strcmp(cBuffer, "Down") == 0) {
			cout << "Ŭ���̾�Ʈ ���� : IP=" << inet_ntoa(tClntAddr.sin_addr) << ", PORT=" << ntohs(tClntAddr.sin_port) << endl;
			//���� �̸� �ޱ�
			char filename[256];
			ZeroMemory(filename, 256);
			datReceive(hClient, filename, 256, 0);
			cout << "���� ���� �̸� : " << filename << endl;
			socc.insertimg(filename);
			if (socc.print_noimg() != "") {
				//�̹��� ������ �� �߸��� �̹����� ������� �ʵ��� �ϱ����� ������ �κ��Դϴ�. 
				cout << "�߸��� �̹��� �����Դϴ�. ó���̹����� start�� �ٿ��ֽð� ���� �̹����� End�� �ٿ��ּ���!" << endl;
				socc.insertimg("");
				continue;
			}
			//���� ũ�� �ޱ�
			int totalbytes;
			datReceive(hClient, (char *)&totalbytes, sizeof(totalbytes), 0);
			//���� ���� ũ�� ���
			cout << "���� ���� ũ�� :" << totalbytes << endl;
			//���� ����
			FILE *fp = fopen(filename, "wb");
			//���� ������ �ޱ�
			int numtotal = 0;
			while (1) {
				retval = datReceive(hClient, buf, PACKET_SIZE, 0);
				//�� ���� �����Ͱ� ������
				if (retval == 0) {
					break;
				}
				else {
					fwrite(buf, 1, retval, fp);
					//���������� ũ�⸸ŭ ������ ������
					numtotal += retval;
				}
			}
			fclose(fp);

			//���� ���
			if (numtotal == totalbytes) {
				cout << "���� ����" << endl;
			}
			else { cout << "���� ����" << endl; }
		}

		else if (strcmp(cBuffer, "ROI") == 0) {
			strcpy_s(cMsg, "StartROI");
			send(hClient, cMsg, strlen(cMsg), 0);
			cout << "ROI����" << endl;
			all.RoiImg(socc.print_firimg());
			cout << "Ȯ��" << endl;
			all.search();
			send(hClient, cMsg, strlen(cMsg), 0);
			
		}
		else if (strcmp(cBuffer, "EndROI") == 0) {
			strcpy_s(cMsg, "EndROI");
			cout << "endROI����" << endl;
			all.End_RoiImg(socc.print_secimg());
			send(hClient, cMsg, strlen(cMsg), 0);
			
		}
		else if (strcmp(cBuffer, "His") == 0) {
			strcpy_s(cMsg, "His");
			cout << "histogram ����" << endl;
			all.Histogram();
			send(hClient, cMsg, strlen(cMsg), 0);
			
		}
		else if (strcmp(cBuffer, "load") == 0) {
			strcpy_s(cMsg, "load");
			cout << "�̹��� �ҷ����� ����" << endl;
			all.Imread(socc.print_firimg(), socc.print_secimg());
			send(hClient, cMsg, strlen(cMsg), 0);
			
		}
		else if (strcmp(cBuffer, "break") == 0) {
			strcpy_s(cMsg, "break");
			cout << "���� ����" << endl;
			send(hClient, cMsg, strlen(cMsg), 0);
			break;
		}
		else {
			cout << "�߸��� ����" << endl;
			

		}

		closesocket(hClient);
	}

	closesocket(hListen);
	//Ŭ���̾�Ʈ �����κ��� ������ �޾ƿ��� ���, Ŭ���̾�Ʈ�� ���� ����

	WSACleanup();//������ Ȱ���ϴ°��� startup�� cleanup ���̿� �ۼ�. �����ڿ� �Ҹ��� ���� ����
}
