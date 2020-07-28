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
//git checkout develop
cd ..
cp -r tp-2020-1c-wRAPPERS/ /home/utnso/Documentos/tp-2020-1c-wRAPPERS

#FIN DE COPIAR CARPETA

cd

#INICIO DE GAMEBOY

cd Documentos/tp-2020-1c-wRAPPERS

if test -e GameBoy.config; then
	rm GameBoy.config
fi

echo -e "IP_BROKER=\nIP_TEAM=\nIP_GAMECARD=\nPUERTO_BROKER=\nPUERTO_TEAM=\nPUERTO_GAMECARD=\nLOG_FILE=/home/utnso/Documentos/Delibird/GameBoy/tp-2020-1c-wRAPPERS" >> GameBoy.config

cd ../../Debug
make clean
make all

#FIN DE GAMEBOY

#INICIO DE GAMECARD
cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/

if test -e gamecard.config; then
	rm gamecard.config
fi
#REVISAR -> No encuentro los parametros del gamecard(config en el archivo)
echo -e "TIEMPO_DE_REINTENTO_CONEXION=10\n
TIEMPO_DE_REINTENTO_OPERACION=5\n
TIEMPO_RETARDO_OPERACION=1\n
PUNTO_MONTAJE_TALLGRASS=/home/utnso/tallgrass-base\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
ID_MODULO=20\n
IP_GAMECARD=10.108.32.2\n
PUERTO_GAMECARD=5001\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/log_gamecard_base.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/nuestro_log_gamecard_base.txt" >> gamecard.config

cd ../../Debug
make clean
make all

#FIN DE GAMECARD

#CREO TEAMS

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/

cp -r team-v2/ team-v2-RR
cp -r team-v2/ team-v2-SJF-SD
cp -r team-v2/ team-v2-SJF-CD

#CONFIGURO LOS TEAMS

#Configuro team con FIFO

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=FIFO\n
QUANTUM=0\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2/src/resources/log_team_FIFO.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2/src/resources/nuestro_log_team_FIFO.txt
ID_MODULO=10\n
IP_TEAM=10.108.32.3\n
PUERTO_TEAM=5002" >> team.config

cd ../../Debug
make clean
make all

#Configuro team con RR -> Quantum = 2

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-RR/src/resources/

if test -e team.config; then
	rm team.config
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
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-RR/src/resources/log_team_RR.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-RR/src/resources/nuestro_log_team_RR.txt\n
ID_MODULO=11\n
IP_TEAM=10.108.32.3\n
PUERTO_TEAM=5002" >> team.config

cd ../../Debug
make clean
make all

#Configuro team con SJF - SD

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-SD/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=SJF-SD\n
QUANTUM=0\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-SD/src/resources/log_team_SJF-SD.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-SD/src/resources/nuestro_log_team_SJF-SD.txt\n
ID_MODULO=12\n
IP_TEAM=10.108.32.3\n
PUERTO_TEAM=5002" >> team.config

cd ../../Debug
make clean
make all

#Configuro team con SJF - CD

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-CD/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,3|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=SJF-CD\n
QUANTUM=0\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-CD/src/resources/log_team_SJF-CD.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-CD/src/resources/nuestro_log_team_SJF-CD.txt\n
ID_MODULO=13\n
IP_TEAM=10.108.32.3\n
PUERTO_TEAM=5002" >> team.config

cd ../../Debug
make clean
make all

#FIN DE TEAM

#INICIO BROKER

#CREO LOS BROKERS

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba

cp -r broker-v2/ broker-v2-CONSOLIDACION-LRU
cp -r broker-v2/ broker-v2-COMPACTACION-PD-FIFO
cp -r broker-v2/ broker-v2-COMPACTACION-PD-LRU
cp -r broker-v2/ broker-v2-BUDDY-SYSTEM-FIFO
cp -r broker-v2/ broker-v2-BUDDY-SYSTEM-LRU

#CONFIGURO BROKERS

#Configuro broker consolidacion Particiones dinamicas -FIFO

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
FRECUENCIA_COMPACTACION=10\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2/src/resources/dump_file.txt\n
ID_MODULO=5\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2/src/resources/log_broker.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2/src/resources/mi_log_broker.txt\n">> broker.config

cd ../../Debug
make clean
make all

#Configuro broker consolidacion Particiones dinamicas -LRU

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-CONSOLIDACION-LRU/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=LRU\n
ALGORITMO_PARTICION_LIBRE=FF\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
FRECUENCIA_COMPACTACION=10\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-CONSOLIDACION-LRU/src/resources/dump_file.txt\n
ID_MODULO=5\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-CONSOLIDACION-LRU/src/resources/log_broker.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-CONSOLIDACION-LRU/src/resources/mi_log_broker.txt\n">> broker.config

cd ../../Debug
make clean
make all

#Configuro broker compactacion Particiones Dinamicas

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-COMPACTACION-PD-FIFO/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-COMPACTACION-PD-FIFO/src/resources/dump_file.txt\n
ID_MODULO=6\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-COMPACTACION-PD-FIFO/src/resources/log_broker.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-COMPACTACION-PD-FIFO/src/resources/mi_log_broker.txt\n">> broker.config

cd ../../Debug
make clean
make all

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-COMPACTACION-PD-LRU/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=LRU\n
ALGORITMO_PARTICION_LIBRE=FF\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-COMPACTACION-PD-LRU/src/resources/dump_file.txt\n
ID_MODULO=6\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-COMPACTACION-PD-LRU/src/resources/log_broker.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-COMPACTACION-PD-LRU/src/resources/mi_log_broker.txt\n">> broker.config

cd ../../Debug
make clean
make all

#Configuro broker Buddy System

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-FIFO/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=BS\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-FIFO/src/resources/dump_file.txt\n
ID_MODULO=7\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-FIFO/src/resources/log_broker.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-FIFO/src/resources/mi_log_broker.txt\n">> broker.config

cd ../../Debug
make clean
make all

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-LRU/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=BS\n
ALGORITMO_REEMPLAZO=LRU\n
ALGORITMO_PARTICION_LIBRE=FF\n
IP_BROKER=10.108.32.2\n
PUERTO_BROKER=4444\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-LRU/src/resources/dump_file.txt\n
ID_MODULO=7\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-LRU/src/resources/log_broker.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-LRU/src/resources/mi_log_broker.txt\n">> broker.config

cd ../../Debug
make clean
make all

#FIN DE BROKER

cd
