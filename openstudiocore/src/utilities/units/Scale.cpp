/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

  /** Static constant defining SI prefix for 10^5. */
  UTILITIES_API const Scale& hectokilo()
  {
    static Scale hk = {"_hk", "hectokilo",5,1.0E5};
    return hk;
  }

  /** Static constant defining SI prefix for 10^4. */
  const Scale& myria()
  {
    static Scale k = {"_ma", "myria",4,1.0E4};
    return k;
  }

  /** Static constant defining SI prefix for 10^3. */
  const Scale& kilo()
  {
    static Scale k = {"k", "kilo",3,1.0E3};
    return k;
  }

  /** Static constant defining SI prefix for 10^2. */
  UTILITIES_API const Scale& hecto()
  {
    static Scale h = {"_h", "hecto",2,1.0E2};
    return h;
  }

  /** Static constant defining SI prefix for 10^1. */
  UTILITIES_API const Scale& deka()
  {
    static Scale da = {"_da", "deka",1,1.0E1};
    return da;
  }

  /** Static constant defining no scale (1.0). */
  const Scale& one()
  {
    static Scale o = {"","",0,1.0};
    return o;
  }

  /** Static constant defining SI prefix for 10^{-1} */
  const Scale& deci()
  {
    static Scale d = {"_d", "deci",-1,1.0E-1};
    return d;
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

  /** Static constant defining SI prefix for 10^{-4} */
  const Scale& decimilli()
  {
    static Scale dm = {"_dm", "decimilli",-4,1.0E-4};
    return dm;
  }

  /** Static constant defining SI prefix for 10^{-5} */
  const Scale& centimilli()
  {
    static Scale cm = {"_cm", "centimilli",-5,1.0E-5};
    return cm;
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
