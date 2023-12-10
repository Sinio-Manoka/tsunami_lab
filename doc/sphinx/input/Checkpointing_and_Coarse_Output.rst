

Checkpointing and Coarse Output
======================================

Checkpointing
--------------

To implement the checkpoint feature, we'll enhance our NetCDF writer and modify the `main.cpp` file. Initially, let's create a method responsible for generating the checkpoint. Within this method, we'll include all necessary components for compiling
our program and additionally store station information.

.. code-block:: cpp

    void tsunami_lab::io::NetCdf::createCheckPoint(std::string i_solver,
                                            t_real i_domain_start_x,
                                            t_real i_domain_start_y,
                                            t_real i_dimension_x,
                                            t_real i_dimension_y,
                                            t_real i_endtime,
                                            t_real i_simTime,
                                            t_real i_frequency,
                                            t_real i_dt,
                                            t_real i_simulation_time_for_last_cp,
                                            const t_real *i_b,
                                            const t_real *i_h,
                                            const t_real *i_hu,
                                            const t_real *i_hv,
                                            t_idx i_time_step_index,
                                            t_idx i_stride,
                                            t_idx i_nx,
                                            t_idx i_ny,
                                            std::string i_setup,
                                            std::string i_stations_string,
                                            std::string i_name_cp,
                                            std::string i_disfile,
                                            std::string i_batfile){
    
    std::string folder_path = "outputs/cp";
    std::string l_name_cp = "outputs/cp/"+i_name_cp;
    int l_err,l_ncId;
    l_err = nc_create(l_name_cp.data(), NC_NETCDF4,&l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_dimXId, l_dimYId;
    l_err = nc_def_dim(l_ncId, "x", i_nx, &l_dimXId);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_dim(l_ncId, "y", i_ny, &l_dimYId);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    int l_var_domain_start_x_id, l_var_domain_start_y_id , l_var_solver_id, l_var_dimension_x_id,
        l_var_dimension_y_id,l_var_endtime_id, l_var_simTime_id, l_var_frequency_id, l_var_b_id,
        l_var_h_id,l_var_hu_id,l_var_hv_id, l_var_dt_id , l_var_disfile_id,l_var_batfile_id;    
    int  l_var_time_step_index_id,l_var_stations_string,l_var_setup, l_simulation_time_for_last_cp_id;
    //-----------------------------------------------------Define Variables
    l_err = nc_def_var(l_ncId,"domain_start_x",NC_FLOAT,0 , nullptr, &l_var_domain_start_x_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"domain_start_y",NC_FLOAT,0 , nullptr, &l_var_domain_start_y_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"solver",NC_INT,0 , nullptr, &l_var_solver_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"dimension_x",NC_FLOAT,0 , nullptr, &l_var_dimension_x_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"simulationtimeforlastcp",NC_FLOAT,0 , nullptr, &l_simulation_time_for_last_cp_id);
    checkNcErr(l_err,__FILE__, __LINE__);


    l_err = nc_def_var(l_ncId,"dimension_y",NC_FLOAT,0 , nullptr, &l_var_dimension_y_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"endtime",NC_FLOAT,0 , nullptr, &l_var_endtime_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"simTime",NC_FLOAT,0 , nullptr, &l_var_simTime_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"setup",NC_STRING,0 , nullptr, &l_var_setup);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"stations",NC_STRING,0 , nullptr, &l_var_stations_string);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"timeStep",NC_INT,0 , nullptr, &l_var_time_step_index_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"frequency",NC_FLOAT,0 , nullptr, &l_var_frequency_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"dt",NC_FLOAT,0 , nullptr, &l_var_dt_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"disfile",NC_STRING,0 , nullptr, &l_var_disfile_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"batfile",NC_STRING,0 , nullptr, &l_var_batfile_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    int l_dims[2] = {l_dimYId,l_dimXId};

    l_err = nc_def_var(l_ncId,"b",NC_FLOAT,2 , l_dims, &l_var_b_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"hu",NC_FLOAT,2 , l_dims, &l_var_hu_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"h",NC_FLOAT,2, l_dims, &l_var_h_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"hv",NC_FLOAT,2 , l_dims, &l_var_hv_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_enddef(l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);

    //-----------------------------------------------------Write Variables
    const char* setupData = i_setup.data();
    l_err = nc_put_var_string(l_ncId, l_var_setup, &setupData);
    checkNcErr(l_err,__FILE__, __LINE__);

    const char* l_stations_string = i_stations_string.data();
    const char* l_disfile_string = i_disfile.data();
    const char* l_batfile_string = i_batfile.data();
    
    l_err = nc_put_var_string(l_ncId, l_var_disfile_id, &l_batfile_string);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_string(l_ncId, l_var_batfile_id, &l_disfile_string);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_string(l_ncId, l_var_stations_string, &l_stations_string);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId,l_var_domain_start_x_id,&i_domain_start_x);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId,l_simulation_time_for_last_cp_id,&i_simulation_time_for_last_cp);
    checkNcErr(l_err,__FILE__, __LINE__);


    l_err = nc_put_var_float(l_ncId, l_var_domain_start_y_id, &i_domain_start_y);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_dimension_x_id, &i_dimension_x);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_dimension_y_id, &i_dimension_y);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_endtime_id, &i_endtime);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_simTime_id, &i_simTime);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    l_err = nc_put_var_float(l_ncId, l_var_frequency_id, &i_frequency);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_dt_id, &i_dt);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_put_var_int(l_ncId, l_var_time_step_index_id, (const int*)&i_time_step_index);
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_solver = 0;
    if(i_solver == "roe") l_solver = 1;
    
    l_err = nc_put_var_int(l_ncId, l_var_solver_id, &l_solver);
    checkNcErr(l_err,__FILE__, __LINE__);

    std::vector<size_t> l_start     = {0, 0};
    std::vector<size_t> l_end       = {1,i_nx};
    std::vector<ptrdiff_t> l_strid = {1,1};

    for(l_start[0] = 0; l_start[0] < i_ny; l_start[0]++){
        l_err = nc_put_vars_float(l_ncId, l_var_b_id, l_start.data(), l_end.data(),l_strid.data() , (i_b + (l_start[0]+1) * i_stride+1));
        checkNcErr(l_err,__FILE__, __LINE__);
        l_err = nc_put_vars_float(l_ncId, l_var_h_id, l_start.data(), l_end.data(),l_strid.data() , (i_h + (l_start[0]+1) * i_stride+1));
        checkNcErr(l_err,__FILE__, __LINE__);
        l_err = nc_put_vars_float(l_ncId, l_var_hu_id, l_start.data(), l_end.data(),l_strid.data() , (i_hu + (l_start[0]+1) * i_stride+1));
        checkNcErr(l_err,__FILE__, __LINE__);
        l_err = nc_put_vars_float(l_ncId, l_var_hv_id, l_start.data(), l_end.data(),l_strid.data() , (i_hv + (l_start[0]+1) * i_stride+1));
        checkNcErr(l_err,__FILE__, __LINE__);
    }

    l_err = nc_close(l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);

    }



