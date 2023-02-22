# debug compiling for device
if [ ! -d build ];then
    mkdir build
fi
pushd build
cmake .. -DCMAKE_BUILD_TYPE=debug
make
popd