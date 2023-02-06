## SFEAS Readme
<img src="https://img.shields.io/badge/version-v1.0-brightgreen" />
prerequisites：
Microsoft Visual Studio 2022
intel oneAPI Base Toolkit(must install Intel® oneAPI Math Kernel Library)
intel oneAPI HPC Toolkit


./BuildVS
this folder is Visual Studio build file
CodeAmout.py is python script to count the number of all codes.


./Examples
this folder is the input file of examples
*.dat is sfeas input file.
*.cdb is ansys input file.


./Doc
this folder is the development document


./Lib
this folder is the library
including arpack and mumps solver.


./Include
this folder is the include
including solver header file.


../Src
this folder is the source code


Current Version:	Structural Finite Element Analysis System(SFEAS)  Release 1.0
(1) Element: Elem45(first order)
including TETRA4, PENTA6,HEXA8.

(2) Analysis Type:	Static(ANTYPE=STA)
				 	Modal Analysis(ANTYPE=MOA)
				 	Direct Transient Analysis(ANTYPE=DTA)
				 	Modal Transient Analysis(ANTYPE=MTA)
				 	Direct Frequency Analysis(ANTYPE=DFA)
				 	Modal Frequency Analysis(ANTYPE=MFA)
(3) Linear Equation SolverType:	pardiso in MKL, mumps
     Eigen SolverType: 			feast eigen solver in MKL, mkl sparse eigen solver in MKL, and sparse eigen solver in Spectra
     
(3) LoadType:	NodeForce，DispLoads, VeloLoads, AcceLoads

(4) Output Type:	Displacement, Velocity, Acceration,Strain, Stress
