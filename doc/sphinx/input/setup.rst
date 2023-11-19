.. Tsunami Project documentation master file, created by
   sphinx-quickstart on Sat Oct 28 03:50:41 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.
.. _ch:setup:




   



Introduction
===========================================
**Today, we will explore the process of setting up our tsunami project.**


Getting Started
-------------------
Before you begin, make sure you have the following prerequisites:

- **A C++ compiler (e.g., g++).**

- **Scons (for building).**

- **Git (for cloning the repository).**




.. _Installation:

Installation
----------------
**1. Clone the repository from our** `GitHub Page`_:

.. _GitHub Page: https://github.com/Sinio-Manoka/tsunami_lab


.. code-block:: 

    git clone https://github.com/Sinio-Manoka/tsunami_lab.git 


**2. Install** `Scons`_ **on Linux :**

.. _Scons: https://scons.org/doc/2.2.0/HTML/scons-user/x121.html#:~:text=If%20your%20Linux%20distribution%20does,%2Fusr%2Flib%2Fscons.&text=Or%2C%20you%20can%20use%20a%20graphical%20RPM%20package%20manager.


.. code-block:: 

      pip3 install Scons



**3. Include Catch2 and nlohmann/json.hpp:**

   3.1. Navigate to the project directory:


      .. code-block:: 

          cd tsunami_lab/



   3.2. initialize the submodules:


      .. code-block:: 

        git submodule init 



   3.3. update the submodules within a Git repository:

      .. code-block:: 

          git submodule update 


.. important::
   
   If you possess an outdated version of our project, please do these steps once more.



**4. How to build the project:**


   4.1. Navigate to the project directory:


      .. code-block:: 

          cd tsunami_lab/

   
  
   4.2. Build the project with Scons:


      .. code-block:: 

          scons

   4.3. You will find the compiled binary in the build directory.




Usage
--------
**To use the Riemann solver, follow these steps:**

**1. To modify the arguments for the initial run, you need to navigate to the "configs" folder.**

   .. code-block:: 

     cd configs



**2. Within the folder, locate config.json file and navigate into it:**

   .. code-block:: 

      {
         "solver" : "fwave",
         "dimension" : 100,
         "setup" :  "dambreak2d",
         "nx" : 200,
         "ny" : 200,
         "hu" : 0.0,
         "location" : 5.0,
         "hv":0.0,
         "hr": 10,
         "hl":0,
         "domain_start" : -50,
         "wavepropagation" : "2d"
         "endtime" : 20
      }

.. important::

      All values have to be written in small letters!!

      solver: Choose between fwave or roe.

      hv: Momentum in the y-direction. should be a number.

      ny: Number of cells in the y-direction; should be an integer.

      setup: Select from shockshock, rarerare, tsunamievent1d, dambreak1d, supercritical, subcritical, dambreak2d.

      nx: Number of cells in the x-direction; should be an integer.

      location: Location for the setup; should be a float number.

      hr: Right heights; should be a number.

      hl: Left heights; should be a number.

      wavepropagation: Choose between 1d or 2d.

      endtime: End time of the simulation; should be a float number.

      domain_start: Starting point of our domain. For the circular dam break, set the dimension to -50.

      dimension: Width and length of our domain at this point in our project. l_dxy = dimension / l_nx; 

      Feel free to adjust the arguments to suit your needs. 
      And don't forget to build the project after every change.
   
.. important:: 


   If you opt for wavepropagation1d, go to the "stations.json" file located in the "config" directory, and confirm that all y-coordinates are configured to 0.   

   For shockshock and rarerare setups, relevant inputs include (hl, hu, location, nx, wavepropagation, dimension, solver, endtime).

   For dambreak1d setups, relevant inputs include (hl, hr, location, nx, wavepropagation, dimension, solver, endtime).

   For supercritical and subcritical setups, relevant inputs include (nx, wavepropagation, dimension, solver, endtime).

   For tsunamievent1d setups, relevant inputs include (dimension=440500.0, endtime, nx, wavepropagation=1d, solver).

   For dambreak2d setups, relevant inputs include (nx, ny, endtime, dimension, wavepropagation=2d, domain_start, solver).

   For each setup, choose the necessary inputs to modify, as other inputs will not affect the setup.

 

**3. Navigate to the build directory:**

.. code-block:: 

     cd build


**4. To run the the project, use the following command:**

.. code-block:: 

    ./tsunami_lab


.. important:: 

   After executing ``./tsunami_lab``, the arguments you specify in the JSON file will be automatically incorporated into the project.
   
   The solution can be located within a directory named ``outputs``.


Documentation
----------------
**The Project Documentation can be found** `Here`_ .

.. _Here: https://doxygen-tsunami.web.app/


how to build documentation
..........................

To build our documentation, you need to install Sphinx and Doxygen on your Linux/Ubuntu system.

   .. code-block::

      apt-get install doxygen

      pip install -U sphinx
       
.. Important::

   to build our project in Sphnix we need to install two things:

   .. code-block::

      pip install sphinxcontrib-video

      pip install sphinx-rtd-theme

   

1. doxygen:
   
   
You can use our configuration file ``dconfig``, which is located inside the ``doc\doxygen\data`` directory.

now just build the doxygen documentation : 

.. code-block:: 

   doxygen dconfig

     

2. Sphnix:

   2.1. We need to navigate to the ``doc\sphnix\maker\``

   2.2. execute the following command to build our documentation:

       .. code-block:: 
         
         make html

   2.3 The built ``documentation can be found in \tsunami_lab\doc\sphinx\site``.











