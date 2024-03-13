a game of minesweeper made entirely in c with ncurses

requirements:
- a linux distribution
- gcc
- the C standard library (pretty sure it comes included with the gcc package)
- libncurses

dependencies (for debian and derivatives)
```shell
## note for debian users: change the prefix of 'ncurses6' for your specific cpu architecture
apt install build-essential lib64ncurses6
```

dependencies (for arch and derivatives)
```shell
pacman -S gcc ncurses
```

to build the binary
```shell
git clone https://github.com/flippyshoe1/cminesweeper.git
cd cminesweeper
make
```

this time im confident i made it memory safe :D\
(feel free to make an issue if it isnt and please dont attempt to run this at a professional enviroment)
