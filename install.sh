mkdir build
echo "---------mkdir build done---------"
cd build
echo "---------cd build done-----------"
echo "--------cmake processing-------"
cmake ..
echo "----------make processing--------"
make
cd ..
echo "----------moving runtest.sh-------"
mv runtest.sh ./build/test
echo "__________Well done ,man !__________"
echo "Now please cd ./build/test and use runtest.sh to tstart"
echo "Have a good time !"