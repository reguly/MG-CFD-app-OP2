//
// auto-generated by op2.py
//

//user function
#include ".././src/Kernels/mg.h"

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
  op_timing_realloc_manytime(18, omp_get_max_threads());
  op_timers_core(&cpu_t1, &wall_t1);
  double non_thread_walltime = 0.0;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  up_post_kernel");
  }

  op_mpi_halo_exchanges(set, nargs, args);
  // set number of threads
  #ifdef _OPENMP
    int nthreads = omp_get_max_threads();
  #else
    int nthreads = 1;
  #endif

  if (set->size >0) {

    // execute plan
    // Pause process timing, and switch to per-thread timing:
    op_timers_core(&cpu_t2, &wall_t2);
    non_thread_walltime += wall_t2 - wall_t1;
    #pragma omp parallel for
    for ( int thr=0; thr<nthreads; thr++ ){
      double thr_wall_t1, thr_wall_t2, thr_cpu_t1, thr_cpu_t2;
      op_timers_core(&thr_cpu_t1, &thr_wall_t1);
      int start  = (set->size* thr)/nthreads;
      int finish = (set->size*(thr+1))/nthreads;
      for ( int n=start; n<finish; n++ ){
        up_post_kernel(
          &((double*)arg0.data)[5*n],
          &((int*)arg1.data)[1*n]);
      }
      op_timers_core(&thr_cpu_t2, &thr_wall_t2);
      OP_kernels[18].times[thr]  += thr_wall_t2 - thr_wall_t1;
    }

    // OpenMP block complete, so switch back to process timing:
    op_timers_core(&cpu_t1, &wall_t1);
  }

  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  non_thread_walltime += wall_t2 - wall_t1;
  OP_kernels[18].name      = name;
  OP_kernels[18].count    += 1;
  OP_kernels[18].times[0] += non_thread_walltime;
  OP_kernels[18].transfer += (float)set->size * arg0.size * 2.0f;
  OP_kernels[18].transfer += (float)set->size * arg1.size;
}