Once the checkpoint data is saved, we'll need to implement a method for reading the information from the checkpoint file. In this read method, we'll iterate through the contents of the checkpoint file and
retrieve each input that has been previously written.

.. code-block:: cpp 

    oid tsunami_lab::io::NetCdf::readCheckPoint(std::string i_path_cp,
                                            std::string * o_solver,
                                            t_real * o_domain_start_x,
                                            t_real * o_domain_start_y,
                                            t_real * o_dimension_x,
                                            t_real * o_dimension_y,
                                            t_real * o_endtime,
                                            t_real * o_simTime,
                                            t_real * o_frequency,
                                            t_real * o_dt,
                                            t_real *o_simulation_time_for_last_cp,
                                            t_real ** o_b,
                                            t_real ** o_h,
                                            t_real ** o_hu,
                                            t_real ** o_hv,
                                            t_idx * o_time_step_index,
                                            t_idx * o_nx,
                                            t_idx * o_ny,
                                            std::string * o_setup,
                                            std::string * o_stations_string,
                                            std::string * o_disfile,
                                            std::string * o_batfile){
    
    int l_ncId = 0,l_err = 0;
    l_err = nc_open(i_path_cp.c_str(), NC_NOWRITE, &l_ncId); 
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_dimXId, l_dimYId;
    l_err = nc_inq_dimid(l_ncId, "x", &l_dimXId);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_inq_dimid(l_ncId, "y", &l_dimYId);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimlen(l_ncId,l_dimXId,o_nx);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimlen(l_ncId,l_dimYId,o_ny);
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_var_domain_start_x_id, l_var_domain_start_y_id , l_var_solver_id, l_var_dimension_x_id,
        l_var_dimension_y_id,l_var_endtime_id, l_var_simTime_id, l_var_frequency_id, l_var_b_id,
        l_var_h_id,l_var_hu_id,l_var_hv_id, l_var_dt_id , l_var_disfile_id,l_var_batfile_id;    
    int  l_var_time_step_index_id,l_var_stations_string,l_var_setup,l_simulation_time_for_last_cp_id;

    l_err = nc_inq_varid(l_ncId,"domain_start_x",&l_var_domain_start_x_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"domain_start_y",&l_var_domain_start_y_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"solver",&l_var_solver_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"simulationtimeforlastcp", &l_simulation_time_for_last_cp_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"dimension_x",&l_var_dimension_x_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"dimension_y",&l_var_dimension_y_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"endtime",&l_var_endtime_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"simTime",&l_var_simTime_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"setup",&l_var_setup);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"stations",&l_var_stations_string);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"timeStep",&l_var_time_step_index_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"frequency",&l_var_frequency_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"dt",&l_var_dt_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"disfile",&l_var_disfile_id);
    checkNcErr(l_err,__FILE__, __LINE__);
  
    l_err = nc_inq_varid(l_ncId,"batfile",&l_var_batfile_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId, "b", &l_var_b_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId, "hu", &l_var_hu_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId, "h", &l_var_h_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId, "hv", &l_var_hv_id);
    checkNcErr(l_err,__FILE__, __LINE__);


    // GET THE VARIABLES

    char *l_setup;
    l_err = nc_get_var_string(l_ncId,l_var_setup,&l_setup);
    *o_setup = std::string(l_setup);
    checkNcErr(l_err,__FILE__, __LINE__);

    char *l_batfile;
    l_err = nc_get_var_string(l_ncId,l_var_batfile_id,&l_batfile);
    *o_batfile = std::string(l_batfile);
    checkNcErr(l_err,__FILE__, __LINE__);
 
    char *l_disfile;
    l_err = nc_get_var_string(l_ncId,l_var_disfile_id,&l_disfile);
    *o_disfile = std::string(l_disfile);
    checkNcErr(l_err,__FILE__, __LINE__);

    char *l_stations_string;
    l_err = nc_get_var_string(l_ncId,l_var_stations_string,&l_stations_string);
    *o_stations_string = std::string(l_stations_string);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_simulation_time_for_last_cp_id,o_simulation_time_for_last_cp);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_domain_start_x_id,o_domain_start_x);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_domain_start_y_id,o_domain_start_y);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_dimension_x_id,o_dimension_x);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_dimension_y_id,o_dimension_y);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_endtime_id,o_endtime);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_simTime_id,o_simTime);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_frequency_id,o_frequency);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId, l_var_dt_id, o_dt);
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_time_step_index;
    l_err = nc_get_var_int(l_ncId, l_var_time_step_index_id, &l_time_step_index);
    *o_time_step_index = l_time_step_index;
    checkNcErr(l_err,__FILE__, __LINE__);
    int l_solver = 0;

    l_err = nc_get_var_int(l_ncId, l_var_solver_id, &l_solver);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    if(l_solver == 1){
        *o_solver = "roe";
    }else{
        *o_solver = "fwave";  
    }

    *o_b = new t_real[(*o_nx) * (*o_ny)];
    *o_h = new t_real[(*o_nx) * (*o_ny)];
    *o_hu = new t_real[(*o_nx) * (*o_ny)];
    *o_hv = new t_real[(*o_nx) * (*o_ny)];

    l_err = nc_get_var_float(l_ncId, l_var_b_id, *o_b);
    checkNcErr(l_err,__FILE__, __LINE__); 

    l_err = nc_get_var_float(l_ncId, l_var_h_id, *o_h);
    checkNcErr(l_err,__FILE__, __LINE__); 

    l_err = nc_get_var_float(l_ncId, l_var_hu_id, *o_hu);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId, l_var_hv_id, *o_hv);
    checkNcErr(l_err,__FILE__, __LINE__);  

    l_err = nc_close(l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    free(l_setup);
    free(l_stations_string);
    free(l_batfile);
    free(l_disfile);
    }


