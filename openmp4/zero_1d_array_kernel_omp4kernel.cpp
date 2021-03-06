//
// auto-generated by op2.py
//

//user function
//user function

void zero_1d_array_kernel_omp4_kernel(
  double *data0,
  int dat0size,
  int count,
  int num_teams,
  int nthread);

// host stub function
void op_par_loop_zero_1d_array_kernel(char const *name, op_set set,
  op_arg arg0){

  int nargs = 1;
  op_arg args[1];

  args[0] = arg0;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(2);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[2].name      = name;
  OP_kernels[2].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  zero_1d_array_kernel");
  }

  op_mpi_halo_exchanges_cuda(set, nargs, args);

  #ifdef OP_PART_SIZE_2
    int part_size = OP_PART_SIZE_2;
  #else
    int part_size = OP_part_size;
  #endif
  #ifdef OP_BLOCK_SIZE_2
    int nthread = OP_BLOCK_SIZE_2;
  #else
    int nthread = OP_block_size;
  #endif


  if (set->size >0) {

    //Set up typed device pointers for OpenMP

    double* data0 = (double*)arg0.data_d;
    int dat0size = getSetSizeFromOpArg(&arg0) * arg0.dat->dim;
    zero_1d_array_kernel_omp4_kernel(
      data0,
      dat0size,
      set->size,
      part_size!=0?(set->size-1)/part_size+1:(set->size-1)/nthread,
      nthread);

  }

  // combine reduction data
  op_mpi_set_dirtybit_cuda(nargs, args);

  if (OP_diags>1) deviceSync();
  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[2].time     += wall_t2 - wall_t1;
  OP_kernels[2].transfer += (float)set->size * arg0.size * 2.0f;
}
