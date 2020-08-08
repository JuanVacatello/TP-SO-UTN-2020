# tp-2020-1c-wRAPPERS
# PRUEBAS INDIVIDUALES
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

if test -d /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/; then
	sudo rm -r /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/
fi

mkdir Documentos

cd tp-2020-1c-wRAPPERS

cd 
cp -r tp-2020-1c-wRAPPERS/ /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base

#FIN DE COPIAR CARPETA

cd

#BASE

if test -d /home/utnso/Documentos/FILESYSTEM-BASE; then
	sudo rm -r /home/utnso/Documentos/FILESYSTEM-BASE
fi

cd

mkdir /home/utnso/Documentos/FILESYSTEM-BASE

#FIN BASE

cd 

#INICIO DE GAMEBOY

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/GameBoy/

if test -e GameBoy.config; then
	rm GameBoy.config
fi

echo -e "IP_BROKER=127.0.0.1\n
IP_TEAM=127.0.0.2\n
IP_GAMECARD=127.0.0.4\n
PUERTO_BROKER=4444\n
PUERTO_TEAM=5555\n
PUERTO_GAMECARD=7777\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/GameBoy/GameBoy.log\n
PROCESS_ID=2808" >> GameBoy.config

cd Debug
make clean
make all

#FIN DE GAMEBOY
cd

#INICIO DE GAMECARD
cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/GameCard/

if test -e GameCard.config; then
	rm GameCard.config
fi

echo -e "BLOCKS=1024\n
BLOCK_SIZE=64\n
MAGIC_NUMBER=TALL_GRASS\n
PUNTO_MONTAJE_TALLGRASS=/home/utnso/Documentos/FILESYSTEM-BASE\n
TIEMPO_DE_REINTENTO_CONEXION=10\n
TIEMPO_DE_REINTENTO_OPERACION=5\n
TIEMPO_RETARDO_OPERACION=1\n
PUERTO_BROKER=4444\n
IP_BROKER=127.0.0.1\n
PROCESS_ID=5678\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/GameCard/GameCard.log\n
PUERTO_GAMECARD=7777\n
IP_GAMECARD=127.0.0.4\n" >> GameCard.config

cd Debug
make clean
make all

#FIN DE GAMECARD

cd

#INICIO BROKER

#CREO LOS BROKERS

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/

cp -r Broker/ Broker-CONSOLIDACION-FIFO
cp -r Broker/ Broker-CONSOLIDACION-LRU
cp -r Broker/ Broker-COMPACTACION-PD-FIFO
cp -r Broker/ Broker-COMPACTACION-PD-LRU
cp -r Broker/ Broker-BUDDY-SYSTEM-FIFO
cp -r Broker/ Broker-BUDDY-SYSTEM-LRU

#CONFIGURO BROKERS
cd
#Configuramos Broker consolidacion Particiones dinamicas FIFO

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-CONSOLIDACION-FIFO

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=10\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-CONSOLIDACION-FIFO/Broker.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-CONSOLIDACION-FIFO/Broker.log\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n">> Broker.config

cd Debug
make clean
make all

cd

#Configuro Broker consolidacion Particiones dinamicas -LRU

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-CONSOLIDACION-LRU

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=LRU\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=10\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-CONSOLIDACION-LRU/Broker.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-CONSOLIDACION-LRU/Broker.log\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n">> Broker.config

cd Debug
make clean
make all

cd

#Configuro Broker compactacion Particiones Dinamicas

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-COMPACTACION-PD-FIFO/

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-COMPACTACION-PD-FIFO/Broker.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-COMPACTACION-PD-FIFO/Broker.log\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n">> Broker.config

cd Debug
make clean
make all

cd

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-COMPACTACION-PD-LRU

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=LRU\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-COMPACTACION-PD-LRU/Broker.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-COMPACTACION-PD-LRU/Broker.log\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n">> Broker.config

cd Debug
make clean
make all

cd

#Configuro Broker Buddy System

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-BUDDY-SYSTEM-FIFO

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=BS\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-BUDDY-SYSTEM-FIFO/Broker.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-BUDDY-SYSTEM-FIFO/Broker.log\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n">> Broker.config

cd Debug
make clean
make all

cd

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-BUDDY-SYSTEM-LRU

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=BS\n
ALGORITMO_REEMPLAZO=LRU\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-BUDDY-SYSTEM-LRU/Broker.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Broker-BUDDY-SYSTEM-LRU/Broker.log\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n">> Broker.config

cd Debug
make clean
make all

#FIN DE BROKER

cd

#CREO TEAMS

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/

cp -r Team/ Team-FIFO
cp -r Team/ Team-RR
cp -r Team/ Team-SJF-SD
cp -r Team/ Team-SJF-CD

#CONFIGURO LOS TEAMS
cd
#Configuro Team con FIFO

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Team-FIFO

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=1\n
ALGORITMO_PLANIFICACION=FIFO\n
QUANTUM=2\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Team-FIFO/Team_FIFO.log\n
PROCESS_ID=1234\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config



cd Debug
make clean
make all

cd
#Configuro Team con RR -> Quantum = 2

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Team-RR

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=1\n
ALGORITMO_PLANIFICACION=RR\n
QUANTUM=2\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Team-RR/Team_RR.log\n
PROCESS_ID=1235\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config


cd Debug

make clean
make all

cd
#Configuro Team con SJF - SD

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Team-SJF-SD

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=1\n
ALGORITMO_PLANIFICACION=SJF-SD\n
QUANTUM=2\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Team-SJF-SD/Team_SJF_SD.log\n
PROCESS_ID=1236\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug
make clean
make all

cd
#Configuro Team con SJF - CD

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Team-SJF-CD

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=1\n
ALGORITMO_PLANIFICACION=SJF-CD\n
QUANTUM=2\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-base/Team-SJF-CD/Team_SJF_CD.log\n
PROCESS_ID=1237\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug
make clean
make all

#FIN DE TEAM

cd