Following the implementation of the write and read methods, it's essential to create a test unit to validate the functionality. This test unit will be added to the `Netcdf.test.cpp` file to ensure that the
checkpointing mechanisms work as intended.

.. code-block:: cpp

    TEST_CASE( "Test the NetCdf-CheckPoint ", "[NetCdfCheckpoint]" ) {

    
    tsunami_lab::t_real l_h[16]  =  {0,0,0,0, 
                                     0,1,2,0, 
                                     0,3,4,0, 
                                     0,0,0,0 };

    tsunami_lab::t_real l_hu[16] = { 0,0,0,0, 
                                     0,5,8,0, 
                                     0,6,7,0, 
                                     0,0,0,0};

    tsunami_lab::t_real l_hv[16] = { 0,0,0,0, 
                                     0,2,2,0,
                                     0,2,2,0, 
                                     0,0,0,0};

    tsunami_lab::t_real l_b[16]  = { 0,0,0,0, 
                                     0,0,0,0, 
                                     0,0,0,0, 
                                     0,0,0,0};


    tsunami_lab::t_real l_h_read_result[4]  ={ 1,2,
                                               3,4};
    tsunami_lab::t_real l_hu_read_result[4]  ={5,8,
                                              6,7};
    tsunami_lab::t_real l_hv_read_result[4]  ={2,2,
                                              2,2};

    tsunami_lab::t_real l_b_read_result[4]  ={0,0,
                                              0,0};


    tsunami_lab::io::NetCdf* l_netCdf = new tsunami_lab::io::NetCdf(2,2,"testsFiles/testCheckPoint(the_test_is_in_output_cp_folder).nc");
    REQUIRE(std::filesystem::exists("testsFiles/testCheckPoint(the_test_is_in_output_cp_folder).nc"));
    
    std::string folder_path = "outputs";
    std::string l_check_point_path= folder_path + "/cp";

    if (!std::filesystem::exists(folder_path)) std::filesystem::create_directory(folder_path);
    if (!std::filesystem::exists(l_check_point_path)) std::filesystem::create_directory(l_check_point_path);


    l_netCdf->createCheckPoint("fwave",1,2,3,4,5,6,7,10,3,l_b,l_h,l_hu,l_hv,1,4,2,2,"test2","test3","CheckpointsTest.nc","test5","test6");
    REQUIRE(std::filesystem::exists("outputs/cp/CheckpointsTest.nc"));
    tsunami_lab::t_real *l_ha; 
    tsunami_lab::t_real *l_ba ;
    tsunami_lab::t_real *l_hva;
    tsunami_lab::t_real *l_hua;
    tsunami_lab::t_real o_domain_start_x;
    tsunami_lab::t_real o_domain_start_y;
    tsunami_lab::t_real o_dimension_x;
    tsunami_lab::t_real o_dimension_y;
    std::string solver;
    tsunami_lab::t_real  o_endtime;
    tsunami_lab::t_real  o_simTime;
    tsunami_lab::t_real  o_frequency;
    tsunami_lab::t_real  o_dt;
    tsunami_lab::t_real  o_simulation_time_for_last_cp;
    tsunami_lab::t_idx  o_time_step_index;
    tsunami_lab::t_idx  o_nx;
    tsunami_lab::t_idx  o_ny;
    std::string  o_setup ;
    std::string  o_stations_string;
    std::string  o_disfile;
    std::string  o_batfile;


    tsunami_lab::io::NetCdf::readCheckPoint("outputs/cp/CheckpointsTest.nc",&solver,&o_domain_start_x,
    &o_domain_start_y,&o_dimension_x,&o_dimension_y,&o_endtime,&o_simTime,&o_frequency, &o_dt,&o_simulation_time_for_last_cp,
    &l_ba,&l_ha,&l_hua,
    &l_hva,&o_time_step_index,&o_nx,&o_ny,&o_setup,&o_stations_string,
    &o_disfile,&o_batfile);

    REQUIRE(o_nx == 2);
    REQUIRE(o_ny == 2);
    REQUIRE(o_domain_start_x == 1.0);
    REQUIRE(o_domain_start_y == 2.0);
    REQUIRE(o_dimension_y == 4);
    REQUIRE(o_dimension_x == 3);
    REQUIRE(o_endtime == 5);
    REQUIRE(o_simTime == 6);
    REQUIRE(o_setup == "test2");//problem fix its not saved
    REQUIRE(o_stations_string == "test3");
    REQUIRE(o_time_step_index == 1);
    REQUIRE(o_frequency == 7);
    REQUIRE(o_dt == 10);
    REQUIRE(o_disfile == "test6");
    REQUIRE(o_batfile == "test5");
    REQUIRE(o_simulation_time_for_last_cp == 3);
    for (tsunami_lab::t_idx l_i = 0; l_i < 2 * 2; l_i++)
    {
        REQUIRE(l_ba[l_i] == l_b_read_result[l_i]);
        REQUIRE(l_ha[l_i] == l_h_read_result[l_i]);
        REQUIRE(l_hua[l_i] == l_hu_read_result[l_i]);
        REQUIRE(l_hva[l_i] == l_hv_read_result[l_i]);
        
    }
    
    delete[] l_ba;
    delete[] l_ha;
    delete[] l_hva;
    delete[] l_hua;
    delete l_netCdf;
    }

