#include "malekshan.h"

void argmax_target_tx_interf (double dist_link, double beta, double alpha, int *target_tx_addr, int *target_interf_addr)
{    
  double data_rate = 0;
  double max_data_rate = 0;
  double rg = (5 * dist_link);
  int try_tx_power, try_interf_power;
    
  //do brute force, let's try tx_power=[1,100]mW and interf_power=[1,50]mW
  for (try_tx_power = 1; try_tx_power <= 100; try_tx_power++) {
    for (try_interf_power = 1; try_interf_power <= 50; try_interf_power++) {
      data_rate = (1/(dist_link)^2 * log2(1 + try_tx_power*(dist_link)^(-alpha)/try_interf_power) / (3*sqrt(3)/2 * (rg/dist_link)^2));
      if ((try_tx_power*(dist_link)^(-alpha)/try_interf_power) >= beta) { //check whether min. SINR is satisfied
        if (data_rate > max_data_rate) 
        {
          max_data_rate = data_rate;
          *target_tx_addr = try_tx_power;
          *target_interf_addr = try_interf_power;
        }
      }
    }
  }
}

	
int test_V_empty(set_link_time V)
{
  for (int i=0; i<V.L; i++) {
    for (int j=0; j<V.T; j++)
      if ( ((V).interf_power[i][j] != '\0') || (V.tx_power[i][j] != '\0') ) return 0; 
  }
  return 1; 
}
