/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "Scale.hpp"

namespace openstudio{

  std::ostream& operator<<(std::ostream& os,const Scale& s) {
    os << s.name << ", " << s.abbr << ", 10^";
    if (s.exponent < 0) {
      os << "{" << s.exponent << "}";
    }
    else {
      os << s.exponent;
    }
    os << " = " << s.value;

    return os;
  }

  bool operator==(const Scale& firstScale,const Scale& secondScale) {
    return ((firstScale.exponent == secondScale.exponent) && (firstScale.abbr == secondScale.abbr));
  }

  bool operator!=(const Scale& firstScale,const Scale& secondScale) {
    return !(firstScale == secondScale);
  }

  bool operator<(const Scale& firstScale,const Scale& secondScale) {
    return (firstScale.exponent < secondScale.exponent);
  }

  bool operator>(const Scale& firstScale,const Scale& secondScale) {
    return (firstScale.exponent > secondScale.exponent);
  }

  bool operator<=(const Scale& firstScale,const Scale& secondScale) {
    return (firstScale.exponent <= secondScale.exponent);
  }

  bool operator>=(const Scale& firstScale,const Scale& secondScale) {
    return (firstScale.exponent >= secondScale.exponent);
  }

  /** Static constant indicating that the scale was not found. To test for it,
   *  check that value == 0. */
  const Scale& notDefined()
  {
    static Scale ND = {"ND", "Not Defined",0,0.0};
    return ND;
  }

  /** Static constant defining SI prefix for 10^24. */
  const Scale& yotta() 
  {
    static Scale Y = {"Y", "yotta",24,1.0E24};
    return Y;
  }

  /** Static constant defining SI prefix for 10^21. */
  const Scale& zetta() 
  {
    static Scale Z = {"Z", "zetta",21,1.0E21};
    return Z;
  }

  /** Static constant defining SI prefix for 10^18. */
  const Scale& exa() 
  {
    static Scale E = {"E", "exa",18,1.0E18};
    return E;
  }

  /** Static constant defining SI prefix for 10^15. */
  const Scale& peta()
  {
    static Scale P = {"P", "peta",15,1.0E15};
    return P;
  }

  /** Static constant defining SI prefix for 10^12. */
  const Scale& tera()
  {
    static Scale T = {"T", "tera",12,1.0E12};
    return T;
  }

  /** Static constant defining SI prefix for 10^9. */
  const Scale& giga()
  {
    static Scale G = {"G", "giga",9,1.0E9};
    return G;
  }

  /** Static constant defining SI prefix for 10^6. */
  const Scale& mega()
  {
    static Scale M = {"M", "mega",6,1.0E6};
    return M;
  }

  /** Static constant defining SI prefix for 10^3. */
  const Scale& kilo()
  {
    static Scale k = {"k", "kilo",3,1.0E3};
    return k;
  }

  /** Static constant defining no scale (1.0). */
  const Scale& one()
  {
    static Scale o = {"","",0,1.0};
    return o;
  }

  /** Static constant defining SI prefix for 10^{-2} */
  const Scale& centi()
  {
    static Scale c = {"c", "centi",-2,1.0E-2};
    return c;
  }

  /** Static constant defining SI prefix for 10^{-3} */
  const Scale& milli()
  {
    static Scale m = {"m", "milli",-3,1.0E-3};
    return m;
  }

  /** Static constant defining SI prefix for 10^{-6} */
  const Scale& micro()
  {
    static Scale mu = {"\\mu", "micro",-6,1.0E-6};
    return mu;
  }

  /** Static constant defining SI prefix for 10^{-9} */
  const Scale& nano()
  {
    static Scale n = {"n", "nano",-9,1.0E-9};
    return n;
  }

  /** Static constant defining SI prefix for 10^{-12} */
  const Scale& pico() 
  {
    static Scale p = {"p", "pico",-12,1.0E-12};
    return p;
  }

  /** Static constant defining SI prefix for 10^{-15} */
  const Scale& femto()
  {
    static Scale f = {"f", "femto",-15,1.0E-15};
    return f;
  }

  /** Static constant defining SI prefix for 10^{-18) */
  const Scale& atto()
  {
    static Scale a = {"a", "atto",-18,1.0E-18};
    return a;
  }

  /** Static constant defining SI prefix for 10^{-21} */
  const Scale& zepto() 
  {
    static Scale z = {"z", "zepto",-21,1.0E-21};
    return z;
  }

  /** Static constant defining SI prefix for 10^{-24} */
  const Scale& yocto() 
  {
    static Scale y = {"y", "yocto",-24,1.0E-24};
    return y;
  }

} // openstudio
