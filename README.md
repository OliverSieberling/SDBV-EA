This repository contains the code for calculating hitting times and generating figures in my bachelor's thesis 

 - [Dynamic monotone functions on the (1+1)-EA: Drift minimization does not maximize hardness](https://polybox.ethz.ch/index.php/s/jkGGklDlNdYuz0b)
  
and the paper 
- [Hardest Monotone Functions for Evolutionary Algorithms](https://arxiv.org/abs/2311.07438) (full version)
  
by M. Kaufmann, M. Larcher, J. Lengler, and O. Sieberling. 

# Calculations
- hitting_time_calculation.cpp: code used to generate the input for Wolfram Alpha to calculate exact hitting time for SDBV with a certain cutoff
- modified_sdbv_even.ipynb: jupyter notebook to generate Figure 5.1 (thesis) / Figure 1 (conference paper)
- modified_sdbv_odd.ipynb: jupyter notebook to generate Figure 5.2 (thesis) / Figure 1 (conference paper) and Figure 5.3 (thesis)
  
# Simulations
- simulations.cpp: code used to generate output files (all except PO-EA)
- po-ea_simulation.cpp: code used to generate output files (only PO-EA)
- output_files: contains output files of simulations that are used to generate the figures
- notebooks: jupyter notebooks used to generate Figure 2 (conference paper)

# Contact
Oliver Sieberling - osieberling@ethz.ch
