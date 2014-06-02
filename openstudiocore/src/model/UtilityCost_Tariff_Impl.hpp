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

#ifndef MODEL_UTILITYCOST_TARIFF_IMPL_HPP
#define MODEL_UTILITYCOST_TARIFF_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "UtilityCost_Tariff.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {
namespace detail {

class MODEL_API UtilityCost_Tariff_Impl : public ParentObject_Impl{
  
 public:
  // constructor
  UtilityCost_Tariff_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  // construct from workspace
  UtilityCost_Tariff_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                 Model_Impl* model,
                 bool keepHandle);

  // clone copy constructor
  UtilityCost_Tariff_Impl(const UtilityCost_Tariff_Impl& other,Model_Impl* model,bool keepHandle);

  // virtual destructor
  virtual ~UtilityCost_Tariff_Impl(){}

  boost::optional<double> energyConversionFactor() const;
  void setEnergyConversionFactor(double num);

  boost::optional<double> demandConversionFactor() const;
  void setDemandConversionFactor(double num);

  OptionalString outputMeterName() const;
  void setOutputMeterName(const std::string& str);

  OptionalString conversionFactorChoice() const;
  void setConversionFactorChoice(const std::string& str);

  OptionalString timeOfUsePeriodScheduleName() const;
  void setTimeOfUsePeriodScheduleName(const std::string& str);

  OptionalString seasonScheduleName() const;
  void setSeasonScheduleName(const std::string& str);

  OptionalString monthScheduleName() const;
  void setMonthScheduleName(const std::string& str);

  OptionalString demandWindowLength() const;
  void setDemandWindowLength(const std::string& str);

  OptionalString monthlyChargeOrVariableName() const;
  void setMonthlyChargeOrVariableName(const std::string& str);

  OptionalString minimumMonthlyChargeOrVariableName() const;
  void setMinimumMonthlyChargeOrVariableName(const std::string& str);

  OptionalString realTimePricingChargeScheduleName() const;
  void setRealTimePricingChargeScheduleName(const std::string& str);

  OptionalString customerBaselineLoadScheduleName() const;
  void setCustomerBaselineLoadScheduleName(const std::string& str);

  OptionalString groupName() const;
  void setGroupName(const std::string& str);

  OptionalString buyOrSell() const;
  void setBuyOrSell(const std::string& str);

  // return the parent object in the hierarchy
  virtual boost::optional<ParentObject> parent() const;

  // set the parent, child may have to call methods on the parent
  virtual bool setParent(ParentObject& newParent);

  // return any children objects in the hierarchy
  virtual std::vector<ModelObject> children() const;

  /// get a vector of allowable children types
  virtual std::vector<IddObjectType> allowableChildTypes() const;

  // Get all output variable names that could be associated with this object.
  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const {return UtilityCost_Tariff::iddObjectType();}

private:
  REGISTER_LOGGER("openstudio.model.UtilityCost_Tariff");

};

} // detail
} // model
} // openstudio

#endif // MODEL_UTILITYCOST_TARIFF_IMPL_HPP
