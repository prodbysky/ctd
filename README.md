# CTD
Tower defense game in C++ and raylib

## Path (level path) format
First byte is the amount of path segments.
Then the segments are pairs of 1 byte values (x, y) until EOF

## Enemy format
First byte is the base speed of the enemy,
Second byte is the type of the enemy

## TODO:

## Build
```bash
mkdir build
cd build
cmake ..
make -j
./bin/ctd
```
