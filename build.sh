if [ ! -d cmake-build ]; then
    mkdir cmake-build;
fi
cd cmake-build || exit
cmake ..
make
cp 42sh ../42sh