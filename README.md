# MikuMikuFormats
 
[日本語README](README_Ja.md)

## Important

The code in this project was introduced in [assimp](https://github.com/assimp/assimp).
Assimp keeps itself maintenance.
Please use the code in assimp because of its safety.

## General

This library can parse the following formats, which are used in MikuMikuDance.

- PMD
- PMX(ver2.0; little support for ver2.1)
- VMD

## Requirement
Windows MSVC:

This library requires C++11 supported compiler, because this library uses the following features,

- scoped enumeration
- uint32_t,uint16_t,uint8_t
- unique_ptr

Un\*x:
- C++14 for the feature make_unique
- cmake
- ICU(International Components for Unicode)

## About Character Encoding
Windows:

This library uses Win32API in 'EncodingHelper.h'.

Un\*x:

We use ICU library to convert from UTF-16LE to UTF-8.

## Sample

```cpp
const char *filename = "sample.pmx";
pmx::PmxModel model;
std::ifstream stream = std::ifstream(filename, std::ios_base::binary);
model.Read(&stream);
stream.close();
```

A sample binary `demo` can be built from `test.cpp`.
```bash
./demo [PMX file path]
```

## License

You can use this library freely.
I don't care about your usages.

Distributed under CC0 1.0 Universal.

`FindICU.cmake` is made by [POINSOT Julien](https://github.com/julp/FindICU.cmake) and is under BSD.
