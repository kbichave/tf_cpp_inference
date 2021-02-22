mkdir build
cp -r data / build/
cd build
cmake ..
make
./3DSmoothNet --fileCloud <path_to_point_cloud_file>
