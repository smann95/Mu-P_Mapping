//
// Created by Luciano Laratelli on 06/06/2017.
//

#include "compressibility.h"
#include <boost/algorithm/string.hpp>

void get_peng_robinson_constants(run &current)
{
    //constants are taken from J.R.Elliot and Carl T. Lira's
    //"Introductory chemical thermodynamics (2e)"
    if(boost::iequals(atom_type,"co2"))//case-insensitive string comparison
    {
        current.Tc = 304.2;
        current.Pc = 7.382;//TODO: THESE ARE IN mPA and need to be converted
        current.w =  0.228;
    }
    else if(boost::iequals(atom_type,"n2"))
    {
        current.Tc = 126.1;
        current.Pc = 3.394;
        current.w = 0.040;
    }
    else if(boost::iequals(atom_type,"h2"))
    {
        current.Tc = 33.3;
        current.Pc = 1.297;
        current.w = -0.215;
    }
    else if(boost::iequals(atom_type,"ch4"))
    {
        current.Tc = 190.6;
        current.Pc = 4.604;
        current.w = 0.011;
    }
    else if(boost::iequals(atom_type,"he"))
    {
        current.Tc = 5.2;
        current.Pc = 0.228;
        current.w = 0.000;
    }
    else if(boost::iequals(atom_type,"ne"))
    {
        current.Tc = 44.4;
        current.Pc = 2.653;
        current.w = -0.041;
    }
    else if(boost::iequals(atom_type,"xe"))
    {
        current.Tc = 289.7;
        current.Pc = 5.840;
        current.w = 0.012;
    }
    else if(boost::iequals(atom_type,"kr"))
    {
        current.Tc = 209.4;
        current.Pc = 5.502;
        current.w = 0.001;
    }
    else if(boost::iequals(atom_type,"ar"))
    {
        current.Tc = 150.9;
        current.Pc = 4.898;
        current.w = -0.004;
    }
}