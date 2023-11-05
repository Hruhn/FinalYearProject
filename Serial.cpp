#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int main()
{
    int serialPort = open("/dev/ttyUSB0", O_RDWR);
    if (serialPort < 0)
    {
        perror("Error opening serial port");
        return 1;
    }

    struct termios tio;
    tcgetattr(serialPort, &tio);
    cfsetospeed(&tio, B115200); // 设置波特率为9600
    tcsetattr(serialPort, TCSANOW, &tio);

    // char sendData[] = "Hello, ESP32!";
    // write(serialPort, sendData, sizeof(sendData));

    char receiveData[100];
    int bytesRead;

    while (1)
    {
        bytesRead = read(serialPort, receiveData, sizeof(receiveData));
        receiveData[bytesRead] = '\0';
        printf("Received: %s\n", receiveData);
    }

    close(serialPort);
    return 0;
}

