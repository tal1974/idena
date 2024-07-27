# idena
## Build Ubuntu

```Bash
sudo apt update
sudo apt upgrade
sudo apt install git
sudo apt install cmake
sudo apt install g++
sudo apt-get install libssl-dev
git clone https://github.com/tal1974/idena
cd idena/replenish/
mkdir out
cd out
cmake ..
cmake --build . --parallel
./idena_replenish http://127.0.0.1:9119 123apikey 10 2
```

## Build Windows
Building for Windows tested under Visual Studio. If you have trouble with the assembly, then check if you have a “Git for Windows” component.

## Release 1.0
[App](https://github.com/tal1974/idena/releases/tag/1.0) tested for Windows 10 64 bit. If start without parameters then help screen appear.