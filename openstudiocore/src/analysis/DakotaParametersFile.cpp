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

#include "DakotaParametersFile.hpp"
#include "DakotaParametersFile_Impl.hpp"
#include "AnalysisEnums.hpp"
#include "UncertaintyDescription.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/filter/newline.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;

namespace openstudio {
namespace analysis {

namespace detail {

  DakotaParametersFile_Impl::DakotaParametersFile_Impl(std::istream& is) 
  {
    bool ok = load(is);
    if (!ok) {
      LOG_AND_THROW("Unable to load DakotaParametersFile.");
    }
  }

  int DakotaParametersFile_Impl::numVariables() const {
    return numContinuousDesignVariables() + numDiscreteDesignIntegerVariables();
  }

  int DakotaParametersFile_Impl::numContinuousDesignVariables() const {
    return m_continuousDesignVariableValues.size();
  }

  int DakotaParametersFile_Impl::numDiscreteDesignIntegerVariables() const {
    return m_discreteDesignIntegerVariableValues.size();
  }

  std::vector<double> DakotaParametersFile_Impl::continuousDesignVariableValues() const {
    return m_continuousDesignVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainNormalVariableValues() const {
    return m_uncertainNormalVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainLognormalVariableValues() const {
    return m_uncertainLognormalVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainUniformVariableValues() const {
    return m_uncertainUniformVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainLoguniformVariableValues() const {
    return m_uncertainLoguniformVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainTriangularVariableValues() const {
    return m_uncertainTriangularVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainExponentialVariableValues() const {
    return m_uncertainExponentialVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainBetaVariableValues() const {
    return m_uncertainBetaVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainGammaVariableValues() const {
    return m_uncertainGammaVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainGumbelVariableValues() const {
    return m_uncertainGumbelVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainFrechetVariableValues() const {
    return m_uncertainFrechetVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainWeibullVariableValues() const {
    return m_uncertainWeibullVariableValues;
  }

  std::vector<double> DakotaParametersFile_Impl::uncertainHistogramBinVariableValues() const {
    return m_uncertainHistogramBinVariableValues;
  }

  std::vector<int> DakotaParametersFile_Impl::discreteDesignIntegerVariableValues() const {
    return m_discreteDesignIntegerVariableValues;
  }

  std::vector<int> DakotaParametersFile_Impl::uncertainPoissonVariableValues() const {
    return m_uncertainPoissonVariableValues;
  }

  std::vector<int> DakotaParametersFile_Impl::uncertainBinomialVariableValues() const {
    return m_uncertainBinomialVariableValues;
  }

  std::vector<int> DakotaParametersFile_Impl::uncertainNegativeBinomialVariableValues() const {
    return m_uncertainNegativeBinomialVariableValues;
  }

  std::vector<int> DakotaParametersFile_Impl::uncertainGeometricVariableValues() const {
    return m_uncertainGeometricVariableValues;
  }

  std::vector<int> DakotaParametersFile_Impl::uncertainHypergeometricVariableValues() const {
    return m_uncertainHypergeometricVariableValues;
  }

  std::vector<int> DakotaParametersFile_Impl::uncertainHistogramPointVariableValues() const {
    return m_uncertainHistogramPointVariableValues;
  }

  int DakotaParametersFile_Impl::numFunctions() const {
    return m_functionASVs.size();
  }

  /** Return the string description of function i, index starting at 0. */
  DakotaFunctionType DakotaParametersFile_Impl::getFunctionType(int i) const {
    return m_functionASVs[i].first;
  }

  /** Returns true if Dakota expects to receive the value of function i, index starting at 0. */
  bool DakotaParametersFile_Impl::getFunctionValueRequired(int i) const {
    int asv = m_functionASVs[i].second;
    if ((asv == 1) || (asv == 3) || (asv == 5) || (asv == 7)) {
      return true;
    }
    return false;
  }

  bool DakotaParametersFile_Impl::load(std::istream& is) {
    std::string line;
    std::string temp;
    boost::smatch matches;
    bool ok;
    // handle all line endings
    boost::iostreams::filtering_istream filt;
    filt.push(boost::iostreams::newline_filter(boost::iostreams::newline::posix));
    filt.push(is);

    // parse variables
    getline(filt,line); trim(line);
    ok = boost::regex_match(line,matches,variablesRegex());
    if (!ok) {
      LOG(Error,"Unexpected start of Dakota parameters file. Was expecting match to boost::regex '" << 
          variablesRegex() << "', but first line is: " << std::endl << line);
      return false;
    }
    temp = std::string(matches[1].first,matches[1].second);
    int nvar = lexical_cast<int>(temp);
    int ncdv(0);
    int nddiv(0);
    int nuv(0), lnuv(0), uuv(0), luuv(0), tuv(0), euv(0), buv(0), gauv(0), guuv(0), fuv(0), wuv(0), hbuv(0);
    int puv(0), biuv(0), nbuv(0), geuv(0), hguv(0), hpuv(0);

    for (int i = 0; i < nvar; ++i) {
      getline(filt,line); trim(line);
      if (boost::regex_match(line,matches,continuousDesignVariableRegex())) {
        ++ncdv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_continuousDesignVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int icdv = boost::lexical_cast<int>(temp);
        if (icdv != ncdv) {
          LOG(Debug,"Expecting variable " << ncdv << ", but got variable " << icdv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainNormalVariableRegex())) {
        ++nuv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainNormalVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != nuv) {
          LOG(Debug,"Expecting variable " << nuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainLognormalVariableRegex())) {
        ++lnuv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainLognormalVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != lnuv) {
          LOG(Debug,"Expecting variable " << lnuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainUniformVariableRegex())) {
        ++uuv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainUniformVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != uuv) {
          LOG(Debug,"Expecting variable " << uuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainLoguniformVariableRegex())) {
        ++luuv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainLoguniformVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != luuv) {
          LOG(Debug,"Expecting variable " << luuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainTriangularVariableRegex())) {
        ++tuv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainTriangularVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != tuv) {
          LOG(Debug,"Expecting variable " << tuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainExponentialVariableRegex())) {
        ++euv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainExponentialVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != euv) {
          LOG(Debug,"Expecting variable " << euv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainBetaVariableRegex())) {
        ++buv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainBetaVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != buv) {
          LOG(Debug,"Expecting variable " << buv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainGammaVariableRegex())) {
        ++gauv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainGammaVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != gauv) {
          LOG(Debug,"Expecting variable " << gauv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainGumbelVariableRegex())) {
        ++guuv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainGumbelVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != guuv) {
          LOG(Debug,"Expecting variable " << guuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainFrechetVariableRegex())) {
        ++fuv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainFrechetVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != fuv) {
          LOG(Debug,"Expecting variable " << fuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainWeibullVariableRegex())) {
        ++wuv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainWeibullVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != wuv) {
          LOG(Debug,"Expecting variable " << wuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainHistogramBinVariableRegex())) {
        ++hbuv;
        temp = std::string(matches[1].first,matches[1].second);
        double dval = boost::lexical_cast<double>(temp);
        m_uncertainHistogramBinVariableValues.push_back(dval);
        temp = std::string(matches[2].first,matches[2].second);  
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != hbuv) {
          LOG(Debug,"Expecting variable " << hbuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,discreteDesignIntegerVariableRegex())) {
        ++nddiv;
        temp = std::string(matches[1].first,matches[1].second);
        double temp2 = boost::lexical_cast<double>(temp);
        int ival = int(temp2 + 0.5);
        m_discreteDesignIntegerVariableValues.push_back(ival);
        temp = std::string(matches[2].first,matches[2].second); 
        int iddiv = boost::lexical_cast<int>(temp);
        if (iddiv != nddiv) {
          LOG(Debug,"Expecting variable " << nddiv << ", but got variable " << iddiv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainPoissonVariableRegex())) {
        ++puv;
        temp = std::string(matches[1].first,matches[1].second);
        double temp2 = boost::lexical_cast<double>(temp);
        int ival = int(temp2 + 0.5);
        m_uncertainPoissonVariableValues.push_back(ival);
        temp = std::string(matches[2].first,matches[2].second); 
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != puv) {
          LOG(Debug,"Expecting variable " << puv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainBinomialVariableRegex())) {
        ++biuv;
        temp = std::string(matches[1].first,matches[1].second);
        double temp2 = boost::lexical_cast<double>(temp);
        int ival = int(temp2 + 0.5);
        m_uncertainBinomialVariableValues.push_back(ival);
        temp = std::string(matches[2].first,matches[2].second); 
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != biuv) {
          LOG(Debug,"Expecting variable " << biuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainNegativeBinomialVariableRegex())) {
        ++nbuv;
        temp = std::string(matches[1].first,matches[1].second);
        double temp2 = boost::lexical_cast<double>(temp);
        int ival = int(temp2 + 0.5);
        m_uncertainNegativeBinomialVariableValues.push_back(ival);
        temp = std::string(matches[2].first,matches[2].second); 
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != nbuv) {
          LOG(Debug,"Expecting variable " << nbuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainGeometricVariableRegex())) {
        ++geuv;
        temp = std::string(matches[1].first,matches[1].second);
        double temp2 = boost::lexical_cast<double>(temp);
        int ival = int(temp2 + 0.5);
        m_uncertainGeometricVariableValues.push_back(ival);
        temp = std::string(matches[2].first,matches[2].second); 
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != geuv) {
          LOG(Debug,"Expecting variable " << geuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainHypergeometricVariableRegex())) {
        ++geuv;
        temp = std::string(matches[1].first,matches[1].second);
        double temp2 = boost::lexical_cast<double>(temp);
        int ival = int(temp2 + 0.5);
        m_uncertainHypergeometricVariableValues.push_back(ival);
        temp = std::string(matches[2].first,matches[2].second); 
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != geuv) {
          LOG(Debug,"Expecting variable " << geuv << ", but got variable " << iuv << ".");
        }
      }
      else if (boost::regex_match(line,matches,uncertainHistogramPointVariableRegex())) {
        ++hpuv;
        temp = std::string(matches[1].first,matches[1].second);
        double temp2 = boost::lexical_cast<double>(temp);
        int ival = int(temp2 + 0.5);
        m_uncertainHistogramPointVariableValues.push_back(ival);
        temp = std::string(matches[2].first,matches[2].second); 
        int iuv = boost::lexical_cast<int>(temp);
        if (iuv != hpuv) {
          LOG(Debug,"Expecting variable " << hpuv << ", but got variable " << iuv << ".");
        }
      }
      else {
        LOG(Error,"Parsing bug or unknown variable type encountered. Expecting a line like: '" 
            << continuousDesignVariableRegex() << "' or '" << discreteDesignIntegerVariableRegex()
            << "', but actual line is: " << std::endl << line);
        return false;
      }
    }

    OS_ASSERT(m_continuousDesignVariableValues.size() == unsigned(ncdv));
    OS_ASSERT(m_uncertainNormalVariableValues.size() == unsigned(nuv));
    OS_ASSERT(m_uncertainLognormalVariableValues.size() == unsigned(lnuv));
    OS_ASSERT(m_uncertainUniformVariableValues.size() == unsigned(uuv));
    OS_ASSERT(m_uncertainLoguniformVariableValues.size() == unsigned(luuv));
    OS_ASSERT(m_uncertainTriangularVariableValues.size() == unsigned(tuv));
    OS_ASSERT(m_uncertainExponentialVariableValues.size() == unsigned(euv));
    OS_ASSERT(m_uncertainBetaVariableValues.size() == unsigned(buv));
    OS_ASSERT(m_uncertainGammaVariableValues.size() == unsigned(gauv));
    OS_ASSERT(m_uncertainGumbelVariableValues.size() == unsigned(guuv));
    OS_ASSERT(m_uncertainFrechetVariableValues.size() == unsigned(fuv));
    OS_ASSERT(m_uncertainWeibullVariableValues.size() == unsigned(wuv));
    OS_ASSERT(m_uncertainHistogramBinVariableValues.size() == unsigned(hbuv));
    OS_ASSERT(m_discreteDesignIntegerVariableValues.size() == unsigned(nddiv));
    OS_ASSERT(m_uncertainPoissonVariableValues.size() == unsigned(puv));
    OS_ASSERT(m_uncertainBinomialVariableValues.size() == unsigned(biuv));
    OS_ASSERT(m_uncertainNegativeBinomialVariableValues.size() == unsigned(nbuv));
    OS_ASSERT(m_uncertainGeometricVariableValues.size() == unsigned(geuv));
    OS_ASSERT(m_uncertainHypergeometricVariableValues.size() == unsigned(hguv));
    OS_ASSERT(m_uncertainHistogramPointVariableValues.size() == unsigned(hpuv));
    OS_ASSERT(ncdv + nuv + lnuv + uuv + luuv + tuv + euv + buv + gauv + guuv + fuv + wuv + hbuv
      + nddiv + puv + biuv + nbuv + geuv + hguv + hpuv == nvar);

    // parse functions
    getline(filt,line); trim(line);
    ok = boost::regex_match(line,matches,functionsRegex());
    if (!ok) {
      LOG(Error,"Unexpected Dakota parameters file format. Was expecting match to boost::regex '" << 
          functionsRegex() << "', but line is: " << std::endl << line);
      return false;
    }
    temp = std::string(matches[1].first,matches[1].second);
    int nfun = lexical_cast<int>(temp);

    for (int i = 0; i < nfun; ++i) {
      getline(filt,line); trim(line);
      ok = boost::regex_match(line,matches,functionASVRegex());
      if (!ok) {
        LOG(Error,"Unexpected Dakota parameters file format. Was expecting to match boost::regex '" <<
            functionASVRegex() << "', but line is: " << std::endl << line);
        return false;
      }
      temp = std::string(matches[1].first,matches[1].second);
      int asv = boost::lexical_cast<int>(temp);
      temp = std::string(matches[2].first,matches[2].second);
      int iasv = boost::lexical_cast<int>(temp);
      if (iasv != i + 1) {
        LOG(Debug,"Expecting ASV_" << i + 1 << ", but got ASV_" << iasv << ".");
      }
      temp = std::string(matches[3].first,matches[3].second);
      std::pair<DakotaFunctionType,int> funInfo(DakotaFunctionType(temp),asv);
      m_functionASVs.push_back(funInfo);
      // ignoring matches[4] for now
    }

    OS_ASSERT(m_functionASVs.size() == unsigned(nfun));

    return true;
  }

  boost::regex DakotaParametersFile_Impl::variablesRegex() const {
    return boost::regex("(\\d+) variables");
  }

  boost::regex DakotaParametersFile_Impl::continuousDesignVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) cdv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainNormalVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) nuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainLognormalVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) lnuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainUniformVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) uuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainLoguniformVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) luuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainTriangularVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) tuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainExponentialVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) euv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainBetaVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) buv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainGammaVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) gauv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainGumbelVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) guuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainFrechetVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) fuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainWeibullVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) wuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainHistogramBinVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) hbuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::discreteDesignIntegerVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) ddiv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainPoissonVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) puv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainBinomialVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) biuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainNegativeBinomialVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) nbuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainGeometricVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) geuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainHypergeometricVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) hguv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::uncertainHistogramPointVariableRegex() const {
    return boost::regex("(-?\\d+.\\d+e\\+?-?\\d+) hpuv_(\\d+)");
  }

  boost::regex DakotaParametersFile_Impl::functionsRegex() const {
    return boost::regex("(\\d+) functions");
  }

  boost::regex DakotaParametersFile_Impl::functionASVRegex() const {
    return boost::regex("([0-7]) ASV_(\\d+):(\\l+_fn)_(\\d+)");
  }

} // detail

