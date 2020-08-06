# tp-2020-1c-wRAPPERS
# PRUEBA COMPLETA

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

if test -d /home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/; then
	sudo rm -r /home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/
fi

mkdir Documentos

cd tp-2020-1c-wRAPPERS

cp -r tp-2020-1c-wRAPPERS/ /home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress

#STRESS

if test -d /home/utnso/Documentos/tallgrass-stress/; then
	sudo rm -r /home/utnso/Documentos/tallgrass-stress/
fi

cd
mkdir -p /home/utnso/Documentos/tallgrass-stress/

#FIN STRESS

cd

#INICIO DE GAMEBOY

cd home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/GameBoy/

if test -e GameBoy.config; then
	rm GameBoy.config
fi

echo -e "IP_BROKER=127.0.0.1\nIP_TEAM=127.0.0.2\nIP_GAMECARD=127.0.0.4\nPUERTO_BROKER=4444\nPUERTO_TEAM=5555\nPUERTO_GAMECARD=7777\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/GameBoy/GameBoy.log\nPROCESS_ID=2808" >> GameBoy.config

cd Debug
make clean
make all

#FIN DE GAMEBOY

cd

#INICIO DE GAMECARD
cd home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/GameCard

if test -e GameCard.config; then
	rm GameCard.config
fi

echo -e "BLOCKS=1024\n
BLOCK_SIZE=64\n
MAGIC_NUMBER=TALL_GRASS\n
PUNTO_MONTAJE_TALLGRASS=/home/utnso/Documentos/tallgrass-stress\n
TIEMPO_DE_REINTENTO_CONEXION=10\n
TIEMPO_DE_REINTENTO_OPERACION=5\n
TIEMPO_RETARDO_OPERACION=1\n
PUERTO_BROKER=4444\n
IP_BROKER=127.0.0.1\n
PROCESS_ID=5678\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/GameCard/GameCard.log\n
PUERTO_GAMECARD=7777\n
IP_GAMECARD=127.0.0.4\n" >> GameCard.config

cd Debug
make clean
make all

#FIN DE GAMECARD

cd

#INICIO BROKER

#CREO LOS BROKERS

cd home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/

cp -r Broker/ Broker-Particiones-dinamicas-stress
cp -r Broker/ Broker-BUDDY-SYSTEM-stress

#CONFIGURO BROKERS

#Configuro broker consolidacion Particiones dinamicas

cd home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Broker-Particiones-dinamicas-stress/

if test -e Broker.config; then
	rm Broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Broker-Particiones-dinamicas-stress/Broker.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Broker-Particiones-dinamicas-stress/Broker.log\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n">> Broker.config

cd Debug
make clean
make all

cd
#Configuro broker Buddy System

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Broker-BUDDY-SYSTEM-stress/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=BS\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Broker-BUDDY-SYSTEM-stress/Broker.dump\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Broker-BUDDY-SYSTEM-stress/Broker.log\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n">> Broker.config

cd Debug
make clean
make all
#FIN DE BROKER

cd

#CREO TEAMS

cd home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/

cp -r Team/ Team-FIFO-stress
cp -r Team/ Team-RR-stress
cp -r Team/ Team-SJF-SD-stress
cp -r Team/ Team-SJF-CD-stress

#CONFIGURO LOS TEAMS

cd

#Configuro team con FIFO

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/Team-FIFO-stress/

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
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Team-FIFO-stress/Team.log\n
PROCESS_ID=1234\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug
make clean
make all

cd

#Configuro team con RR -> Quantum = 1

cd home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Team-RR-stress

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[2|3,6|5,9|9,9|2,2|9]\n
POKEMON_ENTRENADORES=[]\n
OBJETIVOS_ENTRENADORES=[Vaporeon,Jolteon,Flareon,Umbreon,Espeon]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=RR\n
QUANTUM=1\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Team-RR-stress/Team.log\n
PROCESS_ID=1234\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug
make clean
make all

#Configuro team con SJF - SD

cd home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Team-SJF-SD-stress

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
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Team-SJF-SD-stress/Team.log\n
PROCESS_ID=1234\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug
make clean
make all

#Configuro team con SJF - CD

cd home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Team-SJF-CD-stress

if test -e Team.config; then
	rm Team.config
fi

echo -e "POSICIONES_ENTRENADORES=[2|3,6|5,9|9,9|2,2|9]\n
POKEMON_ENTRENADORES=[]\n
OBJETIVOS_ENTRENADORES=[Vaporeon,Jolteon,Flareon,Umbreon,Espeon]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=SJF-CD\n
QUANTUM=1\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS-stress/Team-SJF-CD-stress/Team.log\n
PROCESS_ID=1235\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug
make clean
make all

#FIN DE TEAM

cd