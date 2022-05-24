./build.sh
cp cmake-build-debug/42sh tests/my_moulinette/42sh
cd tests/my_moulinette/
rm -rf *~
if [ $# -ne 1 ]
then
    echo "type ?"
else
    python3 testsuite2.py --binary 42sh --type $1
fi
cd ../..

