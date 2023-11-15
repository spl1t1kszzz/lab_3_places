#include <iostream>
#include "Finder.hpp"

int main() {
    // setlocale(LC_ALL, "Russian");
    std::string request;
    std::cout << "Type request:";
    std::getline(std::cin, request, '\n');
    Places::Finder::find_places(request);
    return 0;
}