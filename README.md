# Pong
Another really simple game I recreated in C++, as a test
for my even newer development environment featuring neovim, clangd, coq_nvim,
and a bit of custom lua code I wrote to manage my projects.

# What I learned
Pong isn't that simple.

This project taught me more than I anticipated. Like using SDL2, and working with cmake.
I also got to take my new setup with neovim, its plugins and my custom scripts for a test ride.
I learned the quirks of working with CMake. I used CMake to build this app for different
platforms. I learned how to integrate CMake into my development routine, by making it generate
compile_commands.json and letting my clangd (my language server) use it.

Apart from that, the game logic also took longer than I expected.
The most interesting part is that the way the ball bounces is actually entirely
dependant on which part of the paddle hits it, and not at all dependant on the
velocity and angle of the ball prior to impact.

Figuring out an approximation of the algorithm used in the original pong
was easily the part that took me the longest amount of time.

I also had to implement my own geometry functions. Although [my geometry subroutines](src/geometry.cpp) 
in this app are by no means a complete library, I implemented collision functions between
circles, rectangles, lines, and line segments of arbitrary sizes from scratch.

# How to build?

If you're on linux or similar run:

```
mkdir -p build/
cmake . -B build/
cd build
make
./pong
```

The above shouldn't be very complicated if you already know how to use CMake.

If you're on windows, just build using cmake.

Note that you have to have SDL2 and SDL2_ttf installed before building, 
regardless of platform.
