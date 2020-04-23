#ifndef SOCLET_H
#define SOCLET_H

class roiimg;
class Socket {
public:
	void Run_socket();
	void set_inum(int count);
	int get_inum();

private:
	int i_num_socket;
};

#endif