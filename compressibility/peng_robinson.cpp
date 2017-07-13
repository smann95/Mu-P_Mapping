//
// Created by Luciano Laratelli on 06/06/2017.
//

#include "compressibility.h"
#include <boost/algorithm/string.hpp>

void get_peng_robinson_constants(run &current)
{
    std::string atom_type = current.atom_type;
    //constants are taken from J.R.Elliot and Carl T. Lira's
    //"Introductory chemical thermodynamics (2e)"
    if(boost::iequals(atom_type,"co2"))//case-insensitive string comparison
    {
        current.Tc = 304.2;
        current.Pc = 72.854676;
        current.w =  0.228;
    }
    else if(boost::iequals(atom_type,"n2"))
    {
        current.Tc = 126.1;
        current.Pc = 33.496176;
        current.w = 0.040;
    }
    else if(boost::iequals(atom_type,"h2"))
    {
        current.Tc = 33.3;
        current.Pc = 12.800395;
        current.w = -0.215;
    }
    else if(boost::iequals(atom_type,"ch4"))
    {
        current.Tc = 190.6;
        current.Pc = 45.437947;
        current.w = 0.011;
    }
    else if(boost::iequals(atom_type,"he"))
    {
        current.Tc = 5.2;
        current.Pc = 2.250185;
        current.w = 0.000;
    }
    else if(boost::iequals(atom_type,"ne"))
    {
        current.Tc = 44.4;
        current.Pc = 26.183074;
        current.w = -0.041;
    }
    else if(boost::iequals(atom_type,"xe"))
    {
        current.Tc = 289.7;
        current.Pc = 57.63632;
        current.w = 0.012;
    }
    else if(boost::iequals(atom_type,"kr"))
    {
        current.Tc = 209.4;
        current.Pc = 54.300518;
        current.w = 0.001;
    }
    else if(boost::iequals(atom_type,"ar"))
    {
        current.Tc = 150.9;
        current.Pc = 48.339502;
        current.w = -0.004;
    }
}


/*Stolen whole-heartedly from MPMC */
double solve_peng_robinson_for_compressibility(double temperature, double pressure, run some_run)
{
    double Z, A, B, aa, bb, Tc, Pc, Tr;
    double alpha, alpha2, w, R, Q, X, j, k, l;
    double theta, Q3;
    double uu, U, V, root1, root2, root3, stuff1, stuff2, stuff3;
    double pi=acos(-1.0);

    /*Peng Robinson variables  units K,atm, L, mole*/
    Tc = some_run.Tc;
    Pc = some_run.Pc;
    w = some_run.w;
    R = 0.08206;   /* gas constant atmL/moleK */

    aa = (0.45724*R*R*Tc*Tc) / Pc;
    bb = (0.07780*R*Tc) / Pc;
    Tr = temperature / Tc;
    stuff1 = 0.37464 + 1.54226*w - 0.26992*w*w;
    stuff2=1.0-sqrt(Tr);
    alpha=1.0+stuff1*stuff2;
    alpha2=alpha*alpha;
    A=alpha2*aa*pressure/(R*R*temperature*temperature);
    B=bb*pressure/(R*temperature);

    /* solving a cubic equation part */
    j=-1.0*(1-B);
    k=A-3.0*B*B-2.0*B;
    l= -1*(A*B- B*B -B*B*B);
    Q=(j*j-3.0*k)/9.0;
    X=(2.0*j*j*j -9.0*j*k+27.0*l)/54.0;
    Q3=Q*Q*Q;

    /* Need to check X^2 < Q^3 */
    if((X*X)<(Q*Q*Q))
    {    /* THREE REAL ROOTS  */
        theta=acos((X/sqrt(Q3)));
        root1=-2.0*sqrt(Q)*cos(theta/3.0)-j/3.0;
        root2=-2.0*sqrt(Q)*cos((theta+2.0*pi)/3.0)-j/3.0;
        root3=-2.0*sqrt(Q)*cos((theta-2.0*pi)/3.0)-j/3.0;

        /*Choose the root closest to 1, which is "ideal gas law" */
        if((1.0-root1)<(1.0-root2) && (1.0-root1)<(1.0-root3))
            Z=root1;
        else if((1.0-root2)<(1.0-root3) && (1.0-root2)<(1.0-root1))
            Z=root2;
        else
            Z=root3;
    }
    else
    {   /* ONLY ONE real root */
        stuff3= X*X-Q*Q*Q;
        uu=X-sqrt(stuff3);
        /*Power function must have uu a positive number*/
        if(uu<0.0)
            uu=-1.0*uu;
        U=pow(uu,(1.0/3.0));
        V=Q/U;
        root1=U+V-j/3.0;
        Z=root1;
    }
    return Z;
}

