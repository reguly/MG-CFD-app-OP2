//
// auto-generated by op2.py
//

#include <math.h>
#include <cmath>
#include "const.h"
#include "inlined_funcs.h"

//user function
__device__ void time_step_kernel_gpu( 
    const int* rkCycle,
    const double* step_factor,
    double* flux,
    const double* old_variable,
    double* variable) {
    double factor = (*step_factor)/double(RK+1-(*rkCycle));

    variable[VAR_DENSITY]        = old_variable[VAR_DENSITY]        + factor*flux[VAR_DENSITY];
    variable[VAR_MOMENTUM+0]     = old_variable[VAR_MOMENTUM+0]     + factor*flux[VAR_MOMENTUM+0];
    variable[VAR_MOMENTUM+1]     = old_variable[VAR_MOMENTUM+1]     + factor*flux[VAR_MOMENTUM+1];
    variable[VAR_MOMENTUM+2]     = old_variable[VAR_MOMENTUM+2]     + factor*flux[VAR_MOMENTUM+2];
    variable[VAR_DENSITY_ENERGY] = old_variable[VAR_DENSITY_ENERGY] + factor*flux[VAR_DENSITY_ENERGY];

    flux[VAR_DENSITY]        = 0.0;
    flux[VAR_MOMENTUM+0]     = 0.0;
    flux[VAR_MOMENTUM+1]     = 0.0;
    flux[VAR_MOMENTUM+2]     = 0.0;
    flux[VAR_DENSITY_ENERGY] = 0.0;

}

// CUDA kernel function
__global__ void op_cuda_time_step_kernel(
  const int *arg0,
  const double *__restrict arg1,
  double *arg2,
  const double *__restrict arg3,
  double *arg4,
  int   set_size ) {


  //process set elements
  for ( int n=threadIdx.x+blockIdx.x*blockDim.x; n<set_size; n+=blockDim.x*gridDim.x ){

    //user-supplied kernel call
    time_step_kernel_gpu(arg0,
                     arg1+n*1,
                     arg2+n*5,
                     arg3+n*5,
                     arg4+n*5);
  }
}


//host stub function
void op_par_loop_time_step_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4){

  int*arg0h = (int *)arg0.data;
  int nargs = 5;
  op_arg args[5];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(11);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[11].name      = name;
  OP_kernels[11].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  time_step_kernel");
  }

  op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    //transfer constants to GPU
    int consts_bytes = 0;
    consts_bytes += ROUND_UP(1*sizeof(int));
    reallocConstArrays(consts_bytes);
    consts_bytes = 0;
    arg0.data   = OP_consts_h + consts_bytes;
    arg0.data_d = OP_consts_d + consts_bytes;
    for ( int d=0; d<1; d++ ){
      ((int *)arg0.data)[d] = arg0h[d];
    }
    consts_bytes += ROUND_UP(1*sizeof(int));
    mvConstArraysToDevice(consts_bytes);

    //set CUDA execution parameters
    #ifdef OP_BLOCK_SIZE_11
      int nthread = OP_BLOCK_SIZE_11;
    #else
      int nthread = OP_block_size;
    //  int nthread = 128;
    #endif

    int nblocks = 200;

    op_cuda_time_step_kernel<<<nblocks,nthread>>>(
      (int *) arg0.data_d,
      (double *) arg1.data_d,
      (double *) arg2.data_d,
      (double *) arg3.data_d,
      (double *) arg4.data_d,
      set->size );
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  cutilSafeCall(cudaDeviceSynchronize());
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[11].time     += wall_t2 - wall_t1;
  OP_kernels[11].transfer += (float)set->size * arg1.size;
  OP_kernels[11].transfer += (float)set->size * arg2.size * 2.0f;
  OP_kernels[11].transfer += (float)set->size * arg3.size;
  OP_kernels[11].transfer += (float)set->size * arg4.size * 2.0f;
}
