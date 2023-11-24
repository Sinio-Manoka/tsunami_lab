
Large Data Input and Output
===========================

netCDF Output
-------------

Install netCDF
..............

1. Please install netCDF from `here`_:

.. _here: https://www.unidata.ucar.edu/software/netcdf/

or

If you're working with the Unix subsystem, execute the provided commands to install netCDF.

.. code-block::

  sudo add-apt-repository universe

  sudo apt-get update

  sudo apt-get install zlib1g-dev libhdf5-dev libnetcdf-dev


now lets change our SCibstruct:

.. code-block:: python

  netcdf_lib = env.Append(LIBS=['netcdf'])

  zlib_lib = env.Append(LIBS=['z'])

  hdf5_lib = env.Append(LIBS=['hdf5_serial'])

  if 'LD_LIBRARY_PATH' not in env['ENV']:
      env['ENV']['LD_LIBRARY_PATH'] = ''

  env['ENV']['LD_LIBRARY_PATH'] = '/usr/lib/x86_64-linux-gnu' + env['ENV']['LD_LIBRARY_PATH']


Implementation of a class NetCdf
.................................



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
  #include <netcdf.h>

  namespace tsunami_lab {
  namespace io {
  class NetCdf;
    }
  }

  class tsunami_lab::io::NetCdf {
  private:
  int m_varIdX, m_varIdY,  m_varIdHeight, m_varIdTime, m_varIdBathymetry, m_varIdImpolseX, m_varIdImpolseY;
  t_idx m_time_step = 0;

  static void checkNcErr( int i_err);

  public:
  void updateFile(t_idx                i_nx,
                  t_idx                i_ny,
                  t_idx                i_stride,
                  t_real               i_time,
                  t_real       const * i_h,
                  t_real       const * i_hu,
                  t_real       const * i_hv);
                  
  void generateFile(t_real l_nx,t_real l_ny);

  void fillConstants(t_idx                   i_nx,  
                    t_idx                   i_ny,
                    t_real                  i_dx,
                    t_real                  i_dy,
                    t_real                  i_domainstart_x,
                    t_real                  i_domainstart_y,
                    t_real                  i_stride,
                    t_real          const * i_b);

                              

  };

  #endif


Within the document, three functions will be present: ``fillConstants``, ``generateFile``, and ``updateFile``.

1. ``generateFile``:

This function is responsible for creating the NetCDF output file.

2. ``fillConstants``:

The purpose of this function is to write the constants into the NetCDF output file.

3. ``updateFile``:

This function is designed to update the output file after each time step.




2.  After incorporating the header file, we can proceed to develop the write function in the ``NetCdf.cpp`` file.

