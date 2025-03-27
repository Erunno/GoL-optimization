#include <stdio.h>
#include "cuda_hello.cuh"

// Simple CUDA kernel
__global__ void hello_kernel() {
    // Empty kernel just to verify CUDA compilation
    printf("Hello from CUDA kernel!\n");
}

// Function to be called from main
void cuda_hello() {
    // Launch a simple kernel
    hello_kernel<<<1, 1>>>();
    
    // Synchronize to ensure kernel completion
    cudaDeviceSynchronize();
    
    // Print CUDA information
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);
    
    printf("Hello from CUDA! Found %d CUDA device(s)\n", deviceCount);
    
    // Print device information for each device
    for (int i = 0; i < deviceCount; i++) {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, i);
        printf("  Device %d: %s\n", i, deviceProp.name);
        printf("    Compute capability: %d.%d\n", deviceProp.major, deviceProp.minor);
    }
}
