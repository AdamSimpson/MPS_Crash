#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"
#include "cuda_runtime.h"
#include <unistd.h> // sleep()

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
void gpuAssert(cudaError_t code, const char *file, int line)
{
   if (code != cudaSuccess)
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      exit(code);
   }
}

void check_GPUs() {
  system("nvidia-smi >> nvidia_smi.txt");
  system("echo '***********************\n\n' >> nvidia_smi.txt");
}

void check_MPS() {
 system("ps aux | grep nvidia-cuda-mps | grep -v grep > /dev/null >> mps_status.txt");
 system("echo '**********************\n\n' >> mps_status.txt");
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0) {
      check_MPS();
      check_GPUs();
    }

    // Number of GPUs per node
    int GPU_COUNT = 4;

    // Set ranks GPU
    int gpu_id = rank%GPU_COUNT;
    cudaSetDevice(gpu_id);

    int *send_buffs[8];
    int *recv_buffs[8];
    int *d_send_buffs[8];
    int *d_recv_buffs[8];

    int count = 1000;
    size_t size_buff = count*sizeof(int);
    cudaError_t d_err;
    for(int i=0; i<8; i++) {
      // Allocate host memory
      send_buffs[i] = (int*)malloc(size_buff);
      recv_buffs[i] = (int*)malloc(size_buff);

      // Allocate device memory
      d_err = cudaMalloc((void**)(d_send_buffs + i), size_buff);
      gpuErrchk( d_err );
      d_err = cudaMalloc((void**)(d_recv_buffs + i), size_buff);
      gpuErrchk( d_err );

      // Fill host memory
      for(int j=0; j<count; j++) {
        send_buffs[i][j] = rank;
        recv_buffs[i][j] = -1;
      }
 
      // Copy memory to devices
      d_err = cudaMemcpy(d_send_buffs[i], send_buffs[i], count*sizeof(int), cudaMemcpyHostToDevice);
      gpuErrchk( d_err );
      d_err = cudaMemcpy(d_recv_buffs[i], recv_buffs[i], count*sizeof(int), cudaMemcpyHostToDevice);
      gpuErrchk( d_err );

      if(rank == 0) {
        check_MPS();
        check_GPUs();
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0)
      printf("__test_success__\n");

    for(int i=0; i<8; i++) {
      free(send_buffs[i]);
      free(recv_buffs[i]);
      d_err = cudaFree(d_send_buffs[i]);
      gpuErrchk( d_err );
      d_err = cudaFree(d_recv_buffs[i]);
      gpuErrchk( d_err );
    }

    MPI_Finalize();

    #ifdef ADD_SLEEP
    sleep(20);
    #endif

    return 0;
}
