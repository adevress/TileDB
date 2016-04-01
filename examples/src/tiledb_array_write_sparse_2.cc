/*
 * File: tiledb_array_write_sparse_2.cc
 * 
 * It shows how to write to a sparse array in two sorted batches.
 */

#include "c_api.h"

int main() {
  // Intialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Initialize array
  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                // Context 
      &tiledb_array,                             // Array object
      "my_workspace/sparse_arrays/my_array_B",   // Array name
      TILEDB_ARRAY_WRITE,                        // Mode
      NULL,                                      // Entire domain
      NULL,                                      // All attributes
      0);                                        // Number of attributes

  // Prepare cell buffers - #1
  int buffer_a1[] = { 0, 1, 2 };
  size_t buffer_a2[] = { 0, 1, 3, 6, 10, 11, 13, 16 };
  const char buffer_var_a2[] = "abbcccddddeffggghhhh";
  float buffer_a3[] = 
  {
      0.1,  0.2,  1.1,  1.2,  2.1,  2.2,  3.1,  3.2,
      4.1,  4.2,  5.1,  5.2,  6.1,  6.2,  7.1,  7.2 
  };
  int64_t buffer_coords[] = { 1, 1, 1, 2 };
  const void* buffers[] = 
      { buffer_a1, buffer_a2, buffer_var_a2, buffer_a3, buffer_coords };
  size_t buffer_sizes[] = 
  { 
      sizeof(buffer_a1),  
      sizeof(buffer_a2),
      sizeof(buffer_var_a2)-1,  // No need to store the last '\0' character
      sizeof(buffer_a3),
      sizeof(buffer_coords)
  };

  // Write to array - #1
  tiledb_array_write(tiledb_array, buffers, buffer_sizes); 

  // Prepare cell buffers - #2
  int buffer_a1_2[] = { 3, 4, 5, 6, 7 };
  size_t* buffer_a2_2;
  const char* buffer_var_a2_2;
  float* buffer_a3_2; 
  int64_t buffer_coords_2[] = { 1, 4, 2, 3, 3, 3, 3, 4, 4, 2, 3, 1 };
  const void* buffers_2[] = 
  { 
       buffer_a1_2, 
       buffer_a2_2, 
       buffer_var_a2_2, 
       buffer_a3_2, 
       buffer_coords_2 };
  size_t buffer_sizes_2[] = 
  { 
      sizeof(buffer_a1_2),  
      0,
      0,
      0,
      sizeof(buffer_coords_2)
  };

  // Write to array - #2
  tiledb_array_write(tiledb_array, buffers_2, buffer_sizes_2);

  // Finalize array
  tiledb_array_finalize(tiledb_array);

  // Finalize context
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}