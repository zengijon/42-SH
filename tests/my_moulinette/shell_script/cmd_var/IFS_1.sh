string="foo
bar
foobar"
IFS="\n"

echo "$string"

for i in $string
do
    echo $i test
done

IFS=":"

for i in $string
do
    echo $i test
done

string2="foo:bar:foobar"

for i in $string2
do
    echo $i test
done

IFS="\n"
