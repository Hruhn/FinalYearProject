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
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <ftm_msgs/ftm.h>
#include <string>

bool parseSensorData(const char *buffer, ftm_msgs::ftm& msg) {
    std::string data(buffer);

    if (data.find("id") == std::string::npos || data.find("rtt_est") == std::string::npos ||
        data.find("rtt_raw") == std::string::npos || data.find("dist_est") == std::string::npos ||
        data.find("average rssi") == std::string::npos || data.find("num_frames") == std::string::npos) {

        printf("Invalid data format: %s\n", data.c_str());
        return false;
    }

    std::istringstream iss(data);
    std::string token;
    while (std::getline(iss, token, ',')) {
        size_t colonPos = token.find(':');
        if (colonPos != std::string::npos) {
            std::string key = token.substr(0, colonPos);
            std::string value = token.substr(colonPos + 1);

            if (key == "id") {
                msg.anchorId = value;
            } else if (key == "rtt_est") {
                // Handle rtt_est if needed
            } else if (key == "rtt_raw") {
                msg.rtt_raw = std::stoi(value);
            } else if (key == "dist_est") {
                msg.dist_est = std::stoi(value);
            } else if (key == "average rssi") {
                msg.rssi = std::stoi(value);
            } else if (key == "num_frames") {
                msg.num_frames = std::stoi(value);
            }
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    // ...（略去前面的代码）

    bool receivedCompleteData = false;

    while (ros::ok()) {
        printf("ftm_node running\n");

        FD_ZERO(&readSet);
        FD_SET(serialPort, &readSet);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ready = select(serialPort + 1, &readSet, NULL, NULL, &timeout);

        if (ready > 0) {
            if (FD_ISSET(serialPort, &readSet)) {
                bytesRead = read(serialPort, buffer + totalBytesRead, sizeof(buffer) - 1 - totalBytesRead);
                if (bytesRead > 0) {
                    totalBytesRead += bytesRead;
                    buffer[totalBytesRead] = '\0';

                    if (strstr(buffer, "num_frames") != NULL) {
                        receivedCompleteData = true;
                    }
                }
            }
        } else if (ready == 0) {
            if (receivedCompleteData) {
                ftm_msgs::ftm msg;
                if (parseSensorData(buffer, msg)) {
                    pub.publish(msg);
                }

                receivedCompleteData = false;
                totalBytesRead = 0;
                memset(buffer, 0, sizeof(buffer));
            }
        } else {
            perror("select");
            break;
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    close(serialPort);
    return 0;
}
*/
