//
// auto-generated by op2.py
//

//user function
#include ".././src/Kernels/validation.h"

// host stub function
void op_par_loop_residual_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2){

  int nargs = 3;
  op_arg args[3];
  const int nk = 12;

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(nk);
  op_timers_core(&cpu_t1, &wall_t1);


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  residual_kernel");
  }

  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  if (set->size >0) {

    for ( int n=0; n<set_size; n++ ){
      residual_kernel(
        &((double*)arg0.data)[5*n],
        &((double*)arg1.data)[5*n],
        &((double*)arg2.data)[5*n]);
    }
  }

  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[nk].name      = name;
  OP_kernels[nk].count    += 1;
  OP_kernels[nk].time     += wall_t2 - wall_t1;
  OP_kernels[nk].transfer += (float)set->size * arg0.size;
  OP_kernels[nk].transfer += (float)set->size * arg1.size;
  OP_kernels[nk].transfer += (float)set->size * arg2.size * 2.0f;
}