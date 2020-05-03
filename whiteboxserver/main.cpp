#include "Allopencv.h"
#include "Socket.h"
#include <iostream>
#include <string>
using namespace std;
int main(void)
{
	Socket sock;
	cout << "소켓통신 시작" << endl;
	sock.Run_socket();
	return 0;
}