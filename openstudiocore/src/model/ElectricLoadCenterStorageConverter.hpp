/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Curve;
class ThermalZone;
class ElectricLoadCenterDistribution;

namespace detail {

  class ElectricLoadCenterStorageConverter_Impl;

} // detail

/** ElectricLoadCenterStorageConverter is a ParentObject that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Storage:Converter'. */
class MODEL_API ElectricLoadCenterStorageConverter : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ElectricLoadCenterStorageConverter(const Model& model);

  virtual ~ElectricLoadCenterStorageConverter() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> powerConversionEfficiencyMethodValues();
  
  /** @name Getters */
  //@{
    
  boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

  Schedule availabilitySchedule() const;
  bool isAvailabilityScheduleDefaulted() const;

  std::string powerConversionEfficiencyMethod() const;

  // Required if powerConversionEfficiencyMethod == "SimpleFixed"
  boost::optional<double> simpleFixedEfficiency() const;

  // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
  boost::optional<double> designMaximumContinuousInputPower() const;

  // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
  // TODO: Later may need to add support for Tables. (UniVariateCurves, UniVariateTables.)
  boost::optional<Curve> efficiencyFunctionofPowerCurve() const;

  // Defaults to 0.0
  double ancillaryPowerConsumedInStandby() const;

  bool isAncillaryPowerConsumedInStandbyDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  boost::optional<ThermalZone> thermalZone() const;

  double radiativeFraction() const;

  bool isRadiativeFractionDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  // No Setter, will be done when calling one of the three following functions
  //bool setPowerConversionEfficiencyMethod(const std::string& powerConversionEfficiencyMethod);

  bool setSimpleFixedEfficiency(double simpleFixedEfficiency);

  bool setDesignMaximumContinuousInputPower(double designMaximumContinuousInputPower);

  bool setEfficiencyFunctionofPowerCurve(const Curve& efficiencyFunctionofPowerCurve);

  bool setAncillaryPowerConsumedInStandby(double ancillaryPowerConsumedInStandby);

  void resetAncillaryPowerConsumedInStandby();

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
  bool setThermalZone(const ThermalZone& thermalZone);

  void resetThermalZone();

  bool setRadiativeFraction(double radiativeFraction);

  void resetRadiativeFraction();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ElectricLoadCenterStorageConverter_Impl ImplType;

  explicit ElectricLoadCenterStorageConverter(std::shared_ptr<detail::ElectricLoadCenterStorageConverter_Impl> impl);

  friend class detail::ElectricLoadCenterStorageConverter_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageConverter");
};

/** \relates ElectricLoadCenterStorageConverter */
typedef boost::optional<ElectricLoadCenterStorageConverter> OptionalElectricLoadCenterStorageConverter;

/** \relates ElectricLoadCenterStorageConverter */
typedef std::vector<ElectricLoadCenterStorageConverter> ElectricLoadCenterStorageConverterVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_HPP