.. code-block:: cpp 

    void tsunami_lab::io::NetCdf::fillConstants(t_idx                   i_nx,
                                              t_idx                   i_ny,
                                              t_real                  i_dx,
                                              t_real                  i_dy,
                                              t_real                  i_domainstart_x,
                                              t_real                  i_domainstart_y,
                                              t_real                  i_stride,
                                              t_real          const * i_b){

  int l_ncId,l_err;
  l_err = nc_open("output.nc",
                  NC_WRITE,
                  &l_ncId);

  std::vector<tsunami_lab::t_real> l_coordinatX(i_nx);
  std::vector<tsunami_lab::t_real> l_coordinatY(i_ny);
  std::vector<t_real> l_temp_data_bathymetry(i_ny *i_nx);
  std::vector<t_real> l_temp_data_z(i_ny *i_nx);

  std::vector<size_t> startp = {0};
  std::vector<size_t> endpX = {i_nx};
  std::vector<size_t> endpY = {i_ny};
  std::vector<ptrdiff_t> stridep = {1};

  for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
          for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ) {
              l_coordinatX[l_ix-1] = ((l_ix-1 + 0.5) * i_dx )+ i_domainstart_x;
              l_coordinatY[l_iy-1] = ((l_iy-1 + 0.5) * i_dy )+ i_domainstart_y;
              t_idx l_id = l_iy * i_stride + l_ix;
              l_temp_data_bathymetry[(l_iy-1) * i_ny + (l_ix-1)] = i_b[l_id];
              l_temp_data_z[(l_iy-1) * i_ny + (l_ix-1)] = i_b[l_id];
          }
      }

      l_err = nc_put_vars_float(l_ncId, m_varIdX, startp.data(), endpX.data(), stridep.data(), &l_coordinatX[0]);
      checkNcErr(l_err);
      l_err = nc_put_vars_float(l_ncId, m_varIdY, startp.data(), endpY.data(), stridep.data(), &l_coordinatY[0]);
      checkNcErr(l_err);
      std::vector<size_t> l_start_bathymetry     = {0,0};
      std::vector<size_t> l_end_bathymetry       = {i_ny,i_nx};
      std::vector<ptrdiff_t> l_stridep           = {1,1}; 
      l_err = nc_put_vars_float(l_ncId, m_varIdBathymetry, l_start_bathymetry.data(), l_end_bathymetry.data(), l_stridep.data(), l_temp_data_bathymetry.data());
      checkNcErr(l_err);
      l_err = nc_close(l_ncId);
      checkNcErr(l_err);

  }

  void tsunami_lab::io::NetCdf::updateFile(t_idx                i_nx,
                                          t_idx                i_ny,
                                          t_idx                i_stride,
                                          t_real               i_time,
                                          t_real       const * i_h,
                                          t_real       const * i_hu,
                                          t_real       const * i_hv){
      
      int l_ncId, l_err;    
      l_err = nc_open("output.nc",
                        NC_WRITE,    
                        &l_ncId); 

      checkNcErr(l_err);
      std::vector<t_real> l_temp_data_height(i_ny * i_nx);
      std::vector<t_real> l_temp_data_momentum_x(i_ny * i_nx);
      std::vector<t_real> l_temp_data_momentum_y(i_ny * i_nx);
      for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
        for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ) {
          t_idx l_id = l_iy * i_stride + l_ix;
          l_temp_data_height[(l_iy-1) * i_ny + (l_ix-1)] = i_h[l_id];
          l_temp_data_momentum_x[(l_iy-1) * i_ny + (l_ix-1)] = i_hu[l_id];
          l_temp_data_momentum_y[(l_iy-1) * i_ny + (l_ix-1)] = i_hv[l_id];
        }
      }
      
      std::vector<size_t> l_startp     = {m_time_step,0,0};
      std::vector<size_t> l_endp       = {1,i_ny,i_nx};
      std::vector<ptrdiff_t> l_stridep = {1,1,1}; // Stride
      
      l_err = nc_put_vars_float(l_ncId, m_varIdHeight, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_height.data());
      checkNcErr(l_err);
          
      l_err = nc_put_vars_float(l_ncId, m_varIdImpolseX, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_momentum_x.data());
      checkNcErr(l_err);
          
      l_err = nc_put_vars_float(l_ncId, m_varIdImpolseY, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_momentum_y.data());
      checkNcErr(l_err);

      l_err = nc_put_var1_float(l_ncId, m_varIdTime, &m_time_step, &i_time);
      checkNcErr(l_err);

      l_err = nc_close(l_ncId);
      checkNcErr(l_err);
      m_time_step ++;
  }

  void tsunami_lab::io::NetCdf::generateFile(t_real l_nx,t_real l_ny) {
      
      int l_ncId,l_err;
      // Dimensions x, y, time 
      int l_dimXId,l_dimYId,l_dimTimeId;
      int l_dimIds[3];

      std::cout << "generating netcdf-file output.nc " << std::endl;
      l_err = nc_create("output.nc",
                        NC_CLOBBER,    
                        &l_ncId);      
      checkNcErr(l_err);

      l_err = nc_def_dim(l_ncId, "x", l_nx, &l_dimXId);
      checkNcErr(l_err);

      l_err = nc_def_dim(l_ncId, "y", l_ny, &l_dimYId);
      checkNcErr(l_err);

      l_err = nc_def_dim(l_ncId, "time", NC_UNLIMITED, &l_dimTimeId);
      checkNcErr(l_err);

      
      l_err = nc_def_var(l_ncId, "x", NC_FLOAT, 1, &l_dimXId, &m_varIdX);
      checkNcErr(l_err);
      const char* units_attribute_x = "meters";
      nc_put_att_text(l_ncId, m_varIdX, "units", strlen(units_attribute_x), units_attribute_x);
      const char* axis_attribute_x = "X";
      nc_put_att_text(l_ncId, m_varIdX, "axis", strlen(axis_attribute_x), axis_attribute_x);


      l_err = nc_def_var(l_ncId, "y", NC_FLOAT, 1, &l_dimYId, &m_varIdY);
      checkNcErr(l_err);
      const char* units_attribute_y = "meters";
      nc_put_att_text(l_ncId, m_varIdY, "units", strlen(units_attribute_y), units_attribute_y);
      const char* axis_attribute_y = "Y";
      nc_put_att_text(l_ncId, m_varIdY, "axis", strlen(axis_attribute_y), axis_attribute_y);

      l_err = nc_def_var(l_ncId, "time", NC_FLOAT, 1, &l_dimTimeId, &m_varIdTime);
      checkNcErr(l_err);
      const char* units_attribute_time = "seconds";
      nc_put_att_text(l_ncId, m_varIdTime, "units", strlen(units_attribute_time), units_attribute_time);

      l_dimIds[0] =l_dimYId;
      l_dimIds[1] =l_dimXId;

      l_err = nc_def_var(l_ncId, "b", NC_FLOAT, 2, l_dimIds, &m_varIdBathymetry);
      checkNcErr(l_err);
      const char* units_attribute_Bathymetry = "meters";
      nc_put_att_text(l_ncId, m_varIdBathymetry, "units", strlen(units_attribute_Bathymetry), units_attribute_Bathymetry);
      
      l_dimIds[0] =l_dimTimeId;
      l_dimIds[1] =l_dimYId;
      l_dimIds[2] =l_dimXId;

      l_err = nc_def_var(l_ncId, "h", NC_FLOAT, 3, l_dimIds, &m_varIdHeight);
      checkNcErr(l_err);
      const char* units_attribute_height = "meters";
      nc_put_att_text(l_ncId, m_varIdHeight, "units", strlen(units_attribute_height), units_attribute_height);

      l_err = nc_def_var(l_ncId, "hu", NC_FLOAT, 3, l_dimIds, &m_varIdImpolseX);
      checkNcErr(l_err);
      const char* units_attribute_impulseX = "meters";
      nc_put_att_text(l_ncId, m_varIdImpolseX, "units", strlen(units_attribute_impulseX), units_attribute_impulseX);

      l_err = nc_def_var(l_ncId, "hv", NC_FLOAT, 3, l_dimIds, &m_varIdImpolseY);
      checkNcErr(l_err);
      const char* units_attribute_impulseY = "meters";
      nc_put_att_text(l_ncId, m_varIdImpolseY, "units", strlen(units_attribute_impulseY), units_attribute_impulseY);


      l_err = nc_enddef( l_ncId ); // ncid
      checkNcErr( l_err );
  }

  void tsunami_lab::io::NetCdf::checkNcErr(int i_err) {
      if (i_err) {
          std::cerr << "Error: " << nc_strerror(i_err) << std::endl;
          exit(2);
      }
  }


