## Build & Run

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
