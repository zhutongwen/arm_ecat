
//1.uboot
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- myd_c437x_idk_defconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j12

//2.linux
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- omap2plus_defconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j12

mount -t nfs -o nolock,rw 192.168.1.152:/home/xw/am437/nfs /mnt
mount -t nfs -o nolock,rw 192.168.1.24:/home/xw/am437/nfs /mnt


//
ip addr add 127.0.0.1/24 dev lo
ip link set lo up
