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

#ifndef MODEL_UTILITYCOST_COMPUTATION_HPP
#define MODEL_UTILITYCOST_COMPUTATION_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class UtilityCost_Computation_Impl;
}

/** UtilityCost_Computation derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:UtilityCost:Computation".
 *
 *  UtilityCost_Computation lists a series of computations that are used to perform the bill calculation. 
 *  This object is only used for complex tariffs that cannot be modeled any other way.
 */
class MODEL_API UtilityCost_Computation : public ParentObject{
 
 public:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new UtilityCost_Computation object in the model.
  explicit UtilityCost_Computation(const Model& model);

  virtual ~UtilityCost_Computation() {}

  //@}
  /** @name Getters */
  //@{

  boost::optional<std::string> name() const;

  boost::optional<std::string> tariffName() const;

  /** Get compute step at index. Index starts at 0. */
  boost::optional<std::string> computeStep(unsigned index) const;

  //@}
  /** @name Setters */
  //@{

  void setName(const std::string& name);

  void setTariffName(const std::string& tariffName);

  /** Set compute step at index. Index starts at 0. */
  bool setComputeStep(unsigned index, const std::string& computeStep);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

  /// Returns the number of compute steps specified, e.g. the number of extensible fields.
  unsigned numComputeSteps() const;

  /// Returns the maximum number of compute steps that may be specified, e.g. the maximum number of extensible fields.
  unsigned maxComputeSteps() const;

 protected:

  /// @cond

  typedef detail::UtilityCost_Computation_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit UtilityCost_Computation(std::shared_ptr<detail::UtilityCost_Computation_Impl> impl);

private:

  REGISTER_LOGGER("openstudio.model.UtilityCost_Computation");

  /// @endcond

};

/** \relates UtilityCost_Computation */
typedef boost::optional<UtilityCost_Computation> OptionalUtilityCost_Computation;

/** \relates UtilityCost_Computation */
typedef std::vector<UtilityCost_Computation> UtilityCost_ComputationVector;

} // model
} // openstudio

#endif // MODEL_UTILITYCOST_COMPUTATION_HPP
