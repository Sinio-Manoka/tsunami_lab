

Bathymetry and Boundary Conditions
===========================================

Non-zero Source Term
-----------------------
Adding bathymetry to the f-wave solver is easy because you can directly include it in how the jump in the flux function is broken down: 



.. math:: \Delta f - \Delta x \Psi_{i-1/2} =  \sum_{p=1}^2 Z_p.
   :label: eq:fwave_source


The term :math:`\Delta x \Psi_{i-1/2}` summarizes\ [1]_ the effect of the bathymetry:

.. math::
   :label: eq:psi

   \Delta x \Psi_{i-1/2} := \begin{bmatrix}
                                   0 \\
                                   -g (b_r - b_l) \frac{h_l+h_r}{2}
                                 \end{bmatrix}.



now lets implement the bathymetry in our fwave solver:

1. lets change the ``eigencoefficientAlpha`` function:

.. code-block:: cpp

   void tsunami_lab::solvers::fwave::eigencoefficientAlpha(t_real i_inverse[4],
                                                        t_real i_delta_f[2],
                                                        t_real i_b,
                                                        t_real o_eigencoefficients[2]){
      // deltaf - bathymetry
    i_delta_f[1] = i_delta_f[1] - i_b;
    //m x n ° n x p = 
    o_eigencoefficients[0] = (i_inverse[0] * i_delta_f[0]) + (i_inverse[1] * i_delta_f[1]);
    o_eigencoefficients[1] = (i_inverse[2] * i_delta_f[0]) + (i_inverse[3] * i_delta_f[1]);

   }


2. now lets change the our netupdates function: 