boost::optional<DakotaParametersFile> DakotaParametersFile::load(const openstudio::path& p) {
  OptionalDakotaParametersFile result;

  try {
    boost::filesystem::ifstream is(p);
    std::shared_ptr<detail::DakotaParametersFile_Impl> impl(new detail::DakotaParametersFile_Impl(is));
    result = DakotaParametersFile(impl);
  }
  catch (...) {}

  return result;
}

int DakotaParametersFile::numVariables() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->numVariables();
}

int DakotaParametersFile::numContinuousDesignVariables() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->numContinuousDesignVariables();
}

int DakotaParametersFile::numDiscreteDesignIntegerVariables() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->numDiscreteDesignIntegerVariables();
}

std::vector<double> DakotaParametersFile::continuousDesignVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->continuousDesignVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainNormalVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainNormalVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainLognormalVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainLognormalVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainUniformVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainUniformVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainLoguniformVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainLoguniformVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainTriangularVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainTriangularVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainExponentialVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainExponentialVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainBetaVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainBetaVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainGammaVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainGammaVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainGumbelVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainGumbelVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainFrechetVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainFrechetVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainWeibullVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainWeibullVariableValues();
}

std::vector<double> DakotaParametersFile::uncertainHistogramBinVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainHistogramBinVariableValues();
}

