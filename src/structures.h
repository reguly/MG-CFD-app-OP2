#ifndef STRUCTURES_H
#define STRUCTURES_H

#ifndef __CUDACC__
  struct float3 { float x, y, z; };
#else
  // nvcc will pull in '/usr/include/vector_types.h' which 
  // contains an identical definition of 'float3' (above).
#endif

struct edge { float a, b; };
struct edge_neighbour { int a, b; float x, y, z; };

#endif
