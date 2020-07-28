#!/bin/bash
cd

if test -d /home/utnso/so-commons-library; then
    cd /home/utnso/so-commons-library/
    sudo make uninstall
    cd ..
    sudo rm -r ./so-commons-library
fi

cd

git clone https://github.com/sisoputnfrba/so-commons-library.git
cd so-commons-library/
make install

cd

# -------------
# Directorio para prueba final
if test -d /home/utnso/lfs-stress/; then
	sudo rm -r /home/utnso/lfs-stress
fi

cd
mkdir -p /home/utnso/lfs-stress
cd /home/utnso/lfs-stress/
mkdir Metadata
mkdir Blocks
mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=128\nBLOCKS=8192\nMAGIC_NUMBER=LISSANDRA" >> Metadata.bin

# -------------

cd

# -------------
# directorio para prueba de Memoria
if test -d /home/utnso/lfs-prueba-memoria/; then
	sudo rm -r /home/utnso/lfs-prueba-memoria
fi

cd
mkdir -p /home/utnso/lfs-prueba-memoria
cd /home/utnso/lfs-prueba-memoria/
mkdir Metadata
mkdir Blocks
mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=4096\nMAGIC_NUMBER=LISSANDRA" >> Metadata.bin

# -------------

cd

# -------------
# directorio para prueba de lfs
if test -d /home/utnso/lfs-compactacion/; then
	sudo rm -r /home/utnso/lfs-compactacion
fi

cd
mkdir -p /home/utnso/lfs-compactacion
cd /home/utnso/lfs-compactacion/
mkdir Metadata
mkdir Blocks
mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=4096\nMAGIC_NUMBER=LISSANDRA" >> Metadata.bin

# -------------

cd

# -------------
# directorio para prueba de Kernel
if test -d /home/utnso/lfs-prueba-kernel/; then
	sudo rm -r /home/utnso/lfs-prueba-kernel
fi

cd
mkdir -p /home/utnso/lfs-prueba-kernel
cd /home/utnso/lfs-prueba-kernel/
mkdir Metadata
mkdir Blocks
mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=128\nBLOCKS=4096\nMAGIC_NUMBER=LISSANDRA" >> Metadata.bin

# -------------
cd

# -------------
# directorio para prueba de Kernel
if test -d /home/utnso/lfs-base/; then
	sudo rm -r /home/utnso/lfs-base
fi

cd
mkdir -p /home/utnso/lfs-base
cd /home/utnso/lfs-base/
mkdir Metadata
mkdir Blocks
mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=4096\nMAGIC_NUMBER=LISSANDRA" >> Metadata.bin

# -------------

cd

# -------------
#Deploy de los 3 sistemas

if test ! -d /home/utnso/workspace; then
    mkdir /home/utnso/workspace
fi

cd

mv /home/utnso/tp-2019-1c-Los-Sinequi/  /home/utnso/workspace

cd /home/utnso/workspace/tp-2019-1c-Los-Sinequi/sharedLib/Debug
    make clean
    make all

cd

cd /home/utnso/workspace/tp-2019-1c-Los-Sinequi/FileSystem/Debug
    make clean
    make all

cd

cd /home/utnso/workspace/tp-2019-1c-Los-Sinequi/Kernel/Debug
    make clean
    make all

cd

cd /home/utnso/workspace/tp-2019-1c-Los-Sinequi/Memory/Debug
    make clean
    make all

cd /home/utnso/workspace/tp-2019-1c-Los-Sinequi
