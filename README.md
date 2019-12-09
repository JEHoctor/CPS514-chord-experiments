## Build & Run

### Clone and checkout this branch
```
git clone https://github.com/JEHoctor/CPS514-chord-experiments.git
cd CPS514-chord-experiments/
git checkout chord-dev-mininet-vm-compat
```

### Installing dependencies in a Ubuntu 14.04 mininet VM

Getting an acceptable g++ is easy:
```
sudo apt-get install g++
```

Unfortunately, the version of cmake provided in the ubuntu 14.04 repositories is too old, so we must
install cmake from Kitware's website.
```
wget https://github.com/Kitware/CMake/releases/download/v3.16.0/cmake-3.16.0-Linux-x86_64.sh
chmod +x cmake-3.16.0-Linux-x86_64.sh
yes | ./cmake-3.16.0-Linux-x86_64.sh
cd cmake-3.16.0-Linux-x86_64/
echo 'PATH=$PATH:'$(pwd)'/bin' >> ~/.bashrc
PATH=$PATH:$(pwd)/bin
```

### Installing Protobuf and gRPC

```
mkdir third-party
cd third-party
git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
cd grpc
git submodule update --init

cd third_party/protobuf
./autogen.sh
./configure --prefix=/opt/protobuf
make -j `nproc`
sudo make install

cd ../..
make -j `nproc` PROTOC=/opt/protobuf/bin/protoc
sudo make prefix=/opt/grpc install
```
Reference: https://github.com/IvanSafonov/grpc-cmake-example/blob/master/README.md

### Build the executable

```
mkdir build
cd build
cmake ..
make
```
*On Mac OS, we need to run an additional command before running the executable*
`$ export DYLD_LIBRARY_PATH=.:/opt/grpc/lib:/opt/protobuf/lib`

The executable is produced in `build/src`.

### Get the gRPC python modules
This is not needed and won't work in the mininet vm. DO NOT DO THIS:
```
sudo apt-get install python-pip
pip install grpcio grpcio-tools
```

### Get matplotlib and NetworkX
Here's something else that doesn't work. Don't do this either:
```
sudo apt-get install python-setuptools
sudo apt-get install python-dev
sudo easy_install pip
pip install --user --upgrade six
pip install --user matplotlib networkx
```

### OK, how about Anaconda? Will that work?
I'm skeptical but let's try. Fist, [install Anaconda](https://www.anaconda.com/distribution/) with python 2.7. The VM may run out of disk space. If so, add another virtual disk, mount that disk, and move things there. The anaconda installation itself will have to be on this mount as well. You will need `mkfs.ext4`, `lsblk`, `mkdir`, and `mount`. This repository (CPS514-chord-experiments) is quite large for some reason, so delete it and clone it on the mount. When you're done with that:
```
pip install matplotlib networkx
```
But you shouldn't do that either, because you can't install the mininet package in anaconda.

### Run the experiment
This assumes that you're still in `CPS514-chord-experiments/build`.
```
cd ../sim/
sudo python mininet-sim.py
```
