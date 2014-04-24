#!bin/bash

# number of nodes
N=16
# internal communication protocol
ICONN="shm"
# external communication protocol
ECONN="dapl"

EXE="./du4"
PARAMS=""

mpiexec.hydra -n $N -f ./mpd.hosts -rr -env I_MPI_FABRICS $ICONN:$ECONN -env I_MPI_FALLBACK_DEVICE 0 $EXE $PARAMS
