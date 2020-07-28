#!/bin/bash
cd

#------------<COMMONS>----------------------

if test -d /home/utnso/so-commons-library; then
    cd /home/utnso/so-commons-library/
    sudo make uninstall
    cd ..
    sudo rm -r ./so-commons-library
fi

cd

git clone https://github.com/sisoputnfrba/so-commons-library.git
cd so-commons-library/
sudo make install

cd

#------------</COMMONS>----------------------
/*
# ------------<BASE>---------------------------------
# directorio para prueba completa
if test -d /home/utnso/delibird-prueba-completa/; then
	sudo rm -r /home/utnso/delibird-prueba-complet
fi

cd
mkdir -p /home/utnso/delibird-prueba-complet
cd /home/utnso/delibird-prueba-complet/
mkdir Metadata
mkdir Blocks
mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS\n" >> Metadata.bin

cd

# Copio carpeta tp

if test -d /home/utnso/workspace/tp-2020-1c-wRAPPERS/; then
	sudo rm -r /home/utnso/workspace/tp-2020-1c-wRAPPERS
fi

cd /home/utnso/tp-safe-zone/	NO SE QUE ES ESTO

cp -r tp-2020-1c-wRAPPERS/ /home/utnso/workspace/tp-2020-1c-wRAPPERS
*/

# Creo GameBoy

cd /home/utnso/workspace/tp-2020-1c-wRAPPERS/GameBoy/Debug/

make all

# Config GameBoy

cd ../

if test -e GameBoy.config; then
	rm GameBoy.config
fi

echo -e "IP_BROKER=127.0.0.1\nPUERTO_BROKER=4444\nIP_TEAM=127.0.0.2\nPUERTO_TEAM=5555\nIP_GAMECARD=127.0.0.4\nPUERTO_GAMECARD=7777\nPROCESS_ID=9876\nPROCESO_ID=\nLOG_FILE=/home/utnso/Documentos/Delibird/GameBoy/tp-2020-1c-wRAPPERS/GameBoy/GameBoy.log" >> GameBoy.config


# Creo GameCard

cd /home/utnso/workspace/tp-2020-1c-wRAPPERS/GameCard/Debug/

make all

cd ../

# Config GameCard

if test -e GameCard.config; then
	rm GameCard.config
fi

echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS\n" >> GameCard.config

# Creo Broker

cd /home/utnso/workspace/tp-2020-1c-wRAPPERS/Broker/Debug/

make all

cd ../

# Config Broker

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\nTAMANO_MINIMO_PARTICION=4\nALGORITMO_MEMORIA=PARTICIONES\nALGORITMO_REEMPLAZO=FIFO\nALGORITMO_PARTICION_LIBRE=FF\nIP_BROKER=\nPUERTO_BROKER=\nFRECUENCIA_COMPACTACION=1\nLOG_FILE=\n" >> Broker.config

# Creo Teams

cd /home/utnso/workspace/tp-2020-1c-wRAPPERS/Team/Debug/

make all

cd ../../

cp Team/ Team2

# Config Team2

cd /home/utnso/workspace/tp-2020-1c-wRAPPERS/Team/

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,2|2]\nPOKEMON_ENTRENADORES=[Pikachu]\nOBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=FIFO\nQUANTUM=0\nESTIMACION_INICIAL=5\nIP_BROKER=\nPUERTO_BROKER=\nLOG_FILE=\n" >> Team.config

cd ../Team2/

if test -e Team2.config; then
	rm Team2.config
fi

echo -e "POSICIONES_ENTRENADORES=[2|3,6|5,9|9,9|2,2|9]\nPOKEMON_ENTRENADORES=[]\nOBJETIVOS_ENTRENADORES=[Vaporeon,Jolteon,Flareon,Umbreon,Espeon]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=RR\nQUANTUM=1\nESTIMACION_INICIAL=5\nIP_BROKER=\nPUERTO_BROKER=\nLOG_FILE=\n" >> Team2.config

# -------------</BASE>---------------------------------

cd




