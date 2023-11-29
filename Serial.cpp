#include <stdio.h>
#include <string.h>

#define MAX_DATA_LENGTH 1000 // 假设数据最大长度为1000

int main() {
    char received_data[MAX_DATA_LENGTH];
    int data_count[6] = {0}; // 记录接收到的id1到id6数据个数

    while (1) {
        // 假设这里是你接收串口数据的代码，将接收到的数据存储到 received_data 中
        // 这里使用 fgets 来模拟接收数据，实际情况可能需要用到串口相关的库函数来接收数据
        fgets(received_data, MAX_DATA_LENGTH, stdin); // 从标准输入中读取模拟的数据

        // 判断接收到的数据是否包含 id1 到 id6 的一系列数据
        for (int i = 1; i <= 6; ++i) {
            char id_str[10];
            sprintf(id_str, "id%d:", i); // 构建id字符串

            if (strstr(received_data, id_str) != NULL) {
                data_count[i - 1]++; // 更新对应id的数据个数
            }
        }

        // 检查是否接收到 id1 到 id6 的一系列数据
        int all_data_received = 1;
        for (int i = 0; i < 6; ++i) {
            if (data_count[i] == 0) {
                all_data_received = 0;
                break;
            }
        }

        if (all_data_received) {
            // 所有数据都已接收到，进行数据处理
            printf("Received all data for id1 to id6.\n");
            // 在这里进行你的数据处理操作

            // 处理完数据后，重置数据计数器，以便下一轮接收
            memset(data_count, 0, sizeof(data_count));
        }
    }

    return 0;
}
