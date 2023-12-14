

Optimization
=============

ARA
---

Upload your code and Ch. 6’s input data to the cluster. Compile your code on the cluster
........................................................................................


To deploy the project on the cluster, we cloned it from our Git repository and included all submodules.

.. code-block:: shell

   git submodule init
   git submodule update


To compile our code, we utilized an sbatch script, necessitating adjustments to the paths in our sconstruct.


.. code-block:: shell

   #!/bin/bash
   #SBATCH --job-name=tsunami
   #SBATCH --output=tsunami_output.txt
   #SBATCH --error=tsunami_error.txt
   #SBATCH --partition=s_hadoop
   #SBATCH --nodes=1
   #SBATCH --ntasks=1
   #SBATCH --time=10:00
   #SBATCH --cpus-per-task=72


   # Set the email address where notifications should be sent.
   #SBATCH --mail-user=minawe.mohamad.khaled@uni-jena.de

   # Specify the types of email notifications you want to receive.
   #SBATCH --mail-type=BEGIN,END,FAIL

   # Load modules 
   module load tools/python/3.8
   module load compiler/gcc/11.2.0
   module load compiler/intel/2020-Update2
   module load libs/hdf5/1.10.8-gcc-10.2.0
   module load libs/zlib/1.2.11-intel-2018
   module load libs/netcdf/4.6.1-intel-2018
   python3.8 -m pip install --user scons

   date
   scons
   ./build/tsunami_lab


in the sconstruct we had to add pathes fo the compiler to it.

.. code-block:: python

   import SCons
   import os
   import SCons.Script
   import sys

   print( '####################################' )
   print( '### Tsunami Lab                  ###' )
   print( '###                              ###' )
   print( '### https://scalable.uni-jena.de ###' )
   print( '####################################' )
   print()
   print('runnning build script')

   # configuration
   vars = Variables()

   vars.AddVariables(
      EnumVariable('mode',
                  'compile modes, option \'san\' enables address and undefined behavior sanitizers',
                  'release',
                  allowed_values=('release', 'debug', 'release+san', 'debug+san')
                  ),
      BoolVariable('use_icpc',
                  'Use Intel C++ Compiler (icpc)',
                  True
                  ),
      BoolVariable('use_report',
                  'Enable compiler optimization report',
                  False  # Set the default value to False; adjust as needed
                  )
   )

   # exit in the case of unknown variables
   if vars.UnknownVariables():
   print( "build configuration corrupted, don't know what to do with: " + str(vars.UnknownVariables().keys()) )
   exit(1)

   # create environment
   env = Environment( variables = vars )

   if env['use_icpc']:
      #env.Replace(CXX=os.environ.get('CXX', 'icpc'))
      env.Replace(CXX='/cluster/intel/parallel_studio_xe_2020.2.108/compilers_and_libraries_2020/linux/bin/intel64/icpc')
      print("Selected Compiler: Intel C++ Compiler (icpc)")
   else:
      # Use GNU Compiler by default
      #env.Replace(CXX=os.environ.get('CXX', 'g++'))
      env.Replace(CXX='/cluster/spack/opt/spack/linux-centos7-broadwell/gcc-10.2.0/gcc-11.2.0-c27urtyjryzoyyqfms5m3ewi6vrtvt44/bin/gcc')
      print("Selected Compiler: GNU Compiler (g++)")

      

   print(f"Selected Compiler: {env['CXX']}")

   # generate help message
   Help( vars.GenerateHelpText( env ) )

   # add default flags
   env.Append( CXXFLAGS = [ '-std=c++17',
                           '-Wall',
                           '-g',
                           '-Wextra',
                           '-Wpedantic',
                           '-Werror' ] )

   # set optimization mode
   if 'debug' in env['mode']:
   env.Append( CXXFLAGS = [ '-g',
                              '-O0' ] )
   else:
   env.Append( CXXFLAGS = [ '-O2' ] )

   if env['use_report']:
   if not env['use_icpc']:
         print("Warning: Cannot generate report because you are running the code on the GNU Compiler.")
   else:
         env.Append(CXXFLAGS=['-qopt-report=5'])
         print("the report is in the build folder")   


   # add sanitizers
   if 'san' in  env['mode']:
   env.Append( CXXFLAGS =  [ '-g',
                              '-fsanitize=float-divide-by-zero',
                              '-fsanitize=bounds',
                              '-fsanitize=address',
                              '-fsanitize=undefined',
                              '-fno-omit-frame-pointer' ] )
   env.Append( LINKFLAGS = [ '-g',
                              '-fsanitize=address',
                              '-fsanitize=undefined' ] )


   conf = Configure(env)

   if not conf.CheckLibWithHeader('netcdf', 'netcdf.h', 'c++'):
      exit(1)




   # add Catch2
   env.Append(CXXFLAGS = [ '-isystem', 'submodules/Catch2/single_include'])

   # add nlohmann json 
   env.Append(CXXFLAGS = ['-isystem', 'submodules/json/single_include'])

   env.Append(LIBPATH=['/home/winter/tools/netcdf/include'])

   # get source files
   VariantDir( variant_dir = 'build/src',
               src_dir     = 'src' )

   env.sources = []
   env.tests = []

   Export('env')
   SConscript( 'build/src/SConscript' )
   Import('env')

   env.Program( target = 'build/tsunami_lab',
               source = env.sources + env.standalone )

   env.Program( target = 'build/tests',
               source = env.sources + env.tests )


