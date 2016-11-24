Distributed vector retrieval 
====================

Building
----------
To build this project following libraries are needed
*   Boost
*   a MPI implementation

Building is done via cmake.

Ilux150/HTW Linux Hosts:
CMake automatically finds boost if installed in `$HOME/include` and `$HOME/lib`, but MPI is usally not found.
So one has to set the 2 CMake cache variables MPI_C_LIBRARIES and MPI_C_INCLUDE_PATH for C 
and MPI_CXX_LIBRARIES and MPI_CXX_INCLUDE_PATH for C++. 
But there is already a convenience `configure` script, simply do:
```
#git clone git@github.com:Superlokkus/dist_vec_ret.git
#cd dist_vec_ret
./configure_htw_linux
```
or add `-D"MPI_C_LIBRARIES=$HOME/mpich-install/lib64/libmpi.so;$HOME/mpich-install/lib64/libpmpi.so" -D"MPI_C_INCLUDE_PATH=$HOME/mpich-install/include" -D"MPI_CXX_LIBRARIES=$HOME/mpich-install/lib64/libmpicxx.so;$HOME/mpich-install/lib64/libmpi.so;$HOME/mpich-install/lib64/libpmpi.so"` to your cmake call

then one can simply `make`, `make test` as usual. 