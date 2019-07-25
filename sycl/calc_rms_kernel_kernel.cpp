//
// auto-generated by op2.py
//

#include "utils.h"

//user function
class calc_rms_kernel_kernel;

//host stub function
void op_par_loop_calc_rms_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1){

  double*arg1h = (double *)arg1.data;
  int nargs = 2;
  op_arg args[2];

  args[0] = arg0;
  args[1] = arg1;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(13);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[13].name      = name;
  OP_kernels[13].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  calc_rms_kernel");
  }

  op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    //set SYCL execution parameters
    #ifdef OP_BLOCK_SIZE_13
      int nthread = OP_BLOCK_SIZE_13;
    #else
      int nthread = OP_block_size;
    #endif

    int nblocks = 200;

    //transfer global reduction data to GPU
    int maxblocks = nblocks;
    int reduct_bytes = 0;
    int reduct_size  = 0;
    reduct_bytes += ROUND_UP(maxblocks*1*sizeof(double));
    reduct_size   = MAX(reduct_size,sizeof(double));
    reallocReductArrays(reduct_bytes);
    reduct_bytes = 0;
    arg1.data   = OP_reduct_h + reduct_bytes;
    int arg1_offset = reduct_bytes/sizeof(double);
    for ( int b=0; b<maxblocks; b++ ){
      for ( int d=0; d<1; d++ ){
        ((double *)arg1.data)[d+b*1] = ZERO_double;
      }
    }
    reduct_bytes += ROUND_UP(maxblocks*1*sizeof(double));
    mvReductArraysToDevice(reduct_bytes);
    cl::sycl::buffer<double,1> *reduct = static_cast<cl::sycl::buffer<double,1> *>((void*)OP_reduct_d);

    cl::sycl::buffer<double,1> *arg0_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg0.data_d);
    int set_size = set->size+set->exec_size;
    try {
    op2_queue->submit([&](cl::sycl::handler& cgh) {
      auto arg0 = (*arg0_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
      auto reduct1 = (*reduct).template get_access<cl::sycl::access::mode::read_write>(cgh);
      cl::sycl::accessor<double, 1, cl::sycl::access::mode::read_write,
         cl::sycl::access::target::local> red_double(nthread, cgh);

      //user fun as lambda
      auto calc_rms_kernel_gpu = [=]( 
            const double* residual,
            double* rms) {
            for (int i=0; i<NVAR; i++) {
                *rms += residual[i]*residual[i];
            }
        
        };
        
      auto kern = [=](cl::sycl::nd_item<1> item) {
        double arg1_l[1];
        for ( int d=0; d<1; d++ ){
          arg1_l[d]=ZERO_double;
        }

        //process set elements
        for ( int n=item.get_global_linear_id(); n<set_size; n+=item.get_global_range()[0] ){

          //user-supplied kernel call
          calc_rms_kernel_gpu(&arg0[n*5],
                    arg1_l);
        }

        //global reductions

        for ( int d=0; d<1; d++ ){
          op_reduction<OP_INC>(reduct1,arg1_offset+d+item.get_group_linear_id()*1,arg1_l[d],red_double,item);
        }

      };
      cgh.parallel_for<class calc_rms_kernel_kernel>(cl::sycl::nd_range<1>(nthread*nblocks,nthread), kern);
    });
    }catch(cl::sycl::exception const &e) {
    std::cout << e.what() << std::endl;exit(-1);
    }
    //transfer global reduction data back to CPU
    mvReductArraysToHost(reduct_bytes);
    for ( int b=0; b<maxblocks; b++ ){
      for ( int d=0; d<1; d++ ){
        arg1h[d] = arg1h[d] + ((double *)arg1.data)[d+b*1];
      }
    }
    arg1.data = (char *)arg1h;
    op_mpi_reduce(&arg1,arg1h);
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  op2_queue->wait();
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[13].time     += wall_t2 - wall_t1;
  OP_kernels[13].transfer += (float)set->size * arg0.size;
}
