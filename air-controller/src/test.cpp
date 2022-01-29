#include <iostream>

#include "util.hpp"

int main() {
    auto tree =
        OctTree<int>(Region{Vector3{0.f, 0.f, 0.f}, Vector3{10.f, 10.f, 10.f}});
    std::cout << "???" << std::endl;
    return 0;
}
