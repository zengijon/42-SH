ls /home/ | cat -e

echo chocolat | cat -e | echo test | cat -e

echo chocolat | cat -e | echo test | cat -e | cat -e | cat -e | cat -e

{ echo te | cat | cat ; } | cat -e

if true | false
then
    echo true
else
    echo false
fi

if false | true
then
    echo true
else
    echo false
fi