std::vector<int> DakotaParametersFile::discreteDesignIntegerVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->discreteDesignIntegerVariableValues();
}

std::vector<int> DakotaParametersFile::uncertainPoissonVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainPoissonVariableValues();
}

std::vector<int> DakotaParametersFile::uncertainBinomialVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainBinomialVariableValues();
}

std::vector<int> DakotaParametersFile::uncertainNegativeBinomialVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainNegativeBinomialVariableValues();
}

std::vector<int> DakotaParametersFile::uncertainGeometricVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainGeometricVariableValues();
}

std::vector<int> DakotaParametersFile::uncertainHypergeometricVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainHypergeometricVariableValues();
}

std::vector<int> DakotaParametersFile::uncertainHistogramPointVariableValues() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->uncertainHistogramPointVariableValues();
}

int DakotaParametersFile::numFunctions() const {
  return getImpl<detail::DakotaParametersFile_Impl>()->numFunctions();
}

DakotaFunctionType DakotaParametersFile::getFunctionType(int i) const {
  return getImpl<detail::DakotaParametersFile_Impl>()->getFunctionType(i);
}

bool DakotaParametersFile::getFunctionValueRequired(int i) const {
  return getImpl<detail::DakotaParametersFile_Impl>()->getFunctionValueRequired(i);
}

