#include "Allopencv.h"
#include "Socket.h"
#include <iostream>
#include <string>
using namespace std;
int main(void)
{
	Socket sock;
	cout << "������� ����" << endl;
	sock.Run_socket();
	return 0;
}