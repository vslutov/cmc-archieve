#!/usr/bin/env sh
cd build

make
rc=$?;
if [[ $rc != 0 ]] ; then
        echo Build error
        exit $rc
fi

cd ..

for test_num in {0001..0010} ; do
    ./build/gear pic/${test_num}.bmp pic/res${test_num}.bmp labelling/res${test_num}.txt
    rc=$?
    if [[ $rc != 0 ]] ; then
        echo Runtime: $test_num
        exit $rc
    fi

    ./compare.py labelling/res${test_num}.txt labelling/${test_num}.txt
    rc=$?
    if [[ $rc != 0 ]] ; then
        echo Failed test: $test_num
        exit $rc
    fi
done
