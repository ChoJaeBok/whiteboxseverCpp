#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>

//��� ����
#pragma warning(disable:4996)
//���� ����� ���� ���̺귯�� ��ŷ
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
		//connect(����, ���ϱ������ ����ü�� �ּ�, �� ����ü�� ũ��);
		//connect �Լ��� ������ ���Ͽ� ������ �������ش�. ������ �����ϱ����� connect�Լ��� ���
		cout << "���� ����:" << endl;
		//���� ���� ����

		cin >> select_i;
		char cMsg[] = "EndROI";
		char cBuffer[PACKET_SIZE] = {};
		if (select_i == 0) {
			strcpy_s(cMsg, "Down");
			send(hSocket, cMsg, strlen(cMsg), 0);
			//���� ����
			//���� ���� ����
			char myFile[256] = "startimage.jpg";
			FILE *fp = fopen(myFile, "rb");

			//���� �̸� ����
			char filename[256];
			ZeroMemory(filename, 256);
			sprintf(filename, myFile);

			//�������� ���� �̸��� ����
			send(hSocket, filename, 256, 0);

			//����ũ�� ���
			fseek(fp, 0, SEEK_END);
			int totalbytes = ftell(fp);

			//���� ũ�� �������� ������
			send(hSocket, (char *)&totalbytes, sizeof(totalbytes), 0);
			//���� ������ ���ۿ� ����� ����
			char buf[PACKET_SIZE];
			int numread;
			int numtotal = 0;

			//���� �����͸� ���� ������ �̵�
			rewind(fp);
			//�ݺ������� ���� ������ ������
			while (1) {
				//������ ������ ���ۿ� ����
				numread = fread(buf, 1, PACKET_SIZE, fp);

				//���� �����Ͱ� �����̶� ���� ���
				if (numread > 0) {
					send(hSocket, buf, numread, 0);
					numtotal += numread;
				}

				//������ ��� ������ ���
				else if (numread == 0 && numtotal == totalbytes) {
					cout << "��" << numtotal << "����Ʈ ���������� �Ϸ�" << endl;
					break;
				}

			}
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else if (select_i == 10) {
			strcpy_s(cMsg, "Down");
			send(hSocket, cMsg, strlen(cMsg), 0);
			//���� ����
			//���� ���� ����
			char myFile[256] = "123.jpg";
			FILE *fp = fopen(myFile, "rb");

			//���� �̸� ����
			char filename[256];
			ZeroMemory(filename, 256);
			sprintf(filename, myFile);

			//�������� ���� �̸��� ����
			send(hSocket, filename, 256, 0);

			//����ũ�� ���
			fseek(fp, 0, SEEK_END);
			int totalbytes = ftell(fp);

			//���� ũ�� �������� ������
			send(hSocket, (char *)&totalbytes, sizeof(totalbytes), 0);
			//���� ������ ���ۿ� ����� ����
			char buf[PACKET_SIZE];
			int numread;
			int numtotal = 0;

			//���� �����͸� ���� ������ �̵�
			rewind(fp);
			//�ݺ������� ���� ������ ������
			while (1) {
				//������ ������ ���ۿ� ����
				numread = fread(buf, 1, PACKET_SIZE, fp);

				//���� �����Ͱ� �����̶� ���� ���
				if (numread > 0) {
					send(hSocket, buf, numread, 0);
					numtotal += numread;
				}

				//������ ��� ������ ���
				else if (numread == 0 && numtotal == totalbytes) {
					cout << "��" << numtotal << "����Ʈ ���������� �Ϸ�" << endl;
					break;
				}
			}
			recv(hSocket, cBuffer, PACKET_SIZE, 0);
			cout << "Recv Msg:" << cBuffer << endl;
		}
		else if (select_i == 1) {
			strcpy_s(cMsg, "Down");
			send(hSocket, cMsg, strlen(cMsg), 0);
			//���� ����
			//���� ���� ����
			char myFile[256] = "Endimage.jpg";
			FILE *fp = fopen(myFile, "rb");

			//���� �̸� ����
			char filename[256];
			ZeroMemory(filename, 256);
			sprintf(filename, myFile);

			//�������� ���� �̸��� ����
			send(hSocket, filename, 256, 0);

			//����ũ�� ���
			fseek(fp, 0, SEEK_END);
			int totalbytes = ftell(fp);

			//���� ũ�� �������� ������
			send(hSocket, (char *)&totalbytes, sizeof(totalbytes), 0);
			//���� ������ ���ۿ� ����� ����
			char buf[PACKET_SIZE];
			int numread;
			int numtotal = 0;

			//���� �����͸� ���� ������ �̵�
			rewind(fp);
			//�ݺ������� ���� ������ ������
			while (1) {
				//������ ������ ���ۿ� ����
				numread = fread(buf, 1, PACKET_SIZE, fp);

				//���� �����Ͱ� �����̶� ���� ���
				if (numread > 0) {
					send(hSocket, buf, numread, 0);
					numtotal += numread;
				}

				//������ ��� ������ ���
				else if (numread == 0 && numtotal == totalbytes) {
					cout << "��" << numtotal << "����Ʈ ���������� �Ϸ�" << endl;
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



	//Ŭ���̾�Ʈ�� �ڵ�, ���� ������� ����ü�� ������ ������ ip�� �����ش�. Ŭ���̾�Ʈ������ bind�Լ� ��� connect�Լ��� ���
	WSACleanup();
	return 0;

}