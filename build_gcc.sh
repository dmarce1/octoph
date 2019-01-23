set -x
archive=gcc-8_2_0-release
cd $WORK
mkdir build
cd build
if [ ! -f ${archive}.tar.gz ]; then
	wget https://github.com/gcc-mirror/gcc/archive/${archive}.tar.gz
fi
rm -r -f gcc-${archive}
tar -xvf ${archive}.tar.gz
cd gcc-${archive}

./contrib/download_prerequisites

./configure --prefix=$HOME/local/gcc/ --with-local-prefix=$HOME/local/gcc/ --enable-languages=c,c++,fortran,lto --enable-lto --enable-checking=release --disable-multilib --enable-shared=yes --enable-static=yes --enable-threads=posix --enable-gold=default --enable-plugins --enable-ld --with-plugin-ld=ld.gold --disable-bootstrap

make -j

make -j install

