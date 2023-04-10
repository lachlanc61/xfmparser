#include "example.h"

void process_data(data_t data, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << data[i] << std::endl;
    }
}
