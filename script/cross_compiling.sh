# debug compiling for device
if [ ! -d build ];then
    mkdir build
fi
pushd build
cmake .. -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ -DCMAKE_BUILD_TYPE=debug
make
popd