#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

int main() {

    std::cout << "Studio starting up\n";

    std::ifstream file("resources/shader.vert", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open shader.vert\n";
        return EXIT_FAILURE;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    if (size <= 0) {
        std::cerr << "Shader file is empty or error occurred\n";
        return EXIT_FAILURE;
    }
    file.seekg(0, std::ios::beg);

    std::vector<char> data(size);
    file.read(data.data(), size);
    file.close();

    for (const auto &c : data) {
        std::cout << c;
    }

    return EXIT_SUCCESS;
}