The SConstruct on our PC appears different due to issues encountered when loading the Intel compiler on the cluster.

.. code-block:: python

   ##
   # @author Alexander Breuer (alex.breuer AT uni-jena.de)
   #
   # @section DESCRIPTION
   # Entry-point for builds.
   ##
   import SCons

   print( '####################################' )
   print( '### Tsunami Lab                  ###' )
   print( '###                              ###' )
   print( '### https://scalable.uni-jena.de ###' )
   print( '####################################' )
   print()
   print('runnning build script')

   # configuration
   vars = Variables()

   vars.AddVariables(
      EnumVariable('mode',
                  'compile modes, option \'san\' enables address and undefined behavior sanitizers',
                  'release',
                  allowed_values=('release', 'debug', 'release+san', 'debug+san')
                  ),
      BoolVariable('use_icpc',
                  'Use Intel C++ Compiler (icpc)',
                  True
                  ),
      BoolVariable('use_report',
                  'Enable compiler optimization report',
                  False  # Set the default value to False; adjust as needed
                  )
   )


   # exit in the case of unknown variables
   if vars.UnknownVariables():
   print( "build configuration corrupted, don't know what to do with: " + str(vars.UnknownVariables().keys()) )
   exit(1)

   # create environment
   env = Environment( variables = vars )

   # generate help message
   Help( vars.GenerateHelpText( env ) )

   # add default flags
   env.Append( CXXFLAGS = [ '-std=c++17',
                           '-Wall',
                           '-Wextra',
                           '-Wpedantic',
                           '-Werror' ] )

   # set optimization mode
   if 'debug' in env['mode']:
   env.Append( CXXFLAGS = [ '-g',
                              '-O0' ] )
   else:
   env.Append( CXXFLAGS = [ '-O2' ] )

   # add sanitizers
   if 'san' in  env['mode']:
   env.Append( CXXFLAGS =  [ '-g',
                              '-fsanitize=float-divide-by-zero',
                              '-fsanitize=bounds',
                              '-fsanitize=address',
                              '-fsanitize=undefined',
                              '-fno-omit-frame-pointer' ] )
   env.Append( LINKFLAGS = [ '-g',
                              '-fsanitize=address',
                              '-fsanitize=undefined' ] )
   

   if env['use_icpc']:
      env.Replace(CXX='/opt/intel/oneapi/compiler/2023.2.2/linux/bin/intel64/icpc')
      print("Selected Compiler: Intel C++ Compiler (icpc)")
         

   if env['use_report']:
   if not env['use_icpc']:
         print("Warning: Cannot generate report because you are running the code on the GNU Compiler.")
   else:
         env.Append(CXXFLAGS=['-qopt-report=5'])
         print("the report is in the build folder")   


   env.Append(LIBS=['netcdf'])

   env.Append(LIBS=['z'])

   env.Append(LIBS=['hdf5_serial'])

   if 'LD_LIBRARY_PATH' not in env['ENV']:
      env['ENV']['LD_LIBRARY_PATH'] = ''

   env['ENV']['LD_LIBRARY_PATH'] = '/usr/lib/x86_64-linux-gnu' + env['ENV']['LD_LIBRARY_PATH']

   # add Catch2
   env.Append(CXXFLAGS = [ '-isystem', 'submodules/Catch2/single_include'])

   # add nlohmann json 
   env.Append(CXXFLAGS = ['-isystem', 'submodules/json/single_include'])

   env.Append( CXXFLAGS = [ '-qopt-report=5' ] )

   env.Append(LIBPATH=['/home/winter/tools/netcdf/include'])

   # get source files
   VariantDir( variant_dir = 'build/src',
               src_dir     = 'src' )

   env.sources = []
   env.tests = []

   Export('env')
   SConscript( 'build/src/SConscript' )
   Import('env')

   env.Program( target = 'build/tsunami_lab',
               source = env.sources + env.standalone )

   env.Program( target = 'build/tests',
               source = env.sources + env.tests )



