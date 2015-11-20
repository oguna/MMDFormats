# MikuMikuFormats

## 概要

このライブラリはMikuMikuDanceで使われている以下のフォーマットの読み込みを行います。

- PMD
- PMX(Ver2.0; 2.1には少し対応)
- VMD

## 必要

以下を使っているため、C++11に対応したコンパイラが必要です。

- scoped enumeration
- uint32_t,uint16_t,uint8_t
- unique_ptr

## 文字コードの扱いについて
EncodingHelper.h内でWin32APIを使って変換しています。
そのためWindows依存です。

## サンプル
```cpp
const char *filename = "sample.pmx";
pmx::PmxModel model;
std::ifstream stream = std::ifstream(filename, std::ios_base::binary);
model.Read(&stream);
stream.close();
```

## ライセンス

ご自由にお使い下さい。
このプログラム使用によるいかなる損害も、ogunaは責任を負わないものとします。

Distributed under CC0 1.0 Universal.

