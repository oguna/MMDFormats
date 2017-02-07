#include "Pmx.h"
#include <iostream>
#include <fstream>

#ifndef __unix__
#define utfcout std::wcout
#else
#define utfcout std::cout
#endif

int main() {
    std::filebuf fb;
    if(!fb.open("test.pmx", std::ios::in | std::ios::binary)) {
        std::cout << "Can't open the test.pmx, please check" << std::endl;
        exit(-1);
    }

    std::istream is(&fb);
    pmx::PmxModel x;
    x.Read(&is);
    
    utfcout << x.model_name << std::endl;
    utfcout << x.model_english_name << std::endl;
}