double solve_peng_robinson_for_fugacity(double temperature, double pressure, run some_run)
{
    double Z, A, B, aa, bb, Tc, Pc, Tr;
    double alpha, alpha2, w, R, Q, X, j, k, l;
    double theta, Q3;
    double uu, U, V, root1, root2, root3, stuff1, stuff2, stuff3;
    double pi=acos(-1.0);

    /*Peng Robinson variables  units K,atm, L, mole*/
    Tc = some_run.Tc;
    Pc = some_run.Pc;
    w = some_run.w;
    R = 0.08206;   /* gas constant atmL/moleK */

    aa = (0.45724*R*R*Tc*Tc) / Pc;
    bb = (0.07780*R*Tc) / Pc;
    Tr = temperature / Tc;
    stuff1 = 0.37464 + 1.54226*w - 0.26992*w*w;
    stuff2=1.0-sqrt(Tr);
    alpha=1.0+stuff1*stuff2;
    alpha2=alpha*alpha;
    A=alpha2*aa*pressure/(R*R*temperature*temperature);
    B=bb*pressure/(R*temperature);

    /* solving a cubic equation part */
    j=-1.0*(1-B);
    k=A-3.0*B*B-2.0*B;
    l= -1*(A*B- B*B -B*B*B);
    Q=(j*j-3.0*k)/9.0;
    X=(2.0*j*j*j -9.0*j*k+27.0*l)/54.0;
    Q3=Q*Q*Q;

    /* Need to check X^2 < Q^3 */
    if((X*X)<(Q*Q*Q))
    {    /* THREE REAL ROOTS  */
        theta=acos((X/sqrt(Q3)));
        root1=-2.0*sqrt(Q)*cos(theta/3.0)-j/3.0;
        root2=-2.0*sqrt(Q)*cos((theta+2.0*pi)/3.0)-j/3.0;
        root3=-2.0*sqrt(Q)*cos((theta-2.0*pi)/3.0)-j/3.0;

        /*Choose the root closest to 1, which is "ideal gas law" */
        if((1.0-root1)<(1.0-root2) && (1.0-root1)<(1.0-root3))
            Z=root1;
        else if((1.0-root2)<(1.0-root3) && (1.0-root2)<(1.0-root1))
            Z=root2;
        else
            Z=root3;
    }
    else
    {   /* ONLY ONE real root */
        stuff3= X*X-Q*Q*Q;
        uu=X-sqrt(stuff3);
        /*Power function must have uu a positive number*/
        if(uu<0.0)
            uu=-1.0*uu;
        U=pow(uu,(1.0/3.0));
        V=Q/U;
        root1=U+V-j/3.0;
        Z=root1;
    }
    /* using Z calculate the fugacity */
    double f1, f2, f3, f4, lnfoverp, fugacity;

    f1=(Z-1.0)-log(Z-B);
    f2=A/(2.0*sqrt(2.0)*B);
    f3=Z+(1.0+sqrt(2.0))*B;
    f4=Z+(1.0-sqrt(2.0))*B;
    lnfoverp=f1-f2*log(f3/f4);
    fugacity=exp(lnfoverp)*pressure;

    return(fugacity);
}
