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

cd tp-2020-1c-wRAPPERS
git checkout prueba --
cd ..
cp -r tp-2020-1c-wRAPPERS/ /home/utnso/Documentos/tp-2020-1c-wRAPPERS

#FIN DE COPIAR CARPETA

cd

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
#CREO TEAMS

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/

cp -r Team/ Team-RR
cp -r Team/ Team-SJF-SD
cp -r Team/ Team-SJF-CD

#CONFIGURO LOS TEAMS
cd
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
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team_FIFO.log\n
PROCESS_ID=1234\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug
make clean
make all

cd
#Configuro Team con RR -> Quantum = 2

cd /home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team-RR/

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
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team_RR.log\n
PROCESS_ID=1235\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug

make clean
make all

cd
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
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team_SJF_SD.log\n
PROCESS_ID=1236\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug
make clean
make all

cd
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
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team_SJF_CD.log\n
PROCESS_ID=1237\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5555\n" >> Team.config

cd Debug
make clean
make all

#FIN DE TEAM

cd