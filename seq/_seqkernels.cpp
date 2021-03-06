//
// auto-generated by op2.py
//

// global constants

// header
#include "op_lib_cpp.h"

#ifdef PAPI
#include <papi.h>
#endif
void op_par_loop_compute_flux_edge_kernel_instrumented(
  char const *name, op_set set,
  op_arg arg0, op_arg arg1, op_arg arg2, op_arg arg3, op_arg arg4
  #ifdef VERIFY_OP2_TIMING
    , double* compute_time_ptr, double* sync_time_ptr
  #endif
  , long* iter_counts_ptr
  #ifdef PAPI
  , long_long* restrict event_counts, int event_set, int num_events
  #endif
);
// user kernel files
#include "initialize_variables_kernel_seqkernel.cpp"
#include "zero_5d_array_kernel_seqkernel.cpp"
#include "zero_1d_array_kernel_seqkernel.cpp"
#include "calculate_cell_volumes_seqkernel.cpp"
#include "dampen_ewt_seqkernel.cpp"
#include "copy_double_kernel_seqkernel.cpp"
#include "calculate_dt_kernel_seqkernel.cpp"
#include "get_min_dt_kernel_seqkernel.cpp"
#include "compute_step_factor_kernel_seqkernel.cpp"
#include "compute_flux_edge_kernel_seqkernel.cpp"
#include "compute_bnd_node_flux_kernel_seqkernel.cpp"
#include "time_step_kernel_seqkernel.cpp"
#include "indirect_rw_kernel_seqkernel.cpp"
#include "residual_kernel_seqkernel.cpp"
#include "calc_rms_kernel_seqkernel.cpp"
#include "count_bad_vals_seqkernel.cpp"
#include "up_pre_kernel_seqkernel.cpp"
#include "up_kernel_seqkernel.cpp"
#include "up_post_kernel_seqkernel.cpp"
#include "down_v2_kernel_pre_seqkernel.cpp"
#include "down_v2_kernel_seqkernel.cpp"
#include "down_v2_kernel_post_seqkernel.cpp"
#include "down_kernel_seqkernel.cpp"
#include "identify_differences_seqkernel.cpp"
#include "count_non_zeros_seqkernel.cpp"
