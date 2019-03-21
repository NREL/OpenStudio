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

#ifndef MODEL_UTILITYCOST_VARIABLE_HPP
#define MODEL_UTILITYCOST_VARIABLE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class UtilityCost_Variable_Impl;
}

/** UtilityCost_Variable derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:UtilityCost:Variable".
 *
 *  UtilityCost_Variable allows for the direct entry of monthly values into a variable.
 */
class MODEL_API UtilityCost_Variable : public ParentObject{

public:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new UtilityCost_Variable object in the model.
  explicit UtilityCost_Variable(const Model& model);

  virtual ~UtilityCost_Variable() {}


  //@}
  /** @name Getters */
  //@{

  boost::optional<double> januaryValue() const;

  boost::optional<double> februaryValue() const;

  boost::optional<double> marchValue() const;

  boost::optional<double> aprilValue() const;

  boost::optional<double> mayValue() const;

  boost::optional<double> juneValue() const;

  boost::optional<double> julyValue() const;

  boost::optional<double> augustValue() const;

  boost::optional<double> septemberValue() const;

  boost::optional<double> octoberValue() const;

  boost::optional<double> novemberValue() const;

  boost::optional<double> decemberValue() const;

  boost::optional<std::string> tariffName() const;

  boost::optional<std::string> variableType() const;

  //@}
  /** @name Setters */
  //@{

  bool setJanuaryValue(double januaryValue);

  bool setFebruaryValue(double februaryValue);

  bool setMarchValue(double marchValue);

  bool setAprilValue(double aprilValue);

  bool setMayValue(double mayValue);

  bool setJuneValue(double juneValue);

  bool setJulyValue(double julyValue);

  bool setAugustValue(double augustValue);

  bool setSeptemberValue(double septemberValue);

  bool setOctoberValue(double octoberValue);

  bool setNovemberValue(double novemberValue);

  bool setDecemberValue(double decemberValue);

  bool setTariffName(const std::string& tariffName);

  bool setVariableType(const std::string& variableType);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

protected:

  /// @cond

  typedef detail::UtilityCost_Variable_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit UtilityCost_Variable(std::shared_ptr<detail::UtilityCost_Variable_Impl> impl);

private:

  REGISTER_LOGGER("openstudio.model.UtilityCost_Variable");

  /// @endcond

};

/** \relates UtilityCost_Variable */
typedef boost::optional<UtilityCost_Variable> OptionalUtilityCost_Variable;

/** \relates UtilityCost_Variable */
typedef std::vector<UtilityCost_Variable> UtilityCost_VariableVector;

} // model
} // openstudio

#endif // MODEL_UTILITYCOST_VARIABLE_HPP
