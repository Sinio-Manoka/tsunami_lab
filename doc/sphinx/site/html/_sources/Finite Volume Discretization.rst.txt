




Finite Volume Discretization
===========================================

Finite Volume Discretization is a powerful and widely used numerical technique in the field of computational fluid dynamics (CFD)
and other scientific and engineering disciplines. It plays a crucial role in the simulation and analysis of physical processes, particularly those governed by partial differential equations (PDEs)


Integrating our f-wave solver into the one-dimensional implementation of a wave propagation
-------------------------------------------------------------------------------------------
There are several ways to integrate our F-wave solver into the one-dimensional wave propagation implementation, including the following options:

- Using the command prompt.

- Utilizing a configuration file, and so on.

Today, we will focus on using the command prompt to integrate our solver with the following files: ``WavePropagation1d.h``, ``WavePropagation1d.cpp``, ``WavePropagation1d.test.cpp``. Let's proceed step by step:

1. In the ``WavePropagation1d.h`` ile, we will define a boolean variable. Update the code in "WavePropagation1d.h" as follows:

.. code-block:: cpp

   class tsunami_lab::patches::WavePropagation1d: public WavePropagation {  
   private:
    bool m_choice = true;
   }

 
2. We will modify the constructor in the ``WavePropagation1d.cpp`` file to include the boolean variable. This change enables us to select between the Roe solver and our F-wave solver within the test files and the command prompt.  

.. code-block:: cpp

   tsunami_lab::patches::WavePropagation1d::WavePropagation1d( t_idx i_nCells,bool i_choice ) {
      m_choice = i_choice;
      m_nCells = i_nCells;

  // allocate memory including a single ghost cell on each side
      for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
      m_h[l_st] = new t_real[  m_nCells + 2 ];
      m_hu[l_st] = new t_real[ m_nCells + 2 ];
   }

  // init to zero
      for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
         for( t_idx l_ce = 0; l_ce < m_nCells; l_ce++ ) {
          m_h[l_st][l_ce] = 0;
            m_hu[l_st][l_ce] = 0;
         }
      }
   }  

3. In the same file, we will navigate to the ``timeStep`` function and add a call to ``netupdates`` based on the user's choice.. 

.. code-block:: cpp

   void tsunami_lab::patches::WavePropagation1d::timeStep( t_real i_scaling ) {
      if(m_choice){
      solvers::Roe::netUpdates( l_hOld[l_ceL],
                              l_hOld[l_ceR],
                              l_huOld[l_ceL],
                              l_huOld[l_ceR],
                              l_netUpdates[0],
                              l_netUpdates[1] );
    }else{
      solvers::fwave::netUpdates( l_hOld[l_ceL],
                                  l_hOld[l_ceR],
                                  l_huOld[l_ceL],
                                  l_huOld[l_ceR],
                                  l_netUpdates[0],
                                  l_netUpdates[1]);
      }
   }

4. To conclude, we navigate to the "main.cpp" file, where we need to modify the solver constructor to permit the selection between our F-wave and Roe solver options.

.. code-block:: cpp

   // construct solver
  tsunami_lab::patches::WavePropagation *l_waveProp;
  if (i_argc > 2) {
      for (char* ptr = i_argv[2]; *ptr; ++ptr) {
          *ptr = std::tolower(static_cast<unsigned char>(*ptr));
      }
      if (std::strstr(i_argv[2], "roe") != nullptr) {
          std::cout << "ROE Will be Used" << std::endl;
          l_waveProp = new tsunami_lab::patches::WavePropagation1d( l_nx , true );
      } else {
          std::cout << "FWAVE Will be Used" << std::endl;
          l_waveProp = new tsunami_lab::patches::WavePropagation1d( l_nx , false );
      }
  }else{
    std::cout << "FWAVE Will be Used" << std::endl;
    l_waveProp = new tsunami_lab::patches::WavePropagation1d( l_nx , false );
  }  

.. Important:: 
   To select between the F-wave and Roe solver, we execute the following command.

      .. code-block:: 

         # ./build/tsunami_lab 200 Roe 

   oder 

      .. code-block::

         # ./build/tsunami_lab 200 fwave 

   if the user does not choose any solver, the F-wave solver will be executed automatically.


5. To create a unit test for our solvers, we need to include a ``true`` or ``false`` argument in the constructor of the unit test:

   .. code-block:: cpp

      TEST_CASE( "Test the 1d wave propagation solver.", "[WaveProp1d]" ) {
      tsunami_lab::patches::WavePropagation1d m_waveProp( 100 , true );
      ...
      }
   .. Important::
      ``true`` will use the ``Roe solver`` and ``flase`` will use the ``Fwave solver``























