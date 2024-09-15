# CTD
Tower defense game in C++ and raylib

## Path (level path) format
First byte is the amount of path segments.
Then the segments are pairs of 1 byte values (x, y) until EOF

## TODO:
 - Path editor

## Build
```bash
mkdir build
cd build
cmake ..
make -j
./bin/ctd
```
