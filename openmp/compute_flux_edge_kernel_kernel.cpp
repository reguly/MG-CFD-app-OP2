//
// auto-generated by op2.py
//

//user function
#include ".././src/Kernels/flux.h"

#ifdef PAPI
#include <papi.h>
#endif

// host stub function
void op_par_loop_compute_flux_edge_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4
  #ifdef VERIFY_OP2_TIMING
    , double* compute_time_ptr
    , double* sync_time_ptr
  #endif
  , long* iter_counts_ptr
  #ifdef PAPI
  , long_long* restrict event_counts, int event_set, int num_events
  #endif
  )
{

  int nargs = 5;
  op_arg args[5];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  double inner_cpu_t1, inner_cpu_t2, inner_wall_t1, inner_wall_t2;
  double compute_time=0.0, sync_time=0.0;
  op_timing_realloc_manytime(9, omp_get_max_threads());
  op_timers_core(&cpu_t1, &wall_t1);
  double non_thread_walltime = 0.0;
  long iter_counts=0;

  int  ninds   = 2;
  int  inds[5] = {0,0,-1,1,1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: compute_flux_edge_kernel\n");
  }

  // get plan
  #ifdef OP_PART_SIZE_9
    int part_size = OP_PART_SIZE_9;
  #else
    int part_size = OP_part_size;
  #endif

  op_timers_core(&inner_cpu_t1, &inner_wall_t1);
  int set_size = op_mpi_halo_exchanges(set, nargs, args);
  op_timers_core(&inner_cpu_t2, &inner_wall_t2);
  sync_time += inner_wall_t2 - inner_wall_t1;

  if (set->size >0) {

    op_plan *Plan = op_plan_get_stage_upload(name,set,part_size,nargs,args,ninds,inds,OP_STAGE_ALL,0);

    // execute plan
    int block_offset = 0;
    for ( int col=0; col<Plan->ncolors; col++ ){
      if (col==Plan->ncolors_core) {
        op_timers_core(&inner_cpu_t1, &inner_wall_t1);
        op_mpi_wait_all(nargs, args);
        op_timers_core(&inner_cpu_t2, &inner_wall_t2);
        sync_time += inner_wall_t2 - inner_wall_t1;
      }
      int nblocks = Plan->ncolblk[col];

      op_timers_core(&inner_cpu_t1, &inner_wall_t1);
      // Pause process timing and switch to per-thread timing:
      cpu_t2=inner_cpu_t1; wall_t2=inner_wall_t1;
      non_thread_walltime += wall_t2 - wall_t1;
      #pragma omp parallel
      {
        double thr_wall_t1, thr_wall_t2, thr_cpu_t1, thr_cpu_t2;
        op_timers_core(&thr_cpu_t1, &thr_wall_t1);

        int nthreads = omp_get_num_threads();
        int thr = omp_get_thread_num();
        int thr_start = (nblocks * thr) / nthreads;
        int thr_end = (nblocks * (thr+1)) / nthreads;
        if (thr_end > nblocks) thr_end = nblocks;
        for ( int blockIdx=thr_start; blockIdx<thr_end; blockIdx++ ){
          int blockId  = Plan->blkmap[blockIdx + block_offset];
          int nelem    = Plan->nelems[blockId];
          int offset_b = Plan->offset[blockId];
          for ( int n=offset_b; n<offset_b+nelem; n++ ){
            int map0idx = arg0.map_data[n * arg0.map->dim + 0];
            int map1idx = arg0.map_data[n * arg0.map->dim + 1];


            compute_flux_edge_kernel(
              &((double*)arg0.data)[5 * map0idx],
              &((double*)arg0.data)[5 * map1idx],
              &((double*)arg2.data)[3 * n],
              &((double*)arg3.data)[5 * map0idx],
              &((double*)arg3.data)[5 * map1idx]);
          }
        }

        op_timers_core(&thr_cpu_t2, &thr_wall_t2);
        OP_kernels[9].times[thr]  += thr_wall_t2 - thr_wall_t1;
      }

      // Revert to process-level timing:
      op_timers_core(&cpu_t1, &wall_t1);

      op_timers_core(&inner_cpu_t2, &inner_wall_t2);
      compute_time += inner_wall_t2 - inner_wall_t1;

      block_offset += nblocks;
    }

    block_offset = 0;
    for ( int col=0; col<Plan->ncolors; col++ ){
      int nblocks = Plan->ncolblk[col];
      for ( int blockIdx=0; blockIdx<nblocks; blockIdx++ ){
        int blockId  = Plan->blkmap[blockIdx + block_offset];
        iter_counts += Plan->nelems[blockId];
      }
      block_offset += nblocks;
    }

    OP_kernels[9].transfer  += Plan->transfer;
    OP_kernels[9].transfer2 += Plan->transfer2;
  }

  op_timers_core(&inner_cpu_t1, &inner_wall_t1);
  if (set_size == 0 || set_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);
  op_timers_core(&inner_cpu_t2, &inner_wall_t2);
  sync_time += inner_wall_t2 - inner_wall_t1;

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  non_thread_walltime += wall_t2 - wall_t1;
  OP_kernels[9].name      = name;
  OP_kernels[9].count    += 1;
  OP_kernels[9].times[0] += non_thread_walltime;

  #ifdef VERIFY_OP2_TIMING
    *compute_time_ptr += compute_time;
    *sync_time_ptr += sync_time;
  #endif
  *iter_counts_ptr += iter_counts;
}
