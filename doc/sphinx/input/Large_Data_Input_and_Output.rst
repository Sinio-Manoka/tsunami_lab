
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
    /**
    * @param i_err 
    *
    */
        static void checkNcErr( int i_err);

    public:
    
        void updateFile(t_idx                i_nx,
                        t_idx                i_ny,
                        t_idx                i_stride,
                        t_real               i_time,
                        t_real       const * i_h,
                        t_real       const * i_hu,
                        t_real       const * i_hv);
                        
        NetCdf(t_real l_nx,t_real l_ny);

        void fillConstants(t_idx                 i_nx,  
                        t_idx                   i_ny,
                        t_real                  i_dx,
                        t_real                  i_dy,
                        t_real                  i_domainstart_x,
                        t_real                  i_domainstart_y,
                        t_real                  i_stride,
                        t_real          const * i_b);

                                    
    };

    #endif


In the document, you'll find two functions: ``fillConstants``, ``updateFile``, and a constructor named ``netCDF``.

    1. ``netCDF``:

        This constructer is responsible for creating the NetCDF output file.

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
        l_err = nc_open("output.nc",NC_WRITE, &l_ncId);

        t_real *l_coordinateX = new t_real[i_nx];
        t_real *l_coordinateY = new t_real[i_ny];
        t_real *l_temp_data_bathymetry = new t_real[(i_ny * i_nx)];
        
        for( t_idx l_iy = 0; l_iy < i_ny; l_iy++ )
        {
            l_coordinateY[l_iy] = ((l_iy + 0.5) * i_dy )+ i_domainstart_y;
        }
        // put y coordinates
        l_err = nc_put_var_float(l_ncId, m_varIdY, l_coordinateY);
        checkNcErr(l_err);

        delete[] l_coordinateY;
        for(t_idx l_ix = 0; l_ix < i_nx; l_ix++) 
        {
            l_coordinateX[l_ix] = ((l_ix + 0.5) * i_dx )+ i_domainstart_x;
        }
        // put x coordinates
        l_err = nc_put_var_float(l_ncId, m_varIdX, l_coordinateX);
        checkNcErr(l_err);
        delete[] l_coordinateX;

        for( t_idx l_iy = 0; l_iy < i_ny; l_iy++ )
        {
            for( t_idx l_ix = 0; l_ix < i_nx; l_ix++)
            {   
                t_idx l_id = (l_iy) * i_stride + (l_ix+1);

                l_temp_data_bathymetry[(l_iy * i_nx) + l_ix] = i_b[l_id];
            }
        }
        
        // put bathymetry values
        l_err = nc_put_var_float(l_ncId, m_varIdBathymetry, l_temp_data_bathymetry);
        checkNcErr(l_err);

        delete[] l_temp_data_bathymetry;

        //close file for now
        l_err = nc_close(l_ncId);
        checkNcErr(l_err);

    }


    void tsunami_lab::io::NetCdf::updateFile(t_idx               i_nx,
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
            l_temp_data_height[(l_iy-1) * i_nx + (l_ix-1)] = i_h[l_id];
            l_temp_data_momentum_x[(l_iy-1) * i_nx + (l_ix-1)] = i_hu[l_id];
            l_temp_data_momentum_y[(l_iy-1) * i_nx + (l_ix-1)] = i_hv[l_id];
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

    
    tsunami_lab::io::NetCdf::NetCdf(t_real l_nx,t_real l_ny) {
        
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



.. code-block:: cpp 

    #include <catch2/catch.hpp>
    #include "../../constants.h"
    #include <sstream>
    #include <vector>
    #define private public
    #include "NetCdf.h"
    #include <filesystem>
    #undef public

    TEST_CASE( "Test the NetCdf-writer", "[NetCdfWrite]" ) {

        tsunami_lab::t_real l_h[10]  = { 0, 1, 2, 3, 4, 5, 6 ,7, 8 ,5};
        tsunami_lab::t_real l_hu[10] = { 6, 5, 4, 3, 2, 1, 0 ,7, 8 ,5};
        tsunami_lab::t_real l_hv[10] = { 6, 5, 4, 3, 2, 1, 0,7, 8 ,5 };
        tsunami_lab::t_real l_b[10]  = { 0, 0, 0, 0, 0, 0, 0 ,7, 8 ,5};

        tsunami_lab::io::NetCdf* l_netCdf = new tsunami_lab::io::NetCdf(5,2);
        l_netCdf->fillConstants(5,2,0.5,1,-50,-50,0,l_b);
        l_netCdf->updateFile(5,2,1,1,l_h,l_hu,l_hv);
        int l_err;
        int l_ncidp;
        int  l_dimXId,l_dimYId,l_TimeId;
        size_t l_nx,l_ny , l_nt ;
        int l_vHId, l_vXId,l_vHuId, l_vYId, l_vTimeId, l_vHvId ,l_vBId ;

        REQUIRE(std::filesystem::exists("output.nc"));

        l_err = nc_open("output.nc", NC_NOWRITE, &l_ncidp);
        l_netCdf->checkNcErr(l_err);

        l_err = nc_inq_dimid(l_ncidp,"x",&l_dimXId);
        l_netCdf->checkNcErr(l_err);

        l_err = nc_inq_dimid(l_ncidp,"y",&l_dimYId);
        l_netCdf->checkNcErr(l_err);

        l_err= nc_inq_dimid(l_ncidp, "time", &l_TimeId);
        l_netCdf->checkNcErr(l_err);

        l_err = nc_inq_dimlen(l_ncidp, l_dimXId, &l_nx);
        l_netCdf->checkNcErr(l_err);

        l_err = nc_inq_dimlen(l_ncidp, l_dimYId, &l_ny);
        l_netCdf->checkNcErr(l_err);

        l_err = nc_inq_dimlen(l_ncidp, l_TimeId, &l_nt);
        l_netCdf->checkNcErr(l_err);

        REQUIRE(l_nx == 5);
        REQUIRE(l_ny == 2);
        REQUIRE(l_nt == 1);


        l_err = nc_inq_varid(l_ncidp, "x", &l_vXId);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_inq_varid(l_ncidp, "y", &l_vYId);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_inq_varid(l_ncidp, "time", &l_vTimeId);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_inq_varid(l_ncidp, "h", &l_vHId);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_inq_varid(l_ncidp, "hu", &l_vHuId);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_inq_varid(l_ncidp, "hv", &l_vHvId);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_inq_varid(l_ncidp, "b", &l_vBId);
        l_netCdf->checkNcErr(l_err);


        std::vector<tsunami_lab::t_real> x(10);
        std::vector<tsunami_lab::t_real> y(2);
        std::vector<tsunami_lab::t_real> time(1);
        std::vector<tsunami_lab::t_real> height(10);
        std::vector<tsunami_lab::t_real> momentum_x(10);
        std::vector<tsunami_lab::t_real> momentum_y(10);
        std::vector<tsunami_lab::t_real> bathymetry(10);
        
        
        l_err = nc_get_var_float(l_ncidp, l_vXId, &x[0]);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_get_var_float(l_ncidp, l_vYId, &y[0]);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_get_var_float(l_ncidp, l_vTimeId, &time[0]);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_get_var_float(l_ncidp, l_vHId, &height[0]);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_get_var_float(l_ncidp, l_vHuId, &momentum_x[0]);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_get_var_float(l_ncidp, l_vHvId, &momentum_y[0]);
        l_netCdf->checkNcErr(l_err);
        l_err = nc_get_var_float(l_ncidp, l_vBId, &bathymetry[0]);
        l_netCdf->checkNcErr(l_err);

        REQUIRE(x[0] == Approx(-49.75));
        REQUIRE(x[1] == Approx(-49.25));
        REQUIRE(x[2] == Approx(-48.75));
        REQUIRE(x[3] == Approx(-48.25));
        REQUIRE(x[4] == Approx(-47.75));
        REQUIRE(y[0] == Approx(-49.5));

        REQUIRE(time[0] == Approx(1));

        REQUIRE(momentum_x[0] == Approx(4));
        REQUIRE(momentum_x[1] == Approx(3));
        REQUIRE(momentum_x[2] == Approx(2));
        REQUIRE(momentum_x[3] == Approx(1));
        REQUIRE(momentum_x[4] == Approx(0));
        REQUIRE(momentum_x[5] == Approx(3));
        REQUIRE(momentum_x[6] == Approx(2));
        REQUIRE(momentum_x[7] == Approx(1));
        REQUIRE(momentum_x[8] == Approx(0));
        REQUIRE(momentum_x[9] == Approx(7));

        REQUIRE(momentum_y[0] == Approx(4));
        REQUIRE(momentum_y[1] == Approx(3));
        REQUIRE(momentum_y[2] == Approx(2));
        REQUIRE(momentum_y[3] == Approx(1));
        REQUIRE(momentum_y[4] == Approx(0));
        REQUIRE(momentum_y[5] == Approx(3));
        REQUIRE(momentum_y[6] == Approx(2));
        REQUIRE(momentum_y[7] == Approx(1));
        REQUIRE(momentum_y[8] == Approx(0));
        REQUIRE(momentum_y[9] == Approx(7));

    for (size_t i = 0; i < 10; i++)
        {
            REQUIRE(bathymetry[i] == Approx(0));
        }
        
        
    }


NetCDF Input
-------------

Implementation and test for the artificial tsunami setup
.........................................................

Now, let's proceed to implement the following files ``ArtificialTsunami2d.cpp`` , ``ArtificialTsunami2d.h`` and ``ArtificialTsunami2d.test.cpp`` utilizing the provided data.

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


1. Let's proceed with the implementation of the initial document ``ArtificialTsunami2d.h``:

.. code-block:: cpp



    #ifndef TSUNAMI_LAB_SETUPS_ARTIFICIALTSUNAMI2D_H
    #define TSUNAMI_LAB_SETUPS_ARTIFICIALTSUNAMI2D_H

    #include "../Setup.h"
    #include <fstream>
    #include <sstream>
    #include <vector>


    namespace tsunami_lab {
    namespace setups {
        class ArtificialTsunami2d;
    }
    }

    /**
    * ArtificialTsunami2d setup.
    **/
    class tsunami_lab::setups::ArtificialTsunami2d: public Setup {

    private:
        /**
        * @param m_delta avoids running into numerical issues due to missing support for wetting and drying in our solver.
        */
        t_real m_delta = 0;
        /**
        * @param m_Bin The swimming pool has a constant bathymetry of -100m before the “earthquake” 
        */
        t_real m_Bin = -100;
        
        /**
        * @brief the method adds the vertical displacement, typically caused by a subduction-zone earthquake.
        * @param i_x is the distance from the Fukushima Daini Nuclear Power Plant.
        */
        t_real displacement( t_real i_x ,t_real i_y) const;

        t_real gFunction(t_real i_y) const;

        t_real fFunction(t_real i_x) const;

    public:
    
        /**
        * @brief The constructor for ArtificialTsunami2d.
        * @param i_delta to avoid running into numerical issues (small value)
        **/

        ArtificialTsunami2d( t_real i_delta);

        /**
        * @brief is an arificial simulation so no inputs needed
        * @return height is hardcoded to 100;
        **/
        t_real getHeight( t_real ,
                        t_real ) const;

        /**
        * @brief Gets the momentum in x-direction.
        * @return Momentum in x-direction.
        **/
        t_real getMomentumX( t_real,
                            t_real) const;

        /**
        * @brief Gets the momentum in y-direction.
        * @return Momentum in y-direction.
        **/
        t_real getMomentumY( t_real,
                            t_real ) const;
        
        /**
        * @brief Computes the bathymetry explained in <a href="https://scalable.uni-jena.de/opt/tsunami/chapters/assignment_5.html#equation-eq-artificial">5.2.1</a>.
        * @param i_x position in the swimming pool.
        * @param i_x position in the swimming pool.
        * @return artificial computed Bathymetry.
        */
        t_real getBathymetry( t_real i_x,
                            t_real i_y) const ;

    
    };

    #endif



2. Now, we will proceed with the implementation of the  ``ArtificialTsunami2d.cpp`` file :

.. code-block:: cpp

    #include "ArtificialTsunami2d.h"
    #include <cmath>
    #include <cstddef> 


    tsunami_lab::setups::ArtificialTsunami2d::ArtificialTsunami2d(t_real i_delta){

        m_delta = i_delta;

    }

    tsunami_lab::t_real tsunami_lab::setups::ArtificialTsunami2d::getBathymetry( t_real i_x,
                                                                                t_real i_y) const {


        //m_Bin is hardcoded to -100                                                                     
        
        if(m_Bin < 0 ){    
            if( m_Bin < -m_delta){
                return m_Bin + displacement(i_x,i_y);  
            }else{
                return -m_delta + displacement(i_x,i_y);
            }
        }else{
        if( m_Bin > m_delta){
                return m_Bin    + displacement(i_x,i_y);
            }else{
                return m_delta + displacement(i_x,i_y);
            }
        }
    }

    tsunami_lab::t_real tsunami_lab::setups::ArtificialTsunami2d::getHeight( t_real, 
                                                                        t_real )const{
        //-100 m bathymetry and water level is at 0 -> 100m height
        return 100;                                                                                                                                                                                   
    }

    tsunami_lab::t_real tsunami_lab::setups::ArtificialTsunami2d::displacement( t_real i_x,t_real i_y) const{

        if((i_x > -500) && (i_y > -500) && (i_x < 500) && (i_y < 500) ){
            return 5 * fFunction(i_x) * gFunction(i_y);
        }else{
            return 0;
        }
    }

    tsunami_lab::t_real tsunami_lab::setups::ArtificialTsunami2d::fFunction(t_real i_x) const{
        return sin(((i_x/500)+1)* M_PI );
    }

    tsunami_lab::t_real tsunami_lab::setups::ArtificialTsunami2d::gFunction(t_real i_y) const{
        return -((i_y/500)*(i_y/500))+1;
    }

    tsunami_lab::t_real tsunami_lab::setups::ArtificialTsunami2d::getMomentumY(  t_real ,
                                                                            t_real )const{
        return 0;
    }



    tsunami_lab::t_real tsunami_lab::setups::ArtificialTsunami2d::getMomentumX(  t_real,
                                                                            t_real )const{
        return 0;
    }



3. Finally, let's implement the test unit for the ArtificialTsunami2d in the ``ArtificialTsunami2d.test.cpp`` file:

.. code-block:: cpp


    #include <catch2/catch.hpp>
    #include "ArtificialTsunami2d.h"

    TEST_CASE( "Test the ArtificialTsunami2d setup.", "[ArtificialTsunami2d]" ) {
    tsunami_lab::setups::ArtificialTsunami2d l_ArtificialTsunami2d(20);

    // x = 500 ,y = 500  not within the if condition
    REQUIRE( l_ArtificialTsunami2d.getHeight( 500, 500 ) == 100.0f);
    //  (500 < 500) && (500 < 500) == false -> return 0 -> -100 + 0
    REQUIRE( l_ArtificialTsunami2d.getBathymetry( 500, 500 ) == -100.0f );
    
    REQUIRE( l_ArtificialTsunami2d.getMomentumX( 500, 500 ) == 0);

    REQUIRE( l_ArtificialTsunami2d.getMomentumY( 500, 500 ) == 0 );


    //x = 20 , y = 600 
    REQUIRE( l_ArtificialTsunami2d.getHeight( 20, 600) == 100.0f );
    //  (600 < 500) == false -> return 0 -> -100 + 0
    REQUIRE( l_ArtificialTsunami2d.getBathymetry( 20, 600 ) ==-100 );
    
    REQUIRE( l_ArtificialTsunami2d.getMomentumX( 20, 600 ) == 0);

    REQUIRE( l_ArtificialTsunami2d.getMomentumY( 20, 600 ) == 0 );


    //within the boundary [-500,500]x[-500,500]->R
    //x = 300 , y = 300

    /*(300 > -500) && (300 > -500) && (300 < 500) && (300 < 500) -> true
        =  m_Bin + displacement(i_x,i_y)
        
        = -100 + 5 * fFunction(i_x)             *         gFunction(i_y);
        
        = -100 + 5 * sin(((300/500)+1)* M_PI )   * (-((300/500)*(300/500))+1) 

        ca ~ 100 + -3.04338

        = -103.04338f
    */
    REQUIRE( l_ArtificialTsunami2d.getHeight( 300, 300 ) == 100.0f);
    
    REQUIRE( l_ArtificialTsunami2d.getBathymetry( 300, 300 ) == -103.04338f );
    
    REQUIRE( l_ArtificialTsunami2d.getMomentumX( 300, 300 ) == 0);

    REQUIRE( l_ArtificialTsunami2d.getMomentumY( 300, 300 ) == 0 );
    /*
        x = 10 , y = 100;
        -> both values in the domain ->

        /*(10 > -500) && (10 > -500) && (100 < 500) && (100 < 500) -> true
        =  m_Bin + displacement(i_x,i_y)
        
        = -100 + 5 * fFunction(i_x)             *         gFunction(i_y);
        
        = -100 + 5 * sin(((10/500)+1)* M_PI )   * (-((100/500)*(100/500))+1) 

        ca ~ -100 + -0.30139449

        = -100.30139449f
    */
    REQUIRE( l_ArtificialTsunami2d.getHeight( 10, 100 ) == 100.0f );

    REQUIRE( l_ArtificialTsunami2d.getBathymetry( 10, 100 ) == -100.30139449f );
    
    REQUIRE( l_ArtificialTsunami2d.getMomentumX( 10, 100 ) == 0);

    REQUIRE( l_ArtificialTsunami2d.getMomentumY( 10, 100 ) == 0 );  
    } 


adding support for reading netCDF files
.........................................

To enable reading from our NetCDF files, we need to implement the read function:

To implement the function, we'll utilize the filename, variablename (which may represent a dimension or a variable), 
and a vector to store the data.
The function is expected to return an integer indicating the state.
If the function returns -1, it should result in the program's exit.

now lests implement the following fucntion in the ``NetCdf.cpp`` the fowllowing function:


.. code-block:: cpp

  int tsunami_lab::io::NetCdf::read(  const char* filename,
                                    const char* varname,
                                    std::vector<t_real>  & data){
    int l_ncId; 
    if (nc_open(filename, NC_NOWRITE, &l_ncId) != NC_NOERR) {
        std::cerr << "Error opening NetCDF file: " << filename << std::endl;
        return -1;
    }

    int varid; 

    if (nc_inq_varid(l_ncId, varname, &varid) != NC_NOERR) {
        std::cerr << "Error getting variable ID for variable: " << varname << std::endl;
        nc_close(l_ncId);
        return -1; 
    }

    int ndims; 
    int dimids[NC_MAX_VAR_DIMS]; 
    if (nc_inq_var(l_ncId, varid, nullptr, nullptr, &ndims, dimids, nullptr) != NC_NOERR) {
        std::cerr << "Error getting variable information for variable: " << varname << std::endl;
        nc_close(l_ncId); 
        return -1;
    }
    t_idx dataSize = 1;
    for (int i = 0; i < ndims; ++i) {
        char dimname[NC_MAX_NAME + 1];
        size_t dimlen;
        if (nc_inq_dim(l_ncId, dimids[i], dimname, &dimlen) != NC_NOERR) {
            std::cerr << "Error getting dimension information." << std::endl;
            nc_close(l_ncId); 
        }
        dataSize *= dimlen;
    }
    data.resize(dataSize);
    int l_err = nc_get_var_float(l_ncId, varid , &data[0]);
    checkNcErr(l_err);

    if (nc_close(l_ncId) != NC_NOERR) {
        std::cerr << "Error closing NetCDF file: " << filename << std::endl;
    }
    return -1;
    }


now we have to add the function to the header file ``NetCdf.h``:

.. code-block:: cpp

    
    int read( const char* filename,
              const char* varname,
              std::vector<t_real> &data);


Let's now create a test unit for the reader function within the``NetCdf.test.cpp`` file:


Now, locate the file we utilized either in our project folder or download it from the provided source `testfile`_

.. _testfile: https://people.sc.fsu.edu/~jburkardt/data/netcdf/simple_xy.nc


.. code-block:: cpp 

    TEST_CASE( "Test the NetCdf-reader ", "[NetCdfreader]" ) {
        std::vector<tsunami_lab::t_real> data;

        tsunami_lab::io::NetCdf* l_netCdf = new tsunami_lab::io::NetCdf(10,10);
    

        l_netCdf->read("testFileForTheReader.nc" , "data", data ) ;
        


    for (tsunami_lab::t_real i = 0; i < 71; i++)
    {
        REQUIRE(i == data[i]);
    }

    }






Integration the new class TsunamiEvent2d
.........................................












Check the correctness 
......................











Personal Contribution
---------------------

- Ward Tammaa, Daniel Schicker Doxygen Documentation
- Mohamad Khaled Minawe, Ward Tammaa, Daniel Schicker Sphnix Documentation
- Daniel Schicker, Mohamad Khaled Minawe , Ward Tammaa functions implementation
- Mohamad Khaled Minawe, Daniel Schicker, Ward Tammaa Unit Testing
- Mohamad Khaled Minawe, Daniel Schicker Geogebra Datei(Calculations for the Unit Tests)
- Ward Tammaa Hosting the code , Action runner


    