3. lastly lets implement a test unit for the  ``NetCdf.test.cpp``.



NetCDF Input
-------------

ArtificialTsunami2d
....................




adding support for reading netCDF files
.........................................

To enable reading from our NetCDF files, we need to implement the following function: 

.. code-block:: cpp

    std::vector<tsunami_lab::t_real> readNetCdfDim(std::string filePath , std::string variableName );

    size_t getsizeOfDimension(std::string filePath , std::string variableName);

    std::vector<tsunami_lab::t_real> readNetCdfbathAndDis(std::string filePath );


The function `readNetCdfDim` is designed to retrieve the data of dimensions in a NetCDF file for a specific dimension name.

The function `getsizeOfDimension` is intended to provide the size of a specified dimension.

The function `readNetCdfbathAndDis` is crafted to return the dependent variable associated with a particular dimension.

now lests implement the following fucntion in the ``NetCdf.cpp`` the fowllowing function:


.. code-block:: cpp

    size_t tsunami_lab::io::NetCdf::getsizeOfDimension(std::string filePath , std::string variableName){
      int l_ncId,l_err , l_dimId;
      size_t l_dim_length;
      const char* file_path = filePath.c_str();
      const char* variable_name = variableName.c_str();

          l_err = nc_open(file_path,
                        NC_NOWRITE,
                        &l_ncId);

          checkNcErr(l_err);
          l_err = nc_inq_dimid(l_ncId, variable_name, &l_dimId);
          checkNcErr(l_err);

          l_err = nc_inq_dimlen(l_ncId, l_dimId ,&l_dim_length);
          checkNcErr(l_err);

      return l_dim_length;
  }


  std::vector<tsunami_lab::t_real> tsunami_lab::io::NetCdf::readNetCdfDim( std::string filePath , std::string variableName ){

      int l_ncId,l_err, varid ;

      const char* file_path = filePath.c_str();
      const char* variable_name = variableName.c_str();

      std::vector<tsunami_lab::t_real> data(getsizeOfDimension(filePath,variableName));
      
      l_err = nc_open(file_path,
                        NC_NOWRITE,    
                        &l_ncId);
      checkNcErr(l_err);

      l_err = nc_inq_varid(l_ncId, variable_name , &varid);
      checkNcErr(l_err);

      l_err = nc_get_var_float(l_ncId, varid , &data[0]);
      checkNcErr(l_err);

      l_err = nc_close(l_ncId);


      return data;
      
  }

  std::vector<tsunami_lab::t_real>  tsunami_lab::io::NetCdf::readNetCdfbathAndDis(std::string filePath){

      int l_ncId,l_err, varid ;

      const char* file_path = filePath.c_str();

      std::vector<tsunami_lab::t_real> data(getsizeOfDimension(filePath,"x") * getsizeOfDimension(filePath,"y"));
      
      l_err = nc_open(file_path,
                        NC_NOWRITE,    
                        &l_ncId);

      checkNcErr(l_err);

      l_err = nc_inq_varid(l_ncId, "z" , &varid);
      checkNcErr(l_err);

      
      l_err = nc_get_var_float(l_ncId, varid , &data[0]);
      checkNcErr(l_err);
        
      l_err = nc_close(l_ncId);

      return data;
      

  }







