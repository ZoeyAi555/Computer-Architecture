#include <iostream>
#include <omp.h>

int main() {
    const int N = 100;
    float a[N], b[N], c[N];

    // Initialize arrays a and b
    for(int i = 0; i < N; i++) {
        a[i] = static_cast<float>(i);
        b[i] = static_cast<float>(2*i);
    }

    // Specify the device (0 is the default device)
    int device = 0;

    // Set the device
    omp_set_default_device(device);

    // Check if the device is available
    if(omp_get_num_devices() > 0) {
        // Offload the computation to the device
        #pragma omp target map(to: a[:N], b[:N]) map(from: c[:N])
        {
            for(int i = 0; i < N; i++) {
                c[i] = a[i] + b[i];
            }
        }
    } else {
        std::cout << "No devices available for offloading.\n";
    }

    // Print the result
    for(int i = 0; i < N; i++) {
        std::cout << c[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}