We attempted to utilize os.environ to retrieve the compiler paths, but it only captured the path of the GNU compiler. 
This is why we manually obtained the compiler paths and included arguments to facilitate the selection between compilers.

.. code-block:: shell
   
   scons use_icpc=True use_report=True



The option use_icpc is for selecting the compiler, and use_report is for choosing whether to generate a report. The report can only be obtained when using the Intel compiler.



   
Run different scenarios using interactive and batch jobs
.........................................................

We executed two scenarios, one for Tohoku and another for Chile, using the following configuration file:

**chile**:


.. code-block:: cpp

   {
      "solver" : "fwave",
      "dimension_x" : 3500000,
      "dimension_y" : 2950000,
      "setup" :  "tsunamievent2d",
      "nx" : 700,
      "ny" : 590,
      "k"  : 3,
      "hu" : 0,
      "location" : 0,
      "hv":0.0,
      "hr": 55,
      "hl": 25,
      "domain_start_x" : -3000000,
      "domain_start_y" : -1450000,
      "wavepropagation" : "2d",
      "endtime" : 36000,
      "writer" : "netcdf",
      "bathfile" : "data/output/chile_gebco20_usgs_250m_bath_fixed.nc",
      "disfile" : "data/output/chile_gebco20_usgs_250m_displ_fixed.nc",
      "outputfilename": "simulation.nc",
      "usecheckpoint" : false
   }

**tohoku**:
.. code-block:: cpp 

   {
      "solver" : "fwave",
      "dimension_x" : 2700000,
      "dimension_y" : 1500000,
      "setup" :  "tsunamievent2d",
      "nx" : 700,
      "ny" : 590,
      "k"  : 3,
      "hu" : 0,
      "location" : 0,
      "hv":0.0,
      "hr": 55,
      "hl": 25,
      "domain_start_x" : -200000,
      "domain_start_y" : -750000,
      "wavepropagation" : "2d",
      "endtime" : 36000,
      "writer" : "netcdf",
      "bathfile" : "data/output/tohoku_gebco20_ucsb3_250m_bath.nc",
      "disfile" : "data/output/tohoku_gebco20_ucsb3_250m_displ.nc",
      "outputfilename": "simulation.nc",
      "usecheckpoint" : false
   }




 Add a timer to your solver which allows you to measure the duration of the time stepping loop. 
 ................................................................................................

lets add a timer in our main.cpp file and lets not forgett to exlude the io overhead nad the setup time from the measurements

