mkdir build
cp pointcloud_data/muffler_2.ply build/
cp frozen_graph/graph.pb build/
cp pointcloud_data/tacked_prat_clean_20_20_20_1_1.ply build/
cp pointcloud_data/tacked_prat_noise.ply build/
cp -r data / build/
cd build
cmake ..
make
./3DSmoothNet --fileCloud muffler_2.ply 
