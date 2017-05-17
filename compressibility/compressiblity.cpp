/*code in this file is from the USF Space Lab's MPMC software package
 *with some modifications by me to suit the purposes of this project.
 */

/* reads in temperature in K, and pressure (of the ideal gas in the reservoir) in atm
 * return the CO2 fugacity via the Peng-Robinson equation of state
 * else return 0.0 on error - I don't have an error statement
 * units are  atm, K,  */

#include <math.h>

double get_state_compressibility(double temperature, double pressure)
{

    double Z, A, B, aa, bb, Tc, Pc, Tr;
    double alpha,alpha2, w, R, Q, X, j,k,l;
    double theta,  Q3;
    double uu,U,V,root1,root2,root3, stuff1, stuff2, stuff3; //, answer;  (unused variable)
    double pi=acos(-1.0);

    /*Peng Robinson variables and equations for CO2 units K,atm, L, mole*/
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

    return(Z);

}
