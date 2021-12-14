echo Hello > test
cat < test
echo Error 1>&2
echo Error >&2
cat test
echo Sorry > test
cat test
