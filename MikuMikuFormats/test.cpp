#include "Pmx.h"
#include <iostream>
#include <fstream>
#include <iomanip>

#ifndef __unix__
#define utfcout std::wcout
#else
#define utfcout std::cout
#endif

#define PRINT(x, y) utfcout << std::setw(24) << x << ":" \
                            << std::setw(24) << std::right << y << std::endl;

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cout << "Usage: ./demo [PMX file path]" << std::endl;
        return 0;
    }
    std::filebuf fb;
    if(!fb.open(argv[1], std::ios::in | std::ios::binary)) {
        std::cout << "Can't open the " << argv[1] << ", please check" << std::endl;
        exit(-1);
    }

    std::istream is(&fb);
    pmx::PmxModel x;
    x.Read(&is);
    
    PRINT("PMX version", x.version)
    PRINT("Model Name", x.model_name)
    PRINT("Model English Name", x.model_english_name)
    PRINT("Comment", x.model_comment)
    PRINT("English Comment", x.model_english_comment)
    PRINT("#vertex", x.vertex_count)
    PRINT("#index", x.index_count)
    PRINT("#texture", x.texture_count)
    PRINT("#material", x.material_count)
    PRINT("#bone", x.bone_count)
    PRINT("#morph", x.morph_count)
    PRINT("#frame", x.frame_count)
    PRINT("#rigid body", x.rigid_body_count)
    PRINT("#joint", x.joint_count)
    PRINT("#soft body", x.soft_body_count)
}
