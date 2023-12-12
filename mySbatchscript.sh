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
module load compiler/intel/2018-Update2
module load libs/hdf5/1.10.8-gcc-10.2.0
module load libs/zlib/1.2.11-intel-2018
module load libs/netcdf/4.6.1-intel-2018
python3.8 -m pip install --user scons

date
scons
./build/tsunami_lab