.. code-block:: cpp

   int main() {

      auto l_startTimer = std::chrono::high_resolution_clock::now(); 

      tsunami_lab::t_idx l_nx = 0;
      tsunami_lab::t_idx l_ny = 1;
      tsunami_lab::t_real l_dxy = 25;

      ........

      std::chrono::nanoseconds l_durationWriting = std::chrono::nanoseconds::zero();
      std::chrono::nanoseconds l_durationWritingStation = std::chrono::nanoseconds::zero();
      std::chrono::nanoseconds l_durationWritingCheckpoint = std::chrono::nanoseconds::zero();
      std::chrono::nanoseconds l_durationWritingConstat = std::chrono::nanoseconds::zero();

      .......

      auto l_loopTimer = std::chrono::high_resolution_clock::now();
  
      if(l_temp_writer == "netcdf"){
         l_netCdf = new tsunami_lab::io::NetCdf(l_nx,l_ny,l_k,l_outputFile);
         auto l_startWritingCostant = std::chrono::high_resolution_clock::now();
         l_netCdf->fillConstants(l_nx,
                                 l_ny,
                                 l_k,
                                 l_waveProp->getStride(),
                                 l_dxy,
                                 l_domain_start_x,
                                 l_domain_start_y,
                                 l_waveProp->getBathymetry(),
                                 l_outputFile);
         auto l_endWritingCostant = std::chrono::high_resolution_clock::now();
         l_durationWritingConstat =  l_endWritingCostant  - l_startWritingCostant ;            
      }

      while( l_simTime < l_temp_endtime ){
         l_waveProp->setGhostOutflow(false);
         if( l_timeStep % 25 == 0 ) {

            auto l_startWriting = std::chrono::high_resolution_clock::now();

            if(l_temp_writer == "csv"){
            std::string l_path = "outputs/solution_" + std::to_string(l_time_step_index) + ".csv";
            std::ofstream l_file;
            l_file.open( l_path );
            tsunami_lab::io::Csv::write(l_dxy,
                                          l_nx,
                                          l_ny,
                                          l_waveProp->getStride(),
                                          l_domain_start_x,
                                          l_domain_start_y,
                                          l_temp_waveprop,
                                          l_waveProp->getHeight(),
                                          l_waveProp->getMomentumX(),
                                          l_waveProp->getMomentumY(),
                                          l_waveProp->getBathymetry(),
                                          l_file);
            auto l_endWriting = std::chrono::high_resolution_clock::now();
            l_durationWriting += l_endWriting - l_startWriting;

            l_file.close();
            }else{
            
            l_netCdf->updateFile( l_nx,
                                    l_ny,
                                    l_waveProp->getStride(),
                                    l_time_step_index,
                                    l_k,
                                    l_simTime,
                                    l_waveProp->getHeight(),
                                    l_waveProp->getMomentumX(),
                                    l_waveProp->getMomentumY(),
                                    l_outputFile);
            auto l_endWriting = std::chrono::high_resolution_clock::now();
            //duration of the writing method
            l_durationWriting += l_endWriting - l_startWriting ;

                                    
            }
            l_time_step_index++;
            
         if(l_temp_waveprop == "2d"){  
            if(l_time_step_index%7 == 0 ){
               auto l_startWritingCheckpoint = std::chrono::high_resolution_clock::now();
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
                                          l_k,
                                          l_temp_setup,
                                          tsunami_lab::io::Station::Stringify(),
                                          l_checkPointName,
                                          l_temp_disFile,
                                          l_temp_bathFile);

               auto l_endWritingCheckpoint = std::chrono::high_resolution_clock::now();
               l_durationWritingCheckpoint += l_endWritingCheckpoint - l_startWritingCheckpoint ;

            std::cout << "\033[1;32m\u2713 " << "Done writing the Checkpoint ."<< "\033[0m"<< std::endl;
            }
            }
         }
         
         //STATIONS_---------------------------------------------START 
         if(l_current_frequency_time <= l_simTime){
            auto l_startWritingStation = std::chrono::high_resolution_clock::now();
            for (const auto& station : l_stations) {
            std::string l_foldername = "stations/"+station.i_name;
            if (!std::filesystem::exists(l_foldername)){
               std::filesystem::create_directory(l_foldername);
            }
            //compute cell ID
            tsunami_lab::t_idx l_ix = ((station.i_x - l_domain_start_x ) / l_dxy )+ l_waveProp->getGhostcellX();
            tsunami_lab::t_idx l_iy = ((station.i_y - l_domain_start_y ) / l_dxy )+ l_waveProp->getGhostcellY();
            if(l_temp_waveprop == "1d"){
               l_iy = 0; 
            }
            tsunami_lab::t_idx l_id = l_iy * l_waveProp->getStride() + l_ix;
            const tsunami_lab::t_real* l_water_height =  l_waveProp->getHeight();
            const tsunami_lab::t_real* l_water_hu =  l_waveProp->getMomentumX();
            const tsunami_lab::t_real* l_water_hv =  l_waveProp->getMomentumY();
            std::string l_station_path = l_foldername +"/"+ station.i_name+".csv";
            if(l_temp_waveprop == "2d"){
            tsunami_lab::io::Station::write(l_ix,
                                             l_iy,
                                             l_simTime,
                                             l_water_height[l_id],
                                             l_water_hu[l_id],
                                             l_water_hv[l_id],
                                             l_station_path,
                                             l_temp_waveprop);
            }else{
               tsunami_lab::io::Station::write(l_ix,
                                             l_iy,
                                             l_simTime,
                                             l_water_height[l_id],
                                             l_water_hu[l_id],
                                             -1,
                                             l_station_path,
                                             l_temp_waveprop);
            }
            }
            l_last_simTime_time = l_simTime;
            l_current_frequency_time = l_current_frequency_time + l_frequency;
            auto l_endWritingStation = std::chrono::high_resolution_clock::now();
            l_durationWritingStation += l_endWritingStation - l_startWritingStation ;


         }
         //STATIONS----------------------------------------------END

         l_waveProp->timeStep( l_scaling);
         l_timeStep++;
         l_simTime += l_dt;
         updateProgressBar(l_simTime, l_temp_endtime,50);

      }