.. code-block:: cpp

   void tsunami_lab::solvers::fwave::netUpdates(t_real   i_hL,
                                             t_real   i_hR,
                                             t_real   i_huL,
                                             t_real   i_huR,
                                             t_real   i_bR,
                                             t_real   i_bL,
                                             t_real   o_minus_A_deltaQ[2],
                                             t_real   o_plus_A_deltaQ[2]){

    t_real l_uL = i_huL / i_hL;
    t_real l_uR = i_huR / i_hR;
      
    t_real l_sL = 0;
    t_real l_sR = 0;

    eigenvalues(i_hL,i_hR,l_uL,l_uR,l_sL,l_sR);

    t_real l_inverse[4];

    inverseMatrix(l_sL, l_sR, l_inverse);

    t_real l_fdelta[2];
    flux(i_hL,i_hR,i_huL,i_huR,l_fdelta);

    //compute bathymetry
    t_real l_b = (-m_g) * (i_bR-i_bL) *((i_hL+i_hR)/2);
    
    
    t_real l_eigencoefficients[2];
    eigencoefficientAlpha(l_inverse,l_fdelta,l_b,l_eigencoefficients);

    t_real l_eigens[2] = {l_sL,l_sR};
    decompose(l_eigencoefficients,l_eigens,o_minus_A_deltaQ,o_plus_A_deltaQ);
    

now lets add bathymetry to the respective patches

   1. lets add the bathymetry the ``WavePropagation1d.h`` file :

   .. code-block:: cpp

      t_real * m_b = nullptr;

      t_real const * getBathymetry(){
      return m_b+1;
      }

      void setBathymetry(t_idx  i_ix,
                       t_idx,
                       t_real i_b){
      m_b[i_ix+1] = i_b;
      }

   2. now lets add it to the ``WavePropagation.h`` file


   .. code-block:: cpp

      
      virtual void setBathymetry(t_idx  i_ix,
                               t_idx  i_iy,
                               t_real i_b)=0;

      virtual t_real const * getBathymetry() = 0;


   3. lastly but no least lets add it to the ``WavePropagation1d.cpp`` file: 

         3.1. first lets allocate memory for the Bathymetry cells and init it to zero :

            .. code-block:: cpp

               tsunami_lab::patches::WavePropagation1d::WavePropagation1d( t_idx i_nCells,bool i_choice ) {
                  m_choice = i_choice;
                  m_nCells = i_nCells;

               // allocate memory including a single ghost cell on each side
                  for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
                  m_h[l_st] = new t_real[  m_nCells + 2 ];
                  m_hu[l_st] = new t_real[ m_nCells + 2 ];
                  }
                  m_b = new t_real[ m_nCells + 2 ];

               // init to zero
                  for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
                  for( t_idx l_ce = 0; l_ce < m_nCells; l_ce++ ) {
                  m_h[l_st][l_ce] = 0;
                  m_hu[l_st][l_ce] = 0;
                  m_b[l_ce] = 0;
                     }
                  }
               }

         3.2. now lets include it in the timeStep function:

          .. code-block:: cpp

            void tsunami_lab::patches::WavePropagation1d::timeStep( t_real i_scaling) {
               // pointers to old and new data
               t_real * l_hOld  = m_h[m_step];
               t_real * l_huOld = m_hu[m_step];


               t_real * l_b  = m_b; 

               m_step = (m_step+1) % 2;
               t_real * l_hNew =  m_h[m_step];
               t_real * l_huNew = m_hu[m_step];


               // init new cell quantities
               for( t_idx l_ce = 1; l_ce < m_nCells+1; l_ce++ ) {
                  l_hNew[l_ce]  = l_hOld[l_ce];
                  l_huNew[l_ce] = l_huOld[l_ce];
               }

               // iterate over edges and update with Riemann solutions
               for( t_idx l_ed = 0; l_ed < m_nCells+1; l_ed++ ) {
                  // determine left and right cell-id
                  t_idx l_ceL = l_ed;
                  t_idx l_ceR = l_ed+1;

                  // compute net-updates
                  t_real l_netUpdates[2][2];
                  //std:: cout << l_bOld[l_ceR] << std::endl ;

                  if(m_choice){
                     solvers::Roe::netUpdates(l_hOld[l_ceL],
                                             l_hOld[l_ceR],
                                             l_huOld[l_ceL],
                                             l_huOld[l_ceR],
                                             l_netUpdates[0],
                                             l_netUpdates[1]);
                  }else{
                     solvers::fwave::netUpdates( l_hOld[l_ceL],
                                                l_hOld[l_ceR],
                                                l_huOld[l_ceL],
                                                l_huOld[l_ceR],
                                                l_b[l_ceR],
                                                l_b[l_ceL],
                                                l_netUpdates[0],
                                                l_netUpdates[1]);
                  }
    
                  // update the cells' quantities
                  l_hNew[l_ceL]  -= i_scaling * l_netUpdates[0][0];
                  l_huNew[l_ceL] -= i_scaling * l_netUpdates[0][1] ;


                  l_hNew[l_ceR]  -= i_scaling * l_netUpdates[1][0];
                  l_huNew[l_ceR] -= i_scaling * l_netUpdates[1][1] ;
    
  
    
               }
            }

         3.3. now lets set Boundary of the bathymetry in ``setGhostOutflow`` function:

            .. code-block:: cpp

               void tsunami_lab::patches::WavePropagation1d::setGhostOutflow() {
                  t_real * l_h = m_h[m_step];
                  t_real * l_hu = m_hu[m_step];
                  t_real * l_b = m_b;

                  // set left boundary
                  l_h[0] = l_h[1];
                  l_hu[0] = l_hu[1];
                  l_b[0] = l_b[1];

                  // set right boundary
                  l_h[m_nCells+1]  = l_h[m_nCells];
                  l_hu[m_nCells+1] = l_hu[m_nCells];
                  l_b[m_nCells+1]  = l_b[m_nCells];
               }
   
         3.3.4. lastly we have to add the bathymetry to the ``~WavePropagation1d`` function:

         .. code-block:: cpp

            tsunami_lab::patches::WavePropagation1d::~WavePropagation1d() {
               for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
                  delete[] m_h[l_st];
                  delete[] m_hu[l_st];
               }
               delete[] m_b;
            }




Personal Contribution
---------------------

- Ward Tammaa, Daniel Schicker Doxygen Documentation
- Mohamad Khaled Minawe, Ward Tammaa Sphnix Documentation
- Daniel Schicker, Mohamad Khaled Minawe , Ward Tammaa functions implementation
- Mohamad Khaled Minawe, Daniel Schicker, Ward Tammaa Unit Testing
- Mohamad Khaled Minawe, Daniel Schicker Geogebra Datei(Calculation for the Unit Tests)
- Ward Tammaa Hosting the code , Action runner


    