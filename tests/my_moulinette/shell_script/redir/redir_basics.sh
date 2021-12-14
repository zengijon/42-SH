echo "Hello" > test
cat < test
echo "Error" 1>&2
echo "Error" >&2
echo "Error" >&2 2>&1
echo "Sorry" > test
cat test
echo "At the End" >> test
cat test
echo "test" >> test2
cat test2
echo coucou <<EOF
EOF

