# Conway's game of life in C with SDL1.2

This is a simple implementation of Conway's game of life in C using SDL1.2.

Make sur you  have SDL1.2 installed by running:

```
sudo apt-get install libsdl1.2-dev
```

Then you can compile and run the program:

``````
gcc -o prog main.c game_of_life.c -lSDL
./prog
``````