Integration of the new class TsunamiEvent2d
...........................................

Now, let's proceed to implement the following files ``TsunamiEvent2d.cpp`` , ``TsunamiEvent2d.h`` and ``TsunamiEvent2d.test.cpp`` utilizing the provided data.

.. math::

   \begin{split}
       h  &= \begin{cases}
               \max( -b_\text{in}, \delta), &\text{if } b_\text{in} < 0 \\
               0, &\text{else}
             \end{cases}\\
       hu &= 0\\
       hv &= 0\\
       b  &= \begin{cases}
               \min(b_\text{in}, -\delta) + d, & \text{ if } b_\text{in} < 0\\
               \max(b_\text{in}, \delta) + d, & \text{ else}.
             \end{cases}
   \end{split}


and 


.. math:: \text{d}: [-500, +500]\times [-500, +500] \rightarrow \mathbb{R}

.. math::
   :label: eq:artificial

   \begin{aligned}
       \text{d}(x, y) & = & 5 \cdot f(x)g(y) \\
       \text{f}(x) & = & \sin\left(\left(\frac{x}{500}+1\right) \cdot \pi\right) \\
       \text{g}(y) & = & -\left(\frac{y}{500}\right)^2 + 1
     \end{aligned}


1. Let's proceed with the implementation of the initial document ``TsunamiEvent2d.h``:

.. code-block:: cpp


  #ifndef TSUNAMI_LAB_SETUPS_TSUNAMIEVENT2D_H
  #define TSUNAMI_LAB_SETUPS_TSUNAMIEVENT2D_H

  #include "../Setup.h"
  #include <fstream>
  #include <sstream>
  #include <vector>


  namespace tsunami_lab {
    namespace setups {
      class TsunamiEvent2d;
    }
  }

 
  class tsunami_lab::setups::TsunamiEvent2d: public Setup {

    private:
     
      t_real m_delta = 0; 

      
      std::vector<t_real> m_bathymetry_values;
      
     
      t_real displacement( t_real i_x) const;

    public:
    
     

      TsunamiEvent1d( t_real i_delta);

    
      t_real getHeight( t_real i_x,
                        t_real i_y     ) const;

     
      t_real getMomentumX( t_real i_x ,
                          t_real i_y) const;

    
      t_real getMomentumY( t_real,
                          t_real ) const;
      
      
      t_real getBathymetry( t_real i_x,
                            t_real i_y) const ;

      
      t_real getBathymetryCdf(t_real i_x
                              t_real i_y) const;
  };

  #endif




2. Now, we will proceed with the implementation of the  ``TsunamiEvent2d.cpp`` file :

.. code-block:: cpp

  /**
  
  *
  * @section DESCRIPTION
  * The Shock Shock problem.
  **/
  #include "TsunamiEvent2d.h"
  #include "../../io/Csv/Csv.h"
  #include <cmath>
  #include <cstddef> 


  tsunami_lab::setups::TsunamiEvent2d::TsunamiEvent2d(t_real i_delta){

      m_delta = i_delta;

      const std::string filename = "data/data_end.csv";
      std::size_t columnIndex = 3;
      m_bathymetry_values = tsunami_lab::io::Csv::read(filename,columnIndex); 
      
  }

  tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getBathymetry( t_real i_x,
                                                                          t_real i_y) const {
      t_real l_bin = getBathymetryCsv(i_x);
      if(l_bin < 0 ){    
          if( l_bin < -m_delta){
              return l_bin    + displacement(i_x);  
          }else{
              return -m_delta + displacement(i_x);
          }
      }else{
        if( l_bin > m_delta){
              return l_bin    + displacement(i_x);
          }else{
              return m_delta + displacement(i_x);
          }
      } 
  }

  tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getHeight( t_real i_x,
                                                                      t_real i_y     )const{
      t_real l_bin = getBathymetryCsv(i_x);                           
      if(l_bin < 0 ){
          if( -l_bin < m_delta){
              return m_delta;
          }else{
              return -l_bin;
          }
      }else{
          return 0;
      }                                                                                                                                                                                     
  }

  tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::displacement( t_real i_x,t_real i_y) const{

      if((i_x > -500) && (i_y > -500) && (i_x < 500) && (i_y < 500) ){
          return 5 * fFunction(i_x) * gFunction(i_y);
      }else{
          return 0;
      }
      

  }

  tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getBathymetrynetCdf(t_real i_x) const{
      //i_x gets divided by 250 because every cell is in 250m steps
      std::size_t l_index = i_x /250; 
      return m_bathymetry_values[l_index];
  }

  tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::fFunction(t_real i_x) const{
      return sin(((i_x/500)+1)* M_PI );
  }

  tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::gFunction(t_real i_y) const{
      return -((i_y/500)*(i_y/500))+1;
  }

  tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumY(  t_real i_x,
                                                                          t_real i_y)const{
      return 0;                                                                                                                                                                      
  }



  tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumX(  t_real i_x,
                                                                          t_real i_y)const{
      return 0;                                                                                                                       
  }



3. Finally, let's implement the test unit for the TsunamiEvent2d in the ``TsunamiEvent2d.test.cpp`` file:

    ---to_do---



Personal Contribution
---------------------

- Ward Tammaa, Daniel Schicker Doxygen Documentation
- Mohamad Khaled Minawe, Ward Tammaa, Daniel Schicker Sphnix Documentation
- Daniel Schicker, Mohamad Khaled Minawe , Ward Tammaa functions implementation
- Mohamad Khaled Minawe, Daniel Schicker, Ward Tammaa Unit Testing
- Mohamad Khaled Minawe, Daniel Schicker Geogebra Datei(Calculations for the Unit Tests)
- Ward Tammaa Hosting the code , Action runner


    