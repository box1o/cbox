#include "cbox/math/detail/arithmetic.hpp"
#include "cbox/math/mat/base.hpp"
#include <cbox/math/math.hpp>
#include <print>

int main() {


    // std::ifstream file("resources/shader.vert", std::ios::binary);
    // if (!file) {
    //     std::cerr << "Failed to open shader.vert\n";
    //     return EXIT_FAILURE;
    // }
    //
    // file.seekg(0, std::ios::end);
    // std::streamsize size = file.tellg();
    // if (size <= 0) {
    //     std::cerr << "Shader file is empty or error occurred\n";
    //     return EXIT_FAILURE;
    // }
    // file.seekg(0, std::ios::beg);
    //
    // std::vector<char> data(size);
    // file.read(data.data(), size);
    // file.close();
    //
    // for (const auto &c : data) {
    //     std::cout << c;
    // }
    //
    //
    // std::println("rad: {} " , cc::rad(180.0f));
    // std::println("degrees: {} " , cc::degrees(cc::pi<float>));
    //
    //
    //
    // std::println("Lerp: {}",cc::lerp(1.0f, 2.0f, 0.5f));
    //
    // if constexpr (cc::sign(-50)) {
    //     std::println("negative");
    // }else {
    //     std::println("positive");
    // }
    //
    //
    // std::println("Min : {} from 7 and 42" , cc::min(7, 42));
    // std::println("Max : {} from 7 and 42" , cc::max(7, 42));
    //
    // std::println("Clamo: {} beetween 7 and 42" , cc::clamp( 10000 ,7, 42));
    //
    //
    // std::println("\t vec : {}", cc::vec<4, float>(1.3));
    // std::println("\t vec : {}", cc::vec<4, float>(1.0, 2.0, 3.0, 4.0));
    //
    // cc::vec<4, float> vector = cc::vec<4, float>(199.9);
    // std::println("\t vec : {}", cc::vec<4, int>(vector));
    //
    //
    // std::println("op+: {}", cc::vec2i(4 ,5) + cc::vec2i(2 , 5));
    // std::println("op-: {}", cc::vec2i(4) - cc::vec2i(2));
    //
    //
    // std::println("op*: {}", cc::vec2i(4 ,5) * cc::vec2i(2 , 5));
    // std::println("op/: {}", cc::vec2i(4) / cc::vec2i(2));
    //
    //
    // std::println("scalar : {}", cc::vec2i(4) * 3);
    // std::println("scalar : {}", cc::vec2i(12) / 3);
    //
    //
    // std::println(" x : {} , y {} , z {}"  , cc::vec3(1,4 ,6).x ,cc::vec3(1,4 ,6).y , cc::vec3(1,4 ,6).z) ;



    auto test = cc::mat<3, 3, int>(
            cc::layout::rowm,
            1, 2, 3, 
            1, 2, 3, 
            1, 2, 3
    );

    std::println("{}",test);


    auto matrixI = cc::mat3f::identity();
    std::println("{}" , matrixI);


    return EXIT_SUCCESS;
}
