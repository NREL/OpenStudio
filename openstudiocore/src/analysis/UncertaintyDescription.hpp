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

