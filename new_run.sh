rm -R ./build
echo "delete dict build"
mkdir build
dir=./build
cd ${dir}
cmake ..
make 
runDir=./src
cd ${runDir}
./renderer