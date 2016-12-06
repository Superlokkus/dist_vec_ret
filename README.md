Distributed vector retrieval 
====================

Building
----------
To build this project following libraries are needed
*   Boost
*   a MPI implementation

Building is done via cmake.
So after cloning one could for example
```
mkdir build
cd build
cmake ..
make
make test
```

Ilux150/HTW Linux Hosts:
CMake automatically finds boost and mpi, even if installed in `$HOME/include` and `$HOME/lib`, 
and MPI also in the case for example mpich in `$HOME/mpich-install. So building should be as easy as above.

Should mpi not be found one has to set the 2 CMake cache variables MPI_C_LIBRARIES and MPI_C_INCLUDE_PATH for C 
and MPI_CXX_LIBRARIES and MPI_CXX_INCLUDE_PATH for C++. 
This could be done by
```
cmake -D"MPI_C_LIBRARIES=$HOME/mpich-install/lib64/libmpi.so;$HOME/mpich-install/lib64/libpmpi.so" -D"MPI_C_INCLUDE_PATH=$HOME/mpich-install/include" -D"MPI_CXX_LIBRARIES=$HOME/mpich-install/lib64/libmpicxx.so;$HOME/mpich-install/lib64/libmpi.so;$HOME/mpich-install/lib64/libpmpi.so"
```