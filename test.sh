if [ ! -d builddir ]; then
    meson setup builddir;
fi


ninja -C builddir
cp builddir/42sh tests/my_moulinette/
cd tests/my_moulinette/
rm -rf *~
if [ $# -ne 1 ]
then
    echo "type ?"
else
    python3 testsuite2.py --binary 42sh --type $1
fi
cd ../..

