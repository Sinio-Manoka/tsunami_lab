
Large Data Input and Output
===========================

netCDF
-------

1. Please install netCDF from `here`_:

.. _here: https://www.unidata.ucar.edu/software/netcdf/

or

If you're working with the Unix subsystem, execute the provided commands to install netCDF.

.. code-block::

  sudo add-apt-repository universe

  sudo apt-get update

  sudo apt-get install zlib1g-dev libhdf5-dev libnetcdf-dev


now lets change our SCibstruct:

.. code-block::

  netcdf_lib = env.Append(LIBS=['netcdf'])

  zlib_lib = env.Append(LIBS=['z'])

  hdf5_lib = env.Append(LIBS=['hdf5_serial'])

  if 'LD_LIBRARY_PATH' not in env['ENV']:
      env['ENV']['LD_LIBRARY_PATH'] = ''

  env['ENV']['LD_LIBRARY_PATH'] = '/usr/lib/x86_64-linux-gnu' + env['ENV']['LD_LIBRARY_PATH']


Now, let's implement a NetCdf class by creating the following files:  : ``NetCdf.cpp`` , ``NetCdf.h`` and ``NetCdf.test.cpp``

1. Let's proceed with the implementation of the header file named``NetCdf.h`` 

.. code-block:: cpp

    /**
  * @author Khaled minawe
  *
  * @section DESCRIPTION
  * IO-routines for writing a Netcdf Files 
  **/
  #ifndef TSUNAMI_LAB_IO_NETCDF
  #define TSUNAMI_LAB_IO_NETCDF

  #include "../../constants.h"
  #include <cstring>
  #include <iostream>
  #include <fstream>
  #include <sstream>
  #include <vector>
  #include <cstddef> 
  #include <netcdf.h>

  namespace tsunami_lab {
    namespace io {
      class NetCdf;
    }
  }

  class tsunami_lab::io::NetCdf {
    public:
      /**
      * @brief Writes the data as CSV to the given stream.
      *
      * @param i_dxy cell width in x- and y-direction.
      * @param i_nx number of cells in x-direction.
      * @param i_ny number of cells in y-direction.
      * @param i_stride stride of the data arrays in y-direction (x is assumed to be stride-1).
      * @param i_domainstart the offset of the domain.
      * @param i_dem_wavepropagation tells us whether we use 1D or 2D.
      * @param i_h water height of the cells; optional: use nullptr if not required.
      * @param i_hu momentum in x-direction of the cells; optional: use nullptr if not required.
      * @param i_hv momentum in y-direction of the cells; optional: use nullptr if not required.
      * @param i_b bathymetry of the cell
      **/
      static void write( t_idx                i_nx,
                        t_idx                i_ny, 
                        t_real       const * i_h,
                        t_real       const * i_hu,
                        t_real       const * i_hv,
                        t_real       const * i_b);
      

  };

  #endif





2.  After incorporating the header file, we can proceed to develop the write function in the ``NetCdf.cpp`` file.

.. code-block:: cpp 

  #include "NetCdf.h"

  void tsunami_lab::io::NetCdf::write(t_idx         i_nx,
                                      t_idx         i_ny,
                                      t_real const *i_h,
                                      t_real const *i_hu,
                                      t_real const *i_hv,
                                      t_real const *i_b) {

      // Specify the NetCDF file name
      std::string filename = "output.nc";

      // Create a new NetCDF file
      int ncid;
      nc_create(filename.c_str(), NC_CLOBBER, &ncid);

      // Define dimensions
      int x_dimid, y_dimid;
      nc_def_dim(ncid, "x", i_nx, &x_dimid);
      nc_def_dim(ncid, "y", i_ny, &y_dimid);

      // Define variables
      int h_varid, hu_varid, hv_varid, b_varid;
      nc_def_var(ncid, "h", NC_FLOAT, 1, &x_dimid, &h_varid);
      nc_def_var(ncid, "hu", NC_FLOAT, 1, &x_dimid, &hu_varid);
      nc_def_var(ncid, "hv", NC_FLOAT, 1, &x_dimid, &hv_varid);
      nc_def_var(ncid, "b", NC_FLOAT, 1, &x_dimid, &b_varid);

      // Write data to variables
      nc_put_var_float(ncid, h_varid, i_h);
      nc_put_var_float(ncid, hu_varid, i_hu);
      nc_put_var_float(ncid, hv_varid, i_hv);
      nc_put_var_float(ncid, b_varid, i_b);

      // Add attributes or other necessary information if needed

      // Close the NetCDF file
      nc_close(ncid);
  }


3. lastly lets implement a test unit for the  ``NetCdf.test.cpp``.

..todo...





Personal Contribution
---------------------

- Ward Tammaa, Daniel Schicker Doxygen Documentation
- Mohamad Khaled Minawe, Ward Tammaa, Daniel Schicker Sphnix Documentation
- Daniel Schicker, Mohamad Khaled Minawe , Ward Tammaa functions implementation
- Mohamad Khaled Minawe, Daniel Schicker, Ward Tammaa Unit Testing
- Mohamad Khaled Minawe, Daniel Schicker Geogebra Datei(Calculations for the Unit Tests)
- Ward Tammaa Hosting the code , Action runner


    