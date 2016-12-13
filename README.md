# To build on summitdev

```
module switch xl pgi
module load cmake cuda

```
$ mkdir build
$ cd build
$ CC=pgcc CXX=pgc++ cmake ..
$ make
$ cd ..
$ cp build/MPS_crash .
$ bsub -env "all,LSB_START_JOB_MPS=y" -R "rusage[ngpus_excl_p=4]" < submit.lsf
```
