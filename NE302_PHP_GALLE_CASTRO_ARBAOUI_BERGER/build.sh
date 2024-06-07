wget https://zlib.net/zlib-1.3.1.tar.gz

tar -xvf zlib-1.3.1.tar.gz

cd zlib-1.3.1

./configure --prefix=../1.3.1

make

make test

make install

cd ..
rm -R zlib-1.3.1/
rm zlib-1.3.1.tar.gz
