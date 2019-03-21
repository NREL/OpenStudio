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

#ifndef MODEL_UTILITYCOST_RATCHET_HPP
#define MODEL_UTILITYCOST_RATCHET_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class UtilityCost_Ratchet_Impl;
}

/** UtilityCost_Ratchet derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:UtilityCost:Ratchet".
 *
 *  UtilityCost_Ratchet allows the modeling of tariffs that include some type of seasonal ratcheting.
 *  Ratchets are most common when used with electric demand charges.
 */
class MODEL_API UtilityCost_Ratchet : public ParentObject{

public:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new UtilityCost_Ratchet object in the model.
  explicit UtilityCost_Ratchet(const Model& model);

  virtual ~UtilityCost_Ratchet() {}


  //@}
  /** @name Getters */
  //@{

  boost::optional<std::string> tariffName() const;

  boost::optional<std::string> baselineSourceVariable() const;

  boost::optional<std::string> adjustmentSourceVariable() const;

  boost::optional<std::string> seasonFrom() const;

  boost::optional<std::string> seasonTo() const;

  boost::optional<std::string> multiplierValueOrVariableName() const;

  boost::optional<std::string> offsetValueOrVariableName() const;

  //@}
  /** @name Setters */
  //@{

  bool setTariffName(const std::string& tariffName);

  bool setBaselineSourceVariable(const std::string& baselineSourceVariable);

  bool setAdjustmentSourceVariable(const std::string& adjustmentSourceVariable);

  bool setSeasonFrom(const std::string& seasonFrom);

  bool setSeasonTo(const std::string& seasonTo);

  bool setMultiplierValueOrVariableName(const std::string& multiplierValueOrVariableName);

  bool setOffsetValueOrVariableName(const std::string& offsetValueOrVariableName);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

protected:

  /// @cond

  typedef detail::UtilityCost_Ratchet_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit UtilityCost_Ratchet(std::shared_ptr<detail::UtilityCost_Ratchet_Impl> impl);

private:

  REGISTER_LOGGER("openstudio.model.UtilityCost_Ratchet");

  /// @endcond

};

/** \relates UtilityCost_Ratchet */
typedef boost::optional<UtilityCost_Ratchet> OptionalUtilityCost_Ratchet;

/** \relates UtilityCost_Ratchet */
typedef std::vector<UtilityCost_Ratchet> UtilityCost_RatchetVector;

} // model
} // openstudio

#endif // MODEL_UTILITYCOST_RATCHET_HPP