/// @cond
DakotaParametersFile::DakotaParametersFile(std::shared_ptr<detail::DakotaParametersFile_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

std::vector<QVariant> getUncertainVariableValues(const DakotaParametersFile& params,
                                                 const UncertaintyDescriptionType& type)
{
  std::vector<QVariant> result;
  UncertaintyDescriptionTypeVector discreteTypes = UncertaintyDescription::validTypes(VariableValueType::Discrete);
  if (std::find(discreteTypes.begin(),discreteTypes.end(),type) != discreteTypes.end()) {
    IntVector values = getDiscreteUncertainVariableValues(params,type);
    for (int value : values) {
      result.push_back(QVariant(value));
    }
  }
  else {
    DoubleVector values = getContinuousUncertainVariableValues(params,type);
    for (double value : values) {
      result.push_back(QVariant(value));
    }
  }
  return result;
}

std::vector<double> getContinuousUncertainVariableValues(const DakotaParametersFile& params,
                                                         const UncertaintyDescriptionType& type)
{
  DoubleVector result;
  switch (type.value()) {
    case UncertaintyDescriptionType::normal_uncertain :
      result = params.uncertainNormalVariableValues();
     break;
    case UncertaintyDescriptionType::lognormal_uncertain :
      result = params.uncertainLognormalVariableValues();
     break;
    case UncertaintyDescriptionType::uniform_uncertain :
      result = params.uncertainUniformVariableValues();
     break;
    case UncertaintyDescriptionType::loguniform_uncertain :
      result = params.uncertainLoguniformVariableValues();
     break;
    case UncertaintyDescriptionType::triangular_uncertain :
      result = params.uncertainTriangularVariableValues();
     break;
    case UncertaintyDescriptionType::exponential_uncertain :
      result = params.uncertainExponentialVariableValues();
     break;
    case UncertaintyDescriptionType::beta_uncertain :
      result = params.uncertainBetaVariableValues();
     break;
    case UncertaintyDescriptionType::gamma_uncertain :
      result = params.uncertainGammaVariableValues();
     break;
    case UncertaintyDescriptionType::gumbel_uncertain :
      result = params.uncertainGumbelVariableValues();
     break;
    case UncertaintyDescriptionType::frechet_uncertain :
      result = params.uncertainFrechetVariableValues();
     break;
    case UncertaintyDescriptionType::weibull_uncertain :
      result = params.uncertainWeibullVariableValues();
     break;
    case UncertaintyDescriptionType::histogram_bin_uncertain :
      result = params.uncertainHistogramBinVariableValues();
     break;
    default :
      LOG_FREE_AND_THROW("openstudio.analysis.DakotaParametersFile",type.valueDescription() <<
                         " is not a valid UncertaintyDescriptionType for continuous variables.");
  }
  return result;
}

std::vector<int> getDiscreteUncertainVariableValues(const DakotaParametersFile& params,
                                                    const UncertaintyDescriptionType& type)
{
  IntVector result;
  switch (type.value()) {
    case UncertaintyDescriptionType::poisson_uncertain :
      result = params.uncertainPoissonVariableValues();
     break;
    case UncertaintyDescriptionType::binomial_uncertain :
      result = params.uncertainBinomialVariableValues();
     break;
    case UncertaintyDescriptionType::negative_binomial_uncertain :
      result = params.uncertainNegativeBinomialVariableValues();
     break;
    case UncertaintyDescriptionType::geometric_uncertain :
      result = params.uncertainGeometricVariableValues();
     break;
    case UncertaintyDescriptionType::hypergeometric_uncertain :
      result = params.uncertainHypergeometricVariableValues();
     break;
    case UncertaintyDescriptionType::histogram_point_uncertain :
      result = params.uncertainHistogramPointVariableValues();
     break;
    default :
      LOG_FREE_AND_THROW("openstudio.analysis.DakotaParametersFile",type.valueDescription() <<
                         " is not a valid UncertaintyDescriptionType for discrete variables.");
  }
  return result;
}

} // analysis
} // openstudio

