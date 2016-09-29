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

#ifndef ANALYSIS_ALGORITHMOPTIONS_HPP
#define ANALYSIS_ALGORITHMOPTIONS_HPP

#include "AnalysisAPI.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

class Attribute;

namespace analysis {

class Algorithm;

namespace detail {

  class AlgorithmOptions_Impl;
  class Algorithm_Impl;

} // detail

/** AlgorithmOptions defines the basic options that all \link Algorithm Algorithms \endlink are 
 *  assumed to have, that is, a maximum number of iterations and a maximum number of simulations.
 *  All Algorithm classes are constructed with an AlgorithmOptions object (often of a derived 
 *  type). \relates Algorithm
 */
class ANALYSIS_API AlgorithmOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** By default, maxIter and maxSims are set to boost::none (no restriction). */
  AlgorithmOptions();

  virtual ~AlgorithmOptions() {}

  AlgorithmOptions clone() const;

  //@}
  /** @name Getters */
  //@{

  boost::optional<int> maxIter() const;

  boost::optional<int> maxSims() const;

  /** Getter provided for serialization; not for general use. */
  std::vector<Attribute> options() const;

  //@}
  /** @name Setters */
  //@{

  void setMaxIter(int maxIter);

  void setMaxSims(int maxSims);

  void clearMaxIter();

  void clearMaxSims();

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
  typedef detail::AlgorithmOptions_Impl ImplType;

  explicit AlgorithmOptions(std::shared_ptr<detail::AlgorithmOptions_Impl> impl);

  friend class detail::AlgorithmOptions_Impl;
  /// @endcond

  friend class Algorithm;

  void setAlgorithm(const Algorithm& algorithm);

  friend class detail::Algorithm_Impl;

  QVariant toVariant() const;
 private:
  std::shared_ptr<detail::AlgorithmOptions_Impl> m_impl;

  REGISTER_LOGGER("openstudio.analysis.AlgorithmOptions");
};

/** \relates AlgorithmOptions*/
typedef boost::optional<AlgorithmOptions> OptionalAlgorithmOptions;

/** \relates AlgorithmOptions*/
typedef std::vector<AlgorithmOptions> AlgorithmOptionsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_ALGORITHMOPTIONS_HPP

