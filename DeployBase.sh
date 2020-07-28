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

#BASE

if test -d /home/utnso/tallgrass-base/; then
	sudo rm -r /home/utnso/tallgrass-base/
fi

cd
mkdir -p /home/utnso/tallgrass-base
cd /home/utnso/tallgrass-base/

mkdir Metadata


cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS" >> Metadata.bin

cd

#FIN BASE

#COPIO CARPETA TP

if test -d /home/utnso/Documentos/tp-2020-1c-wRAPPERS/; then
	sudo rm -r /home/utnso/Documentos/tp-2020-1c-wRAPPERS/
fi

mkdir Documentos

cd tp-2020-1c-wRAPPERS
# git checkout develop
cd ..
cp -r tp-2020-1c-wRAPPERS/ /home/utnso/Documentos/tp-2020-1c-wRAPPERS

#FIN DE COPIAR CARPETA

cd

#INICIO DE GAMEBOY

cd Documentos/tp-2020-1c-wRAPPERS/GameBoy/

if test -e GameBoy.config; then
	rm GameBoy.config
fi

echo -e "IP_BROKER=\nIP_TEAM=\nIP_GAMECARD=\nPUERTO_BROKER=\nPUERTO_TEAM=\nPUERTO_GAMECARD=\nLOG_FILE=/home/utnso/Documentos/Delibird/GameBoy/tp-2020-1c-wRAPPERS/GameBoy/GameBoy.log" >> GameBoy.config

cd Debug
make clean
make all

#FIN DE GAMEBOY

#INICIO DE GAMECARD
cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/GameCard/

if test -e GameCard.config; then
	rm GameCard.config
fi


echo -e "TIEMPO_DE_REINTENTO_CONEXION=10\n
TIEMPO_DE_REINTENTO_OPERACION=5\n
TIEMPO_RETARDO_OPERACION=1\n
PUNTO_MONTAJE_TALLGRASS=/home/utnso/tallgrass-base\n
IP_BROKER=\n
PUERTO_BROKER=\n
PROCESS_ID=\n
LOG_FILE=\n
IP_GAMECARD=\n
PUERTO_GAMECARD=\n" >> GameCard.config

cd ../../Debug
make clean
make all

#FIN DE GAMECARD

#CREO TEAMS

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/

cp -r Team/ Team-RR
cp -r Team/ Team-SJF-SD
cp -r Team/ Team-SJF-CD

#CONFIGURO LOS TEAMS

#Configuro Team con FIFO

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=FIFO\n
QUANTUM=2\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=\n
PUERTO_BROKER=\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team_FIFO.log\n
PROCESS_ID=1234\n
IP_TEAM=\n
PUERTO_TEAM=\n" >> Team.config

cd Debug
make clean
make all

#Configuro Team con RR -> Quantum = 2

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team-RR

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=RR\n
QUANTUM=2\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=\n
PUERTO_BROKER=\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team_RR.log\n
PROCESS_ID=1235\n
IP_TEAM=\n
PUERTO_TEAM=\n" >> Team.config

cd ../../Debug
make clean
make all

#Configuro Team con SJF - SD

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team-SJF-SD

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=SJF-SD\n
QUANTUM=2\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=\n
PUERTO_BROKER=\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team_SJF_SD.log\n
PROCESS_ID=1236\n
IP_TEAM=\n
PUERTO_TEAM=\n" >> Team.config

cd ../../Debug
make clean
make all

#Configuro Team con SJF - CD

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team-SJF-CD

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=SJF-CD\n
QUANTUM=2\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=\n
PUERTO_BROKER=\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team_SJF_CD.log\n
PROCESS_ID=1237\n
IP_TEAM=\n
PUERTO_TEAM=\n" >> Team.config

cd ../../Debug
make clean
make all

#FIN DE TEAM

#INICIO BROKER

#CREO LOS BROKERS

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/

cp -r Broker/ Broker-CONSOLIDACION-LRU
cp -r Broker/ Broker-COMPACTACION-PD-FIFO
cp -r Broker/ Broker-COMPACTACION-PD-LRU
cp -r Broker/ Broker-BUDDY-SYSTEM-FIFO
cp -r Broker/ Broker-BUDDY-SYSTEM-LRU

#CONFIGURO BROKERS

#Configuramos Broker consolidacion Particiones dinamicas FIFO

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=10\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker/Broker_dump.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker/Broker_log.log\n
IP_BROKER=\n
PUERTO_BROKER=\n">> Broker.config

cd ../../Debug
make clean
make all

#Configuro Broker consolidacion Particiones dinamicas -LRU

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-CONSOLIDACION-LRU

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=LRU\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=10\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-CONSOLIDACION-LRU/Broker_dump.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-CONSOLIDACION-LRU/Broker_log.log\n
IP_BROKER=\n
PUERTO_BROKER=\n">> Broker.config

cd ../../Debug
make clean
make all

#Configuro Broker compactacion Particiones Dinamicas

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-COMPACTACION-PD-FIFO/

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-COMPACTACION-PD-FIFO/Broker_dump.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-COMPACTACION-PD-FIFO/Broker_log.log\n
IP_BROKER=\n
PUERTO_BROKER=\n">> Broker.config

cd ../../Debug
make clean
make all

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-COMPACTACION-PD-LRU

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=LRU\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-COMPACTACION-PD-LRU/Broker_dump.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-COMPACTACION-PD-LRU/Broker_log.log\n
IP_BROKER=\n
PUERTO_BROKER=\n">> Broker.config

cd ../../Debug
make clean
make all

#Configuro Broker Buddy System

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-BUDDY-SYSTEM-FIFO/

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=BS\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-BUDDY-SYSTEM-FIFO/Broker_dump.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-BUDDY-SYSTEM-FIFO/Broker_log.log\n
IP_BROKER=\n
PUERTO_BROKER=\n">> Broker.config

cd ../../Debug
make clean
make all

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-BUDDY-SYSTEM-LRU

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=BS\n
ALGORITMO_REEMPLAZO=LRU\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-BUDDY-SYSTEM-LRU/Broker_dump.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker-BUDDY-SYSTEM-LRU/Broker_log.log\n
IP_BROKER=\n
PUERTO_BROKER=\n">> Broker.config

cd ../../Debug
make clean
make all

#FIN DE BROKER

cd
