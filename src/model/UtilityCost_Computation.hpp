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

  boost::optional<std::string> tariffName() const;

  /** Get compute step at index. Index starts at 0. */
  boost::optional<std::string> computeStep(unsigned index) const;

  //@}
  /** @name Setters */
  //@{

  bool setTariffName(const std::string& tariffName);

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
