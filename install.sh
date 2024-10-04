apt-get install  -y git cmake libssl-dev libplist-dev libavahi-compat-libdnssd-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev gstreamer1.0-plugins-base gstreamer1.0-libav gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer.10-tools

cd $HOME

git clone https://github.com/FDH2/UxPlay
# TODO download unser kack

cd UxPlay
mkdir build
cd build
cmake ..
make
make install

cd $HOME
git clone https://github.com/Koefte/AirPlayLeo -b master
cd AirPlayLeo
cmake
make
