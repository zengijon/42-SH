if ! true
then
    echo false
else
    echo true
fi

if ! false
then
    echo true
else
    echo false
fi

t=false

if ! $t
then
    t=true
else
    t=2
fi

echo $t

