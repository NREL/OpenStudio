/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef ANALYSIS_DAKOTAPARAMETERSFILE_HPP
#define ANALYSIS_DAKOTAPARAMETERSFILE_HPP

#include "AnalysisAPI.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Path.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {
namespace analysis {

class DakotaFunctionType;
class UncertaintyDescriptionType;

namespace detail {

  class DakotaParametersFile_Impl;

} // detail

/** DakotaParametersFile is an interface to the files that DAKOTA drops on the filesystem to 
 *  indicate that we should create and run a new DataPoint. */
class ANALYSIS_API DakotaParametersFile {
 public:

  /** Loads the DAKOTA parameters input file at p, if possible. Otherwise, return value evaluates
   *  to false. */
  static boost::optional<DakotaParametersFile> load(const openstudio::path& p);

  /** @name Getters and Queries */
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

  /** Get the impl pointer */
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  /** Cast to type T. Throws std::bad_cast if object is not a T. */
  template<typename T>
  T cast() const {
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) {
      throw(std::bad_cast());
    }
    return T(impl);
  }

  /** Cast to boost::optional<T>. Return value is boost::none (evaluates to false) 
   *  if object is not a T. */
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl);
    }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::DakotaParametersFile_Impl ImplType;

  explicit DakotaParametersFile(std::shared_ptr<detail::DakotaParametersFile_Impl> impl);

  friend class detail::DakotaParametersFile_Impl;

  /// @endcond
 private:

  std::shared_ptr<detail::DakotaParametersFile_Impl> m_impl;

  REGISTER_LOGGER("openstudio.analysis.DakotaParametersFile");
};

/** \relates DakotaParametersFile*/
typedef boost::optional<DakotaParametersFile> OptionalDakotaParametersFile;

/** \relates DakotaParametersFile*/
typedef std::vector<DakotaParametersFile> DakotaParametersFileVector;

/** Returns the variable values for those variables with uncertainty of type.
 *  \relates DakotaParametersFile */
ANALYSIS_API std::vector<QVariant> getUncertainVariableValues(
    const DakotaParametersFile& params,
    const UncertaintyDescriptionType& type);

/** Returns the variable values for those variables with uncertainty of type. Throws if type is
 *  not applicable to continuous variables. \relates DakotaParametersFile */
ANALYSIS_API std::vector<double> getContinuousUncertainVariableValues(
    const DakotaParametersFile& params,
    const UncertaintyDescriptionType& type);

/** Returns the variable values for those variables with uncertainty of type. Throws if type is
 *  not applicable to discrete variables. \relates DakotaParametersFile */
ANALYSIS_API std::vector<int> getDiscreteUncertainVariableValues(
    const DakotaParametersFile& params,
    const UncertaintyDescriptionType& type);

} // analysis
} // openstudio

#endif // ANALYSIS_DAKOTAPARAMETERSFILE_HPP

