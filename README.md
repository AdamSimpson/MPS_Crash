# To build

## Setup environment on summitdev
```
module load cmake cuda
```

## Build the MPS_crash reproducer
```
$ mkdir build
$ cd build
$ CC=gcc CXX=g++ cmake ..
$ make
$ cd ..
$ cp build/MPS_crash .
$ bsub -env "all,LSB_START_JOB_MPS=y" -R "rusage[ngpus_excl_p=4]" < submit.lsf
```
