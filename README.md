# MikuMikuFormats

[日本語README](README_Ja.md)

## General

This library can parse the following formats, which are used in MikuMikuDance.

- PMD
- PMX(ver2.0; little support for ver2.1)
- VMD

## Requirement

This library requires C++11 supported compiler, bacause this library uses the following features,

- scoped enumeration
- uint32_t,uint16_t,uint8_t
- unique_ptr

## About Character Encoding
This library uses Win32API in 'EncodingHelper.h' ,
so this library need in Windows.

## Sample

```cpp
const char *filename = "sample.pmx";
pmx::PmxModel model;
std::ifstream stream = std::ifstream(filename, std::ios_base::binary);
model.Read(&stream);
stream.close();
```

## License

You can use this library freely.
I dont't care about your usages.

Distributed under CC0 1.0 Universal.

