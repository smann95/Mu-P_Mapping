//
// Created by Luciano Laratelli on 17/05/2017.
//

#include "compressibility.h"

//takes pressure in Pa and volume in m^3
double get_simulation_compressibility(double temperature, double pressure, double volume)
{
    double num = 64.0/AVOGADRO;//n is in moles
    return (pressure * volume) / (num * GAS_CONSTANT * temperature);
}

/* this next function is incredibly horrific; I stole most code
 * directly from MPMC and glued some stuff onto it so it would work with my code
 * The awful variable names and magic numbers are thankfully not my doing
 * I'm very sorry if you've made it here looking for answers */

double get_simulation_fugacity(double Z, double pressure,double temperature, std::string species)
{
    double A, B, aa, bb, Tc, Pc, Tr;
    double alpha,alpha2, w, R;
    double stuff1, stuff2; //, answer;  (unused variable)
    double f1, f2, f3, f4, simulation_fugacity, lnfoverp;

    if(species == "co2")
    {
        /*Peng Robinson variables for CO2 units K,atm, L, mole*/
        Tc=304.12;   /*K  */
        Pc=73.74/1.01325;    /*bar to atm*/
        w=0.225;
        R=0.08206;   /* gas constant atmL/moleK */

        aa=0.45724*R*R*Tc*Tc/Pc;
        bb=0.07780*R*Tc/Pc;
        Tr=temperature/Tc;
        stuff1=0.37464+1.54226*w -0.26992*w*w;
        stuff2=1.0-sqrt(Tr);
        alpha=1.0+stuff1*stuff2;
        alpha2=alpha*alpha;
        A=alpha2*aa*pressure/(R*R*temperature*temperature);
        B=bb*pressure/(R*temperature);

    }
    else if(species == "n2")
    {
        //Peng robinson variables for N2 units K,atm, L, mole
        Tc = 126.192;   /* K */
        Pc = 33.514;    /* atm */
        w  = 0.037;
        R    = 0.08206;   /* gas constant atmL/moleK */

        aa = (0.45724*R*R*Tc*Tc) / Pc;
        bb = (0.07780*R*Tc) / Pc;
        Tr = temperature / Tc;
        stuff1 = 0.37464 + 1.54226*w - 0.26992*w*w;
        stuff2=1.0-sqrt(Tr);
        alpha=1.0+stuff1*stuff2;
        alpha2=alpha*alpha;
        A=alpha2*aa*pressure/(R*R*temperature*temperature);
        B=bb*pressure/(R*temperature);
    }
    else
    {
        std::cerr << "You shouldn't be getting this message."
                  << "How did you mess up this badly?"
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    f1=(Z-1.0)-log(Z-B);
    f2=A/(2.0*sqrt(2.0)*B);
    f3=Z+(1.0+sqrt(2.0))*B;
    f4=Z+(1.0-sqrt(2.0))*B;
    lnfoverp=f1-f2*log(f3/f4);
    simulation_fugacity=exp(lnfoverp)*pressure;

    return simulation_fugacity;
}
