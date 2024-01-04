#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>

#define SERIAL_DEVICE "/dev/ttyS0" // 串口设备，根据你的设置修改
#define SERVER_IP "192.168.3.211"  // 服务器 IP 地址
#define SERVER_PORT 8080           // 服务器端口号，根据你的设置修改

int main() {

    int serialPort = open("/dev/ttyUSB0", O_RDWR);
    if (serialPort < 0)
    {
        perror("Error opening serial port");
        return 1;
    }

    struct termios tio;
    tcgetattr(serialPort, &tio);
    cfsetospeed(&tio, B115200); // 设置波特率为115200
    tcsetattr(serialPort, TCSANOW, &tio);

    // char sendData[] = "Hello, ESP32!";
    // write(serialPort, sendData, sizeof(sendData));

    char receiveData[100];
    int bytesRead;

    int sock;
    struct sockaddr_in server_addr;


    // 创建TCP套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Unable to create socket: %s\n", strerror(errno));
        return 1;
    }

    // 设置服务器地址信息
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address or address not supported\n");
        return 1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Connection failed: %s\n", strerror(errno));
        return 1;
    }

    while (1) {
        bytesRead = read(serialPort, receiveData, sizeof(receiveData));
        receiveData[bytesRead] = '\0';
        if (bytesRead > 0) {
            if (send(sock, receiveData, bytesRead, 0) != bytesRead) {
                fprintf(stderr, "Failed to send data to server\n");
                break;
            }
        }
    }

    // 关闭连接和串口
    close(sock);
    close(serialPort);

    return 0;
}


