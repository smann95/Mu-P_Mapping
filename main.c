#include "fugacity.h"

int main()
{
    double fug = co2_pengr_fugacity(450,200), 
           Xmu = log(fug/(200*BAR_TO_ATM))*r*450,
           T = 450.0,//kelvin
           p = 200.0,//bar
           SIM_N = 28.16583,//molecules
           SIM_V = 8.0* pow(10,-27),//meters cubed
           mass = 6.644 * pow(10,-26),//kilograms per molecule
           h = 6.6261 * pow(10,-34),//J*s
           R = 8.314459848,//m3Pa/Kmol
           ideal_mu = -BOLTZMANN_KJ_MOLAR*T*log((SIM_V/ SIM_N) * pow(((10.0*M_PI*mass*BOLTZMANN_J_PER_K*T)/(3.0*h*h)),(3.0/2.0))),
           sim_mu = -BOLTZMANN_KJ_MOLAR*T*log(((R*T)/(p*BAR_TO_PASCAL))/AVOGADRO * pow(((10.0*M_PI*mass*BOLTZMANN_J_PER_K*T)/(3.0*h*h)),(3.0/2.0))),
           sim_fug = exp(-(sim_mu-ideal_mu)/(BOLTZMANN_KJ_MOLAR*T)) * (200 * BAR_TO_ATM);
    printf("EQUATION OF STATE FUGACITY    =    %lf\n",fug);
    printf("SIMULATION FUGACITY           =    %lf\n",sim_fug);
    printf("SIMULATION V/N                =    %.12e\n",SIM_V/SIM_N);
    printf("IDEAL RT/P                    =    %.12e\n",((R*T)/(p*BAR_TO_PASCAL))/AVOGADRO);
    printf("EQUATION OF STATE EXCESS MU   =   %lf\n",Xmu/1000.0);//kJ per mole
    printf("IDEAL MU                      =   %lf\n",ideal_mu);
    printf("TOTAL SIMULATION MU           =   %lf",sim_mu);
    return 0;
}