Now let's calculate the time per cell and iteration.

.. code-block:: cpp 

   auto l_endTimer = std::chrono::high_resolution_clock::now();
   auto l_duration = l_endTimer - l_startTimer;
   auto l_durationLoop = l_endTimer - l_loopTimer;

  std::cout << std::endl;
  std::cout << "total duration: " << std::endl;
  printDuration(l_duration);
  std::cout << "loop duration: " << std::endl;
  printDuration(l_durationLoop - l_durationWritingStation - l_durationWritingCheckpoint - l_durationWriting- l_durationWritingConstat);
  std::cout << "Station: " << std::endl;
  printDuration(l_durationWritingStation);
  std::cout << "Checkpoint: " << std::endl;
  printDuration(l_durationWritingCheckpoint);
  std::cout << "time per cell: " << std::endl;
  printDuration((l_durationLoop - l_durationWritingStation - l_durationWritingCheckpoint - l_durationWriting- l_durationWritingConstat)/(l_nx * l_ny));
  std::cout << "time per iteration: " << std::endl;
  printDuration((l_durationLoop - l_durationWritingStation - l_durationWritingCheckpoint - l_durationWriting- l_durationWritingConstat)/(l_timeStep * l_nx * l_ny));


he ``printDuration`` is a method that we implemented to display the time in hours, minutes, seconds, and nanoseconds.

.. code-block:: cpp 

   void printDuration(std::chrono::nanoseconds duration) {
   auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
      duration -= hours;

      auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
      duration -= minutes;

      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
      duration -= seconds;

      auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
      duration -= milliseconds;

      auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
      duration -= microseconds;

      auto nanoseconds = duration;

      std::cout << "Duration: " << hours.count() << " hours, "
               << minutes.count() << " minutes, "
               << seconds.count() << " seconds, "
               << milliseconds.count() << " milliseconds, "
               << microseconds.count() << " microseconds, "
               << nanoseconds.count() << " nanoseconds" << std::endl;
   }


Is the cluster faster than your own computer
............................................


We compiled both events on Mohamad Khaled Minawe's PC and the cluster, then compared the results.
the results for both can be seen in the following pictures:


For Chile:

Mohamad Khaled Minawe's PC:

.. image:: _static/chile_event_Khaled_pc.png
   :width: 700px
   :height: 500px
   :scale: 100 %
   :alt: alternate text
   :align: right



cluster:


.. image:: _static/chile_event_cluster_node.png
   :width: 700px
   :height: 500px
   :scale: 100 %
   :alt: alternate text
   :align: right


For Tohoku:

Mohamad Khaled Minawe's PC:

.. image:: _static/tohoku_event_Khaled_pc.png
   :width: 700px
   :height: 500px
   :scale: 100 %
   :alt: alternate text
   :align: right



cluster:


.. image:: _static/tohoku_event_cluster_node.png
   :width: 700px
   :height: 500px
   :scale: 100 %
   :alt: alternate text
   :align: right



The pictures indicate that Mohamad Khaled's PC is significantly faster than the cluster.



Compilers
---------











Personal Contribution
---------------------

- Ward Tammaa, Daniel Schicker Doxygen Documentation
- Mohamad Khaled Minawe, Ward Tammaa, Daniel Schicker Sphnix Documentation
- Daniel Schicker, Mohamad Khaled Minawe , Ward Tammaa functions implementation
- Mohamad Khaled Minawe, Daniel Schicker, Ward Tammaa Unit Testing
- Mohamad Khaled Minawe, Daniel Schicker Geogebra Datei(Calculations for the Unit Tests)
- Ward Tammaa Hosting the code , Action runner


    