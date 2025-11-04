#include "cbox/math/common/functions.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <vector>

#include <cbox/math/math.hpp>

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


    std::println("rad: {} " , cc::rad(180.0f));
    std::println("degrees: {} " , cc::degrees(cc::pi<float>));



    std::println("Lerp: {}",cc::lerp(1.0f, 2.0f, 0.5f));

    if constexpr (cc::sign(-50)) {
        std::println("negative");
    }else {
        std::println("positive");
    }


    std::println("Min : {} from 7 and 42" , cc::min(7, 42));
    std::println("Max : {} from 7 and 42" , cc::max(7, 42));

    std::println("Clamo: {} beetween 7 and 42" , cc::clamp( 10000 ,7, 42));

    return EXIT_SUCCESS;
}
