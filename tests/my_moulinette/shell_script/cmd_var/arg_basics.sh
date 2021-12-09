echo $@
echo $*
echo $1
echo $2
echo $3
echo $4
echo $5 $6 $7 $8 $9 $0
echo $#

echo coucou
for i in $@
do
    echo $i
done

echo bonjour

for i in $*
do
    echo $i
done

