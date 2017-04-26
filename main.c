#include "fugacity.h"

double co2_fugacity(double temperature, double pressure) {

	double Z, A, B, aa, bb, Tc, Pc, Tr;
	double alpha,alpha2, w, R, Q, X, j,k,l;
	double theta,  Q3;
	double uu,U,V,root1,root2,root3, stuff1, stuff2, stuff3; //, answer;  (unused variable)
	double f1, f2, f3, f4, fugacity, lnfoverp;
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
	if((X*X)<(Q*Q*Q)){    /* THREE REAL ROOTS  */
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
			
	} else {   /* ONLY ONE real root */
		
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
	f1=(Z-1.0)-log(Z-B);
	f2=A/(2.0*sqrt(2.0)*B);
	f3=Z+(1.0+sqrt(2.0))*B;
	f4=Z+(1.0-sqrt(2.0))*B;
	lnfoverp=f1-f2*log(f3/f4);
	fugacity=exp(lnfoverp)*pressure;
	
	return(fugacity);

}

int main()
{
    double r = 8.3144598,//j per kelvinmoles
           fug = co2_fugacity(450,200), 
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
