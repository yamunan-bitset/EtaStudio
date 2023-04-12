# EtaStudio
Graphics Framework

# Build:
```
$ # build the static file 
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ cd ..
$ # build the example
$ gcc main.c -L build -l eta -l SDL2 -l SDL2_gfx 
```