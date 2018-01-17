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

#ifndef ANALYSIS_UNCERTAINTYDESCRIPTION_HPP
#define ANALYSIS_UNCERTAINTYDESCRIPTION_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisEnums.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/data/Attribute.hpp"

#include <boost/optional.hpp>

namespace openstudio {
namespace analysis {

namespace detail {
  class UncertaintyDescription_Impl;
}

/** UncertaintyDescription is an abstract base class that can be used to append uncertainty information
 *  to a Variable. At this time, such descriptions are only used if the variables are sampled using 
 *  SamplingAlgorithm (which is available through DAKOTA). */
class ANALYSIS_API UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{
  
  virtual ~UncertaintyDescription() {}

  UncertaintyDescription clone() const;
  
  //@}

  /** Returns all the \link UncertaintyDescriptionType UncertaintyDescriptionTypes\endlink that are 
   *  applicable to variableValueType (VariableValueType::Continuous or VariableValueType::Discrete). */
  static std::vector<UncertaintyDescriptionType> validTypes(const VariableValueType& variableValueType);

  /** Returns all the \link UncertaintyDescriptionType UncertaintyDescriptionTypes\endlink that are 
   *  applicable to the specified combination of variableValueType (VariableValueType::Continuous or 
   *  VariableValueType::Discrete) and uncertaintyType (UncertaintyType::Aleatory or 
   *  UncertaintyType::Epistemic). */
  static std::vector<UncertaintyDescriptionType> validTypes(const VariableValueType& variableValueType,
                                                            const UncertaintyType& uncertaintyType);

  /** @name Getters and Queries */
  //@{

  UncertaintyDescriptionType type() const;

  //@}
  /** @name Setters */
  //@{

  /** Restores the default options for this UncertaintyDescription and its type(). */
  void restoreDefaults();

  //@}
  /** @name Type Casting */
  //@{
  
  // ETH@20120921 TODO: cast() should allow casts to the base class, but I don't think it does right now.

  /** Casts the UncertaintyDescription to type T, if possible. All descriptions can be cast to 
   *  GenericUncertaintyDescription, which provides a unified interface for all types. Otherwise, the
   *  cast will only succeed if type() == T::type(). */
  template<typename T>
  T cast() const {
    bool isCorrectType(type() == T::type());
    if (!isCorrectType && (T::type() != UncertaintyDescriptionType(UncertaintyDescriptionType::Generic))) {
      throw(std::bad_cast());
    }
    return T(impl());
  }

  template<typename T>
  boost::optional<T> optionalCast() const {
    boost::optional<T> result;
    bool isCorrectType(type() == T::type());
    if (!isCorrectType && (T::type() != UncertaintyDescriptionType(UncertaintyDescriptionType::Generic))) {
      return result;
    }
    return T(impl());
  }

  //@}
 protected:
   explicit UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl> impl);

   typedef detail::UncertaintyDescription_Impl ImplType;

   friend class detail::UncertaintyDescription_Impl;
   friend class AnalysisObject;

   std::shared_ptr<detail::UncertaintyDescription_Impl> impl() const;
 private: 
   std::shared_ptr<detail::UncertaintyDescription_Impl> m_impl;

   // private, unimplemented default constructor
   UncertaintyDescription();

   REGISTER_LOGGER("openstudio.analysis.UncertaintyDescription");
};

/** \relates UncertaintyDescription */
typedef boost::optional<UncertaintyDescription> OptionalUncertaintyDescription;

namespace detail {

  QVariant toVariant(const UncertaintyDescription& udesc);

  UncertaintyDescription toUncertaintyDescription(const QVariant& variant,
                                                  const VersionString& version);

}

} // analysis
} // openstudio

#endif // ANALYSIS_UNCERTAINTYDESCRIPTION_HPP

