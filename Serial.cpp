#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>

int main() {
    int serialPort = open("/dev/ttyUSB0", O_RDWR);
    if (serialPort < 0) {
        perror("Error opening serial port");
        return 1;
    }

    struct termios tio;
    tcgetattr(serialPort, &tio);
    cfsetospeed(&tio, B9600);  // 设置波特率为9600
    tcsetattr(serialPort, TCSANOW, &tio);

    char buffer[256];
    memset(buffer, 0, sizeof(buffer);

    fd_set readSet;
    struct timeval timeout;
    int bytesRead = 0;
    int totalBytesRead = 0;

    while (1) {
        FD_ZERO(&readSet);
        FD_SET(serialPort, &readSet);

        timeout.tv_sec = 1;  // 等待1秒钟
        timeout.tv_usec = 0;

        int ready = select(serialPort + 1, &readSet, NULL, NULL, &timeout);

        if (ready > 0) {
            if (FD_ISSET(serialPort, &readSet)) {
                bytesRead = read(serialPort, buffer, sizeof(buffer) - 1);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    printf("Received: %s", buffer);
                    totalBytesRead += bytesRead;
                }
            }
        } else if (ready == 0) {
            printf("No data received within 1 second.\n");
        } else {
            perror("select");
            break;
        }
    }

    close(serialPort);
    return 0;
}
