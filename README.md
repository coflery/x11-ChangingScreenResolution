# x11-ChangingScreenResolution
Example which chang screen resolution on x11, it's just a copy from opengl demo.


## on ubuntu
  apt install libxrandr-dev

## compile
  gcc -o Xrandr Xrandr.c -lX11 -lXrandr -lstdc++
