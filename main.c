#include "fugacity.h"

int main()
{
    double r = 8.3144598,//j per kelvinmoles
           fug = co2_pengr_fugacity(450,200), 
           bar_to_atm=1.01325,//bar to atm
           Xmu = log(fug/(200*bar_to_atm))*r*450,
           k_in = 1.38064852 * pow(10,-23),
           T = 450.0,//kelvin
           p = 200.0,//bar
           SIM_N = 28.16583,//molecules
           SIM_V = 8.0* pow(10,-27),//meters cubed
           mass = 6.644 * pow(10,-26),//kilograms per molecule
           k_out = 0.008314462175,
           h = 6.6261 * pow(10,-34),//J*s
           R = 8.314459848,//m3Pa/Kmol
           avogadro = 6.022 * pow(10.0,23.0),
           bar_to_pascal = 100000.0,
           ideal_mu = -k_out*T*log((SIM_V/ SIM_N) * pow(((10.0*M_PI*mass*k_in*T)/(3.0*h*h)),(3.0/2.0))),
           sim_mu = -k_out*T*log(((R*T)/(p*bar_to_pascal))/avogadro * pow(((10.0*M_PI*mass*k_in*T)/(3.0*h*h)),(3.0/2.0))),
           sim_fug = exp(-(sim_mu-ideal_mu)/(k_out*T)) * (200 * bar_to_atm);
    printf("EQUATION OF STATE FUGACITY    =    %lf\n",fug);
    printf("SIMULATION FUGACITY           =    %lf\n",sim_fug);
    printf("SIMULATION V/N                =    %.12e\n",SIM_V/SIM_N);
    printf("IDEAL RT/P                    =    %.12e\n",((R*T)/(p*bar_to_pascal))/avogadro);
    printf("EQUATION OF STATE EXCESS MU   =   %lf\n",Xmu/1000.0);//kJ per mole
    printf("IDEAL MU                      =   %lf\n",ideal_mu);
    printf("TOTAL SIMULATION MU           =   %lf",sim_mu);
    return 0;
}
