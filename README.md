windows run command

```
g++ floppy.cpp -O2 -I raylib/include -L raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -mconsole 
```

Mac run command

install raylib if not installed

```
brew install raylib
```

compile command
```
g++ floppy.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib
```
or
```
g++ floppy.cpp -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -o floppy
```