Before invoking the write and read methods,
modifications need to be made to the `station.cpp` file. In this file,
a method should be implemented to update the station's CSV, ensuring that when a checkpoint is executed
, the data in the checkpoint matches the information in the station CSV file.
To implement the method, we will transform the data from the station CSV file into a structured object in
the `constant.h `file .

.. code-block:: cpp 

    struct DataPoint {
    t_idx x, y;
    t_real water_height, water_hu, water_hv, time_in_seconds;
  };

now lets implement the method.

.. code-block:: cpp 

    void tsunami_lab::io::Station::updateStation(tsunami_lab::t_real simulation_time, std::string csv_file_path) {
    std::ifstream inFile(csv_file_path);
    if (!inFile) {
        std::cerr << "Error opening file: " << csv_file_path << std::endl;
        return;
    }

    std::vector<DataPoint> dataPoints;
    std::string line;

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        DataPoint point;
        char comma;
        if (iss >> point.x >> comma >> point.y >> comma >> point.water_height >> comma
            >> point.water_hu >> comma >> point.water_hv >> comma >> point.time_in_seconds) {
            dataPoints.push_back(point);
        }
        
    }

    inFile.close();


    //tsunami_lab::t_real l_simulation_time = floor(simulation_time); //= std::round(simulation_time * 1)/1;

    auto it = std::find_if(dataPoints.begin(), dataPoints.end(),
        [simulation_time](const DataPoint& point) {
            return floor(point.time_in_seconds) == floor(simulation_time);
        });

    if (it != dataPoints.end()) {
        // Erase elements after the found point
        dataPoints.erase(it + 1, dataPoints.end());

        // Write the modified data back to the file
        std::ofstream outFile(csv_file_path);
        if (!outFile) {
            std::cerr << "Error opening file for writing: " << csv_file_path << std::endl;
            return;
        }

        for (const auto& point : dataPoints) {
            outFile << point.x << ',' << point.y << ',' << point.water_height << ','
                << point.water_hu << ',' << point.water_hv << ',' << point.time_in_seconds << '\n';
        }

        std::cout << "Data after " << simulation_time << " seconds replaced with empty strings." << std::endl;
    }
    else {
        std::cout << "No match found for " << simulation_time << " seconds in the 'time_in_seconds' column." << std::endl;
    }
    }



The method compares the latest simulation time for the station stored in the checkpoint with the time written in the
CSV file. Any data saved after this point will be deleted.


Let's now integrate the write and read methods in the main.cpp file. For the read method, we'll introduce a local variable, allowing the user to decide whether to
utilize a checkpoint. Additionally, we need to check
 if an output file exists for the checkpoint


.. code-block:: cpp 

     bool l_use_cp = tsunami_lab::io::Configuration::readFromConfigBoolean("usecheckpoint");

.. important::

    The checkpoint will be saved in the "outputs/cp" directory. In the JSON file,
    users can customize the names of output files. If two output files
    share the same name, they will overwrite each other, including the checkpoint data.
    For each output, a corresponding checkpoint will be created and overwritten with the new data every 7 timesteps.

.. code-block:: cpp 

    std::string l_temp_outputfile =  "outputs/" + l_temp_outputfilename;
    if(l_use_cp){
    if (!std::filesystem::exists(l_temp_outputfile)) {

      std::cout <<"\033[1;31m\u2717 Cannot use Checkpoint " << "\033[0m"<< std::endl;
      std::cout << "\033[1;31mReason : there is no output file matchs the config file name" <<"\033[0m"<< std::endl;
      return EXIT_FAILURE; 
    }
    //Reading Data from the Checkpoint File
    l_temp_waveprop = "2d";
    l_temp_writer   = "netcdf";

    std::string l_cp_path= "outputs/cp/CheckPoint-" + l_temp_outputfilename;

    std::string l_stations_json_file = "";
    tsunami_lab::io::NetCdf::readCheckPoint(l_cp_path,
                                            &l_temp_solver,
                                            &l_domain_start_x,
                                            &l_domain_start_y,
                                            &l_temp_dimension_x,
                                            &l_temp_dimension_y,
                                            &l_temp_endtime,
                                            &l_simTime,
                                            &l_frequency,
                                            &l_dt,
                                            &l_last_simTime_time,
                                            &l_cp_b,
                                            &l_cp_h,
                                            &l_cp_hu,
                                            &l_cp_hv,
                                            &l_time_step_index,
                                            &l_nx,
                                            &l_ny,
                                            &l_temp_setup,
                                            &l_stations_json_file,
                                            &l_temp_disFile,
                                            &l_temp_bathFile);
    tsunami_lab::io::Configuration::readStationsFromString(l_stations_json_file,l_stations);
    for (const auto& station : l_stations) {
      std::string l_station_path = "stations/" + station.i_name + "/" + station.i_name + ".csv";
      tsunami_lab::io::Station::updateStation(l_last_simTime_time,l_station_path);
    }

And now, let's configure the data we read for our setup.


.. code-block:: cpp 

    if(!l_use_cp){
    for( tsunami_lab::t_idx l_cy = 0; l_cy < l_ny; l_cy++ ){
    tsunami_lab::t_real l_y = l_cy * l_dxy + l_domain_start_y;
    for( tsunami_lab::t_idx l_cx = 0; l_cx < l_nx; l_cx++ ){
        tsunami_lab::t_real l_x = l_cx * l_dxy + l_domain_start_x;

        // get initial values of the setup
        tsunami_lab::t_real l_h = l_setup->getHeight( l_x,
                                                    l_y );
        l_hMax = std::max( l_h, l_hMax );
        tsunami_lab::t_real l_hu = l_setup->getMomentumX( l_x,
                                                        l_y );
        tsunami_lab::t_real l_hv = l_setup->getMomentumY( l_x,
                                                        l_y );
        tsunami_lab::t_real l_bv = l_setup->getBathymetry(l_x,
                                                        l_y );                                       
        // set initial values in wave propagation solver
        l_waveProp->setHeight( l_cx,
                            l_cy,
                            l_h );
        l_waveProp->setMomentumX( l_cx,
                                l_cy,
                                l_hu );
        l_waveProp->setMomentumY( l_cx,
                                l_cy,
                                l_hv );
        l_waveProp->setBathymetry( l_cx,
                                l_cy,
                                l_bv);
    }
    }
    // derive maximum wave speed in setup; the momentum is ignored
    tsunami_lab::t_real l_speedMax = std::sqrt( 9.81 * l_hMax );
    l_dt = 0.50 * l_dxy / l_speedMax;
    }else{
    for( tsunami_lab::t_idx l_cy = 0; l_cy < l_ny; l_cy++ ){

    for( tsunami_lab::t_idx l_cx = 0; l_cx < l_nx; l_cx++ ){ 

        l_waveProp->setHeight(l_cx,
                            l_cy,
                            l_cp_h[l_cx+l_cy*l_nx]);
        l_waveProp->setMomentumX( l_cx,
                                l_cy,
                                l_cp_hu[l_cx+l_cy*l_nx]);
        l_waveProp->setMomentumY( l_cx,
                                l_cy,
                                l_cp_hv[l_cx+l_cy*l_nx]);
        l_waveProp->setBathymetry( l_cx,
                                l_cy,
                                l_cp_b[l_cx+l_cy*l_nx]);
            }
        }    
    }

  
Now, invoke the write method within the `main.cpp` file.
The write function should be called every 7 timesteps within 
the while loop when writing the data.


.. code-block:: cpp

    while(l_simTime < l_temp_endtime){
        .......

      if(l_time_step_index%7 == 0 ){
        std::cout << "\n\033[1;34m" << "Started writing a new Checkpoint ."<< "\033[0m" << std::endl;
        l_netCdf->createCheckPoint(l_temp_solver,
                                    l_domain_start_x,
                                    l_domain_start_y,
                                    l_temp_dimension_x,
                                    l_temp_dimension_y,
                                    l_temp_endtime,
                                    l_simTime,
                                    l_frequency,
                                    l_dt,
                                    l_last_simTime_time,
                                    l_waveProp->getBathymetry(),
                                    l_waveProp->getHeight(),
                                    l_waveProp->getMomentumX(),
                                    l_waveProp->getMomentumY(),
                                    l_time_step_index,
                                    l_waveProp->getStride(),
                                    l_nx,
                                    l_ny,
                                    l_temp_setup,
                                    tsunami_lab::io::Station::Stringify(),
                                    l_checkPointName,
                                    l_temp_disFile,
                                    l_temp_bathFile);
        std::cout << "\033[1;32m\u2713 " << "Done writing the Checkpoint ."<< "\033[0m"<< std::endl;
            }
        }
    }



Personal Contribution
---------------------

- Ward Tammaa, Daniel Schicker Doxygen Documentation
- Mohamad Khaled Minawe, Ward Tammaa, Daniel Schicker Sphnix Documentation
- Daniel Schicker, Mohamad Khaled Minawe , Ward Tammaa functions implementation
- Mohamad Khaled Minawe, Daniel Schicker, Ward Tammaa Unit Testing
- Mohamad Khaled Minawe, Daniel Schicker Geogebra Datei(Calculations for the Unit Tests)
- Ward Tammaa Hosting the code , Action runner


    