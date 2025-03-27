#include <iostream>
#include <cxxopts.hpp>
#include "cuda_hello.cuh"

int main(int argc, char* argv[]) {
    // Set up command line argument parsing using cxxopts
    cxxopts::Options options("GoL_Benchmark", "Benchmark application for CUDA Game of Life implementations");
    
    options.add_options()
        ("h,help", "Show help")
        ("t,test", "Test name to run", cxxopts::value<std::string>()->default_value("default"))
        ("n,number", "A number to print (for testing cxxopts)", cxxopts::value<int>());
    
    // Parse arguments
    auto result = options.parse(argc, argv);
    
    // Show help if requested
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }
    
    // Get the test parameter
    std::string test_name = result["test"].as<std::string>();
    std::cout << "Running test: " << test_name << std::endl;
    
    // Check if number is provided and print it
    if (result.count("number")) {
        int number = result["number"].as<int>();
        std::cout << "Number provided: " << number << std::endl;
    }
    
    // C++ hello world
    std::cout << "Hello from C++!" << std::endl;
    
    // Call CUDA hello world
    cuda_hello();
    
    return 0;
}
