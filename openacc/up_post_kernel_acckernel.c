//
// auto-generated by op2.py
//

//user function
#include <math.h>
#include "const.h"

//user function
//#pragma acc routine
inline void up_post_kernel_openacc( 
    double* variable,
    const int* up_scratch) {
    double avg = (*up_scratch)==0 ? 1.0 : 1.0 / (double)(*up_scratch);
    variable[VAR_DENSITY] *= avg;
    variable[VAR_MOMENTUM+0] *= avg;
    variable[VAR_MOMENTUM+1] *= avg;
    variable[VAR_MOMENTUM+2] *= avg;
    variable[VAR_DENSITY_ENERGY] *= avg;
}

// host stub function
void op_par_loop_up_post_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1){

  int nargs = 2;
  op_arg args[2];

  args[0] = arg0;
  args[1] = arg1;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(18);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[18].name      = name;
  OP_kernels[18].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  up_post_kernel");
  }

  op_mpi_halo_exchanges_cuda(set, nargs, args);


  if (set->size >0) {


    //Set up typed device pointers for OpenACC

    double* data0 = (double*)arg0.data_d;
    int* data1 = (int*)arg1.data_d;
    #pragma acc parallel loop independent deviceptr(data0,data1)
    for ( int n=0; n<set->size; n++ ){
      up_post_kernel_openacc(
        &data0[5*n],
        &data1[1*n]);
    }
  }

  // combine reduction data
  op_mpi_set_dirtybit_cuda(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[18].time     += wall_t2 - wall_t1;
  OP_kernels[18].transfer += (float)set->size * arg0.size * 2.0f;
  OP_kernels[18].transfer += (float)set->size * arg1.size;
}
