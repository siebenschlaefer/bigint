# bigint


## How to install googletest

wget -c https://github.com/google/googletest/archive/release-1.8.0.tar.gz
tar xf release-1.8.0.tar.gz
cd googletest-release-1.8.0/googlemock/
autoreconf -i
./configure
make fused-gmock-internal
cp -r fused-gmock-internal to "$bigint"/googletest

