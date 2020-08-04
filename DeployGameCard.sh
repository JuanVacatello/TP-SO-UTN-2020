# tp-2020-1c-wRAPPERS

cd

#INICIO DE COMMONS

if test -d /home/utnso/so-commons-library; then
    cd /home/utnso/so-commons-library/
    sudo make unistall
    cd ..
    sudo rm -r ./so-commons-library
fi

cd

git clone https://github.com/sisoputnfrba/so-commons-library.git
cd so-commons-library/
sudo make install

cd

#FIN DE COMMONS

#COPIO CARPETA TP

if test -d /home/utnso/Documentos/tp-2020-1c-wRAPPERS/; then
	sudo rm -r /home/utnso/Documentos/tp-2020-1c-wRAPPERS/
fi

mkdir Documentos

cd

cp -r tp-2020-1c-wRAPPERS/ /home/utnso/Documentos/tp-2020-1c-wRAPPERS

#FIN DE COPIAR CARPETA

cd

#BASE

if test -d /home/utnso/Documentos/FILESYSTEM; then
	sudo rm -r /home/utnso/Documentos/FILESYSTEM
fi

cd

mkdir /home/utnso/Documentos/FILESYSTEM

cd 

#FIN BASE

#INICIO DE GAMEBOY

cd Documentos/tp-2020-1c-wRAPPERS/GameBoy/

if test -e GameBoy.config; then
	rm GameBoy.config
fi

echo -e "IP_BROKER=127.0.0.1\nIP_TEAM=127.0.0.2\nIP_GAMECARD=127.0.0.4\nPUERTO_BROKER=4444\nPUERTO_TEAM=5555\nPUERTO_GAMECARD=7777\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/GameBoy/GameBoy.log" >> GameBoy.config

cd Debug
make clean
make all

#FIN DE GAMEBOY

cd

#INICIO DE GAMECARD
cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/GameCard/

if test -e GameCard.config; then
	rm GameCard.config
fi

echo -e "BLOCKS=1024\n
BLOCK_SIZE=64\n
MAGIC_NUMBER=TALL_GRASS\n
PUNTO_MONTAJE_TALLGRASS=/home/utnso/Documentos/FILESYSTEM\n
TIEMPO_DE_REINTENTO_CONEXION=10\n
TIEMPO_DE_REINTENTO_OPERACION=5\n
TIEMPO_RETARDO_OPERACION=1\n
PUERTO_BROKER=4444\n
IP_BROKER=127.0.0.1\n
PROCESS_ID=5678\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/GameCard/GameCard.log\n
PUERTO_GAMECARD=7777\n
IP_GAMECARD=127.0.0.4\n" >> GameCard.config

cd Debug
make clean
make all

#FIN DE GAMECARD