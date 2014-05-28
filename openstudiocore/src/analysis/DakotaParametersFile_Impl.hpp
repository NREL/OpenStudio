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

#ifndef ANALYSIS_DAKOTAPARAMETERSFILE_IMPL_HPP
#define ANALYSIS_DAKOTAPARAMETERSFILE_IMPL_HPP

#include "AnalysisAPI.hpp"

#include "DakotaParametersFile.hpp"

#include <boost/regex.hpp>

namespace openstudio {
namespace analysis {

class DakotaParametersFile;

namespace detail {

  /** DakotaParametersFile_Impl is the implementation class for DakotaParametersFile. */
  class ANALYSIS_API DakotaParametersFile_Impl : public std::enable_shared_from_this<DakotaParametersFile_Impl> {
   public:
    /** @name Constructors and Destructors */
    //@{

    DakotaParametersFile_Impl(std::istream& is);

    //@}
    /** @name Getters */
    //@{
   
    int numVariables() const;

    int numContinuousDesignVariables() const;

    int numDiscreteDesignIntegerVariables() const;

    std::vector<double> continuousDesignVariableValues() const;

    std::vector<double> uncertainNormalVariableValues() const;
    
    std::vector<double> uncertainLognormalVariableValues() const;
    
    std::vector<double> uncertainUniformVariableValues() const;
    
    std::vector<double> uncertainLoguniformVariableValues() const;
    
    std::vector<double> uncertainTriangularVariableValues() const;
    
    std::vector<double> uncertainExponentialVariableValues() const;
    
    std::vector<double> uncertainBetaVariableValues() const;
    
    std::vector<double> uncertainGammaVariableValues() const;
    
    std::vector<double> uncertainGumbelVariableValues() const;
    
    std::vector<double> uncertainFrechetVariableValues() const;
    
    std::vector<double> uncertainWeibullVariableValues() const;
    
    std::vector<double> uncertainHistogramBinVariableValues() const;

    std::vector<int> discreteDesignIntegerVariableValues() const;

    std::vector<int> uncertainPoissonVariableValues() const;
    
    std::vector<int> uncertainBinomialVariableValues() const;
    
    std::vector<int> uncertainNegativeBinomialVariableValues() const;
    
    std::vector<int> uncertainGeometricVariableValues() const;
    
    std::vector<int> uncertainHypergeometricVariableValues() const;
    
    std::vector<int> uncertainHistogramPointVariableValues() const;

    int numFunctions() const;

    /** Return the string description of function i, index starting at 0. */
    DakotaFunctionType getFunctionType(int i) const;

    /** Returns true if Dakota expects to receive the value of function i, index starting at 0. */
    bool getFunctionValueRequired(int i) const;

    //@}
    /** @name Type Casting */
    //@{

    /** Get a public object that wraps this impl.*/
    template<typename T>
    T getPublicObject() const {
      T result(std::dynamic_pointer_cast<typename T::ImplType>(
                 std::const_pointer_cast<DakotaParametersFile_Impl>(shared_from_this())));
      return result;
    }

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.DakotaParametersFile");

    std::vector<double> m_continuousDesignVariableValues;
    std::vector<int> m_discreteDesignIntegerVariableValues;
    std::vector<double> m_uncertainNormalVariableValues;
    std::vector<double> m_uncertainLognormalVariableValues;
    std::vector<double> m_uncertainUniformVariableValues;
    std::vector<double> m_uncertainLoguniformVariableValues;
    std::vector<double> m_uncertainTriangularVariableValues;
    std::vector<double> m_uncertainExponentialVariableValues;
    std::vector<double> m_uncertainBetaVariableValues;
    std::vector<double> m_uncertainGammaVariableValues;
    std::vector<double> m_uncertainGumbelVariableValues;
    std::vector<double> m_uncertainFrechetVariableValues;
    std::vector<double> m_uncertainWeibullVariableValues;
    std::vector<double> m_uncertainHistogramBinVariableValues;
    std::vector<int> m_uncertainPoissonVariableValues;
    std::vector<int> m_uncertainBinomialVariableValues;
    std::vector<int> m_uncertainNegativeBinomialVariableValues;
    std::vector<int> m_uncertainGeometricVariableValues;
    std::vector<int> m_uncertainHypergeometricVariableValues;
    std::vector<int> m_uncertainHistogramPointVariableValues;
    std::vector< std::pair<DakotaFunctionType, int> > m_functionASVs;

    bool load(std::istream& is);

    boost::regex variablesRegex() const;
    boost::regex continuousDesignVariableRegex() const;
    boost::regex uncertainNormalVariableRegex() const;
    boost::regex uncertainLognormalVariableRegex() const;
    boost::regex uncertainUniformVariableRegex() const;
    boost::regex uncertainLoguniformVariableRegex() const;
    boost::regex uncertainTriangularVariableRegex() const;
    boost::regex uncertainExponentialVariableRegex() const;
    boost::regex uncertainBetaVariableRegex() const;
    boost::regex uncertainGammaVariableRegex() const;
    boost::regex uncertainGumbelVariableRegex() const;
    boost::regex uncertainFrechetVariableRegex() const;
    boost::regex uncertainWeibullVariableRegex() const;
    boost::regex uncertainHistogramBinVariableRegex() const;
    boost::regex discreteDesignIntegerVariableRegex() const;
    boost::regex uncertainPoissonVariableRegex() const;
    boost::regex uncertainBinomialVariableRegex() const;
    boost::regex uncertainNegativeBinomialVariableRegex() const;
    boost::regex uncertainGeometricVariableRegex() const;
    boost::regex uncertainHypergeometricVariableRegex() const;
    boost::regex uncertainHistogramPointVariableRegex() const;
    boost::regex functionsRegex() const;
    boost::regex functionASVRegex() const;
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_DAKOTAPARAMETERSFILE_IMPL_HPP

