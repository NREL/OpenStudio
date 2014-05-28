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

  boost::optional<std::string> name() const;

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

  void setEnergyConversionFactor(double energyConversionFactor);
  
  void setDemandConversionFactor(double demandConversionFactor);
  
  bool setName(const std::string& name);

  void setOutputMeterName(const std::string& outputMeterName);

  void setConversionFactorChoice(const std::string& conversionFactorChoice);

  void setTimeOfUsePeriodScheduleName(const std::string& timeOfUsePeriodScheduleName);
  
  void setSeasonScheduleName(const std::string& seasonScheduleName);

  void setMonthScheduleName(const std::string& monthScheduleName);

  void setDemandWindowLength(const std::string& demandWindowLength);
  
  void setMonthlyChargeOrVariableName(const std::string& monthlyChargeOrVariableName);
  
  void setMinimumMonthlyChargeOrVariableName(const std::string& minimumMonthlyChargeOrVariableName);

  void setRealTimePricingChargeScheduleName(const std::string& realTimePricingChargeScheduleName);
  
  void setCustomerBaselineLoadScheduleName(const std::string& customerBaselineLoadScheduleName);
  
  void setGroupName(const std::string& groupName);

  void setBuyOrSell(const std::string& buyOrSell);

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
