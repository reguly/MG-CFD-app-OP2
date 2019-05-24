//
// auto-generated by op2.py
//

#include <math.h>
#include "const.h"

//user function
__device__ void up_kernel_gpu( 
    const double* variable,
    double* variable_above,
    int* up_scratch) {
    variable_above[VAR_DENSITY]        += variable[VAR_DENSITY];
    variable_above[VAR_MOMENTUM+0]     += variable[VAR_MOMENTUM+0];
    variable_above[VAR_MOMENTUM+1]     += variable[VAR_MOMENTUM+1];
    variable_above[VAR_MOMENTUM+2]     += variable[VAR_MOMENTUM+2];
    variable_above[VAR_DENSITY_ENERGY] += variable[VAR_DENSITY_ENERGY];
    *up_scratch += 1;

}

// CUDA kernel function
__global__ void op_cuda_up_kernel(
  double *__restrict ind_arg0,
  int *__restrict ind_arg1,
  const int *__restrict opDat1Map,
  const double *__restrict arg0,
  int    block_offset,
  int   *blkmap,
  int   *offset,
  int   *nelems,
  int   *ncolors,
  int   *colors,
  int   nblocks,
  int   set_size) {
  double arg1_l[5];
  int arg2_l[1];

  __shared__ int    nelems2, ncolor;
  __shared__ int    nelem, offset_b;

  extern __shared__ char shared[];

  if (blockIdx.x+blockIdx.y*gridDim.x >= nblocks) {
    return;
  }
  if (threadIdx.x==0) {

    //get sizes and shift pointers and direct-mapped data

    int blockId = blkmap[blockIdx.x + blockIdx.y*gridDim.x  + block_offset];

    nelem    = nelems[blockId];
    offset_b = offset[blockId];

    nelems2  = blockDim.x*(1+(nelem-1)/blockDim.x);
    ncolor   = ncolors[blockId];

  }
  __syncthreads(); // make sure all of above completed

  for ( int n=threadIdx.x; n<nelems2; n+=blockDim.x ){
    int col2 = -1;
    int map1idx;
    if (n<nelem) {
      //initialise local variables
      for ( int d=0; d<5; d++ ){
        arg1_l[d] = ZERO_double;
      }
      for ( int d=0; d<1; d++ ){
        arg2_l[d] = ZERO_int;
      }
      map1idx = opDat1Map[n + offset_b + set_size * 0];


      //user-supplied kernel call
      up_kernel_gpu(arg0+(n+offset_b)*5,
              arg1_l,
              arg2_l);
      col2 = colors[n+offset_b];
    }

    //store local variables

    for ( int col=0; col<ncolor; col++ ){
      if (col2==col) {
        arg1_l[0] += ind_arg0[0+map1idx*5];
        arg1_l[1] += ind_arg0[1+map1idx*5];
        arg1_l[2] += ind_arg0[2+map1idx*5];
        arg1_l[3] += ind_arg0[3+map1idx*5];
        arg1_l[4] += ind_arg0[4+map1idx*5];
        ind_arg0[0+map1idx*5] = arg1_l[0];
        ind_arg0[1+map1idx*5] = arg1_l[1];
        ind_arg0[2+map1idx*5] = arg1_l[2];
        ind_arg0[3+map1idx*5] = arg1_l[3];
        ind_arg0[4+map1idx*5] = arg1_l[4];
        arg2_l[0] += ind_arg1[0+map1idx*1];
        ind_arg1[0+map1idx*1] = arg2_l[0];
      }
      __syncthreads();
    }
  }
}


//host stub function
void op_par_loop_up_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2){

  int nargs = 3;
  op_arg args[3];
  const int nk = 16;

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(nk);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[nk].name      = name;
  OP_kernels[nk].count    += 1;


  int    ninds   = 2;
  int    inds[3] = {-1,0,1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: up_kernel\n");
  }

  //get plan
  #ifdef OP_PART_SIZE_16
    int part_size = OP_PART_SIZE_16;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    op_plan *Plan = op_plan_get(name,set,part_size,nargs,args,ninds,inds);

    //execute plan

    int block_offset = 0;
    for ( int col=0; col<Plan->ncolors; col++ ){
      if (col==Plan->ncolors_core) {
        op_mpi_wait_all_cuda(nargs, args);
      }
      #ifdef OP_BLOCK_SIZE_16
      int nthread = OP_BLOCK_SIZE_16;
      #else
      int nthread = OP_block_size;
      #endif

      dim3 nblocks = dim3(Plan->ncolblk[col] >= (1<<16) ? 65535 : Plan->ncolblk[col],
      Plan->ncolblk[col] >= (1<<16) ? (Plan->ncolblk[col]-1)/65535+1: 1, 1);
      if (Plan->ncolblk[col] > 0) {
        op_cuda_up_kernel<<<nblocks,nthread>>>(
        (double *)arg1.data_d,
        (int *)arg2.data_d,
        arg1.map_data_d,
        (double*)arg0.data_d,
        block_offset,
        Plan->blkmap,
        Plan->offset,
        Plan->nelems,
        Plan->nthrcol,
        Plan->thrcol,
        Plan->ncolblk[col],
        set->size+set->exec_size);

      }
      block_offset += Plan->ncolblk[col];
    }
    OP_kernels[nk].transfer  += Plan->transfer;
    OP_kernels[nk].transfer2 += Plan->transfer2;
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  cutilSafeCall(cudaDeviceSynchronize());
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[nk].time     += wall_t2 - wall_t1;
}