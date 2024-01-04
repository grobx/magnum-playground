This procedure was tested on Ubuntu 23.04.

Install the dependencies:

``` sh
apt install clang libstdc++-13-dev libxext-dev libgl-dev libglx-dev
apt install gdb lldb
```

Clone this repository:

``` sh
git clone --recurse-submodules -j8 https://github.com/grobx/magnum-playground.git
```

Compile:

``` sh
cd magnum-playground
mkdir build
cd build
cmake -G "Ninja" \
    -D CMAKE_CXX_COMPILER=/usr/bin/clang++ \
    -D CMAKE_C_COMPILER=/usr/bin/clang \
    ..
```
