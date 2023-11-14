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

/*
// 修改解析串口数据的函数，添加条件判断
void parseSensorData(const char *data, struct SensorData *sensorData) {
    // 添加判断条件，确保数据包含有效的传感器信息
    if (strstr(data, "id:") == NULL || strstr(data, "rtt_est:") == NULL || strstr(data, "rtt_raw:") == NULL ||
        strstr(data, "dist_est:") == NULL || strstr(data, "average rssi:") == NULL || strstr(data, "num_frames:") == NULL) {
        // 数据格式不符合预期，不执行解析
        printf("Invalid data format: %s\n", data);
        return;
    }

    char tempData[strlen(data) + 1];
    strcpy(tempData, data);

    char *token = strtok(tempData, ",");
    while (token != NULL) {
        char key[20];
        int value;

        // 添加条件判断，确保解析的键值对格式正确
        if (sscanf(token, "%19[^:]:%d", key, &value) == 2) {
            if (strcmp(key, "id") == 0) {
                strncpy(sensorData->id, token + 3, sizeof(sensorData->id) - 1);
                sensorData->id[sizeof(sensorData->id) - 1] = '\0';
            } else if (strcmp(key, "rtt_est") == 0) {
                sensorData->rtt_est = value;
            } else if (strcmp(key, "rtt_raw") == 0) {
                sensorData->rtt_raw = value;
            } else if (strcmp(key, "dist_est") == 0) {
                sensorData->dist_est = value;
            } else if (strcmp(key, "average rssi") == 0) {
                sensorData->average_rssi = value;
            } else if (strcmp(key, "num_frames") == 0) {
                sensorData->num_frames = value;
            }
        }

        token = strtok(NULL, ",");
    }
}
*/
