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

#ifndef MODEL_UTILITYCOST_TARIFF_HPP
#define MODEL_UTILITYCOST_TARIFF_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class UtilityCost_Tariff_Impl;
}

/** UtilityCost_Tariff derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:UtilityCost:Tariff".
 *
 *  UtilityCost_Tariff allows the modeling of utilities tariffs.
 */
class MODEL_API UtilityCost_Tariff : public ParentObject{

public:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new UtilityCost_Ratchet object in the model.
  explicit UtilityCost_Tariff(const Model& model);

  virtual ~UtilityCost_Tariff() {}

  //@}
  /** @name Getters */
  //@{

  boost::optional<double> energyConversionFactor() const;

  boost::optional<double> demandConversionFactor() const;

  boost::optional<std::string> outputMeterName() const;

  boost::optional<std::string> conversionFactorChoice() const;

  boost::optional<std::string> timeOfUsePeriodScheduleName() const;

  boost::optional<std::string> seasonScheduleName() const;

  boost::optional<std::string> monthScheduleName() const;

  boost::optional<std::string> demandWindowLength() const;

  boost::optional<std::string> monthlyChargeOrVariableName() const;

  boost::optional<std::string> minimumMonthlyChargeOrVariableName() const;

  boost::optional<std::string> realTimePricingChargeScheduleName() const;

  boost::optional<std::string> customerBaselineLoadScheduleName() const;

  boost::optional<std::string> groupName() const;

  boost::optional<std::string> buyOrSell() const;

  //@}
  /** @name Setters */
  //@{

  bool setEnergyConversionFactor(double energyConversionFactor);

  bool setDemandConversionFactor(double demandConversionFactor);

  bool setOutputMeterName(const std::string& outputMeterName);

  bool setConversionFactorChoice(const std::string& conversionFactorChoice);

  bool setTimeOfUsePeriodScheduleName(const std::string& timeOfUsePeriodScheduleName);

  bool setSeasonScheduleName(const std::string& seasonScheduleName);

  bool setMonthScheduleName(const std::string& monthScheduleName);

  bool setDemandWindowLength(const std::string& demandWindowLength);

  bool setMonthlyChargeOrVariableName(const std::string& monthlyChargeOrVariableName);

  bool setMinimumMonthlyChargeOrVariableName(const std::string& minimumMonthlyChargeOrVariableName);

  bool setRealTimePricingChargeScheduleName(const std::string& realTimePricingChargeScheduleName);

  bool setCustomerBaselineLoadScheduleName(const std::string& customerBaselineLoadScheduleName);

  bool setGroupName(const std::string& groupName);

  bool setBuyOrSell(const std::string& buyOrSell);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

protected:

  /// @cond
  typedef detail::UtilityCost_Tariff_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit UtilityCost_Tariff(std::shared_ptr<detail::UtilityCost_Tariff_Impl> impl);

private:

  REGISTER_LOGGER("openstudio.model.UtilityCost_Tariff");

  /// @endcond

};

/** \relates UtilityCost_Tariff */
typedef boost::optional<UtilityCost_Tariff> OptionalUtilityCost_Tariff;

/** \relates UtilityCost_Tariff */
typedef std::vector<UtilityCost_Tariff> UtilityCost_TariffVector;

} // model
} // openstudio

#endif // MODEL_UTILITYCOST_TARIFF_HPP
