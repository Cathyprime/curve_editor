# Bèzier curve editor

A demo allowing you to play with a bèzier curve, made using c++ and raylib.

Uses a custom build system, [flavortown](https://github.com/Cathyprime/flavortown) (not yet fully working on the niche operating system made for games called windows).
To run the project you have to bootstrap the build system by compiling it

```bash
clang++ -o nob nob.cc
```

From now use the `./nob` command to build the project, type `./nob help` to see all available commands.
The 'nob' executable will rebuild itself if it detects any changes to it's source files, and will rebuild the c++ files as needed
