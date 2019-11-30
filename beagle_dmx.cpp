#include "main.h"

#include <iostream>
#include <fstream>
#include <termios.h>
#include <unistd.h>

using namespace std;

int main()
{
	ifstream ttyStreamIn;
	ofstream ttyStreamOut;
	string input;
	string output;
	int random = rand();
	cout << "Hello Beagle" << random << endl;
	if(setupTTY(4,57600) < 0)
	{
		cout << "ERROR";
		return -1;
	}

	ttyStreamIn.open("/dev/ttyO4", ios::in);
	ttyStreamOut.open("/dev/ttyO4", ios::out);

	while(1)
	{
		cin >> input;
		if(input == "exit")
		{
			break;
		}
		else
		{
			ttyStreamOut << input;
			flush(ttyStreamOut);
		}
		usleep(100000);
		ttyStreamIn >> output;
		cout << output << endl;

	}
	ttyStreamIn.close();
	ttyStreamOut.close();
	return 0;
}

int setupTTY(int number, int baudrate)
{
	FILE * fpTTY;
	int fdTTY = 0;
	struct termios termios_p;
	//char* path = "/dev/ttyO4";
	//sprintf(path,"/dev/ttyO%d", number);

	fpTTY = fopen("/dev/ttyO4","r+");
	fdTTY = fileno(fpTTY);
	if(!isatty(fdTTY))
	{
		cout << "File is not pointing to a TTY device" << endl;
		return -1;
	}
	if(tcgetattr(fdTTY, &termios_p) < 0)
	{
		cout << "Failed to read current configuration." << endl;
		return -1;
	}

	// Input flags - turn off input processing
	termios_p.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);

	// Output flags - turn off output processing
	termios_p.c_oflag = 0;

	// No line processing
	termios_p.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

	// Turn off character processing
	termios_p.c_cflag &= ~(CSIZE | PARENB);
	termios_p.c_cflag |= CS8;

	// One input byte is enough to return from read()
	// Inter-character timer off
	termios_p.c_cc[VMIN] = 0;
	termios_p.c_cc[VTIME] = 0;

	if(cfsetispeed(&termios_p, B57600) < 0 || cfsetospeed(&termios_p, B57600) < 0)
	{
		cout << "Failed to set Baudrate" << endl;
		return -1;
	}

	if(tcsetattr(fdTTY, TCSAFLUSH, &termios_p))
	{
		cout << "Failed to set new configuration." << endl;
		return -1;
	}

	cout << "Successfully configured TTY device." << endl;
	return 0;

	fclose(fpTTY);
}
