cp ../../builddir/42sh .
rm -rf *~
if [ $# -ne 1 ]
then
    echo "type ?"
else
    python3 testsuite2.py --binary 42sh --type $1
fi

