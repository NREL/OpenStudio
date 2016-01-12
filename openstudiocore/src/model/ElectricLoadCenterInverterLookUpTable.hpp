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

#ifndef MODEL_ELECTRICLOADCENTERINVERTERLOOKUPTABLE_HPP
#define MODEL_ELECTRICLOADCENTERINVERTERLOOKUPTABLE_HPP

#include "ModelAPI.hpp"
#include "Inverter.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class ElectricLoadCenterInverterLookUpTable_Impl;

} // detail

/** ElectricLoadCenterInverterLookUpTable is a Inverter that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Inverter:LookUpTable'. */
class MODEL_API ElectricLoadCenterInverterLookUpTable : public Inverter {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ElectricLoadCenterInverterLookUpTable(const Model& model);

  virtual ~ElectricLoadCenterInverterLookUpTable() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> radiativeFraction() const;

  boost::optional<double> ratedMaximumContinuousOutputPower() const;

  boost::optional<double> nightTareLossPower() const;

  boost::optional<double> nominalVoltageInput() const;

  // DLM: the IDD for these fields looks weird, there is no default and the field is not required but I don't
  // see how the object would work without values for these fields

  boost::optional<double> efficiencyAt10PowerAndNominalVoltage() const;


  boost::optional<double> efficiencyAt20PowerAndNominalVoltage() const;


  boost::optional<double> efficiencyAt30PowerAndNominalVoltage() const;


  boost::optional<double> efficiencyAt50PowerAndNominalVoltage() const;


  boost::optional<double> efficiencyAt75PowerAndNominalVoltage() const;


  boost::optional<double> efficiencyAt100PowerAndNominalVoltage() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setRadiativeFraction(double radiativeFraction);

  void resetRadiativeFraction();

  void setRatedMaximumContinuousOutputPower(double ratedMaximumContinuousOutputPower);

  void resetRatedMaximumContinuousOutputPower();

  void setNightTareLossPower(double nightTareLossPower);

  void resetNightTareLossPower();

  void setNominalVoltageInput(double nominalVoltageInput);

  void resetNominalVoltageInput();


  bool setEfficiencyAt10PowerAndNominalVoltage(double efficiencyAt10PowerAndNominalVoltage);


  void resetEfficiencyAt10PowerAndNominalVoltage();


  bool setEfficiencyAt20PowerAndNominalVoltage(double efficiencyAt20PowerAndNominalVoltage);


  void resetEfficiencyAt20PowerAndNominalVoltage();


  bool setEfficiencyAt30PowerAndNominalVoltage(double efficiencyAt30PowerAndNominalVoltage);


  void resetEfficiencyAt30PowerAndNominalVoltage();


  bool setEfficiencyAt50PowerAndNominalVoltage(double efficiencyAt50PowerAndNominalVoltage);


  void resetEfficiencyAt50PowerAndNominalVoltage();


  bool setEfficiencyAt75PowerAndNominalVoltage(double efficiencyAt75PowerAndNominalVoltage);


  void resetEfficiencyAt75PowerAndNominalVoltage();


  bool setEfficiencyAt100PowerAndNominalVoltage(double efficiencyAt100PowerAndNominalVoltage);


  void resetEfficiencyAt100PowerAndNominalVoltage();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ElectricLoadCenterInverterLookUpTable_Impl ImplType;

  explicit ElectricLoadCenterInverterLookUpTable(std::shared_ptr<detail::ElectricLoadCenterInverterLookUpTable_Impl> impl);

  friend class detail::ElectricLoadCenterInverterLookUpTable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ElectricLoadCenterInverterLookUpTable");
};

/** \relates ElectricLoadCenterInverterLookUpTable*/
typedef boost::optional<ElectricLoadCenterInverterLookUpTable> OptionalElectricLoadCenterInverterLookUpTable;

/** \relates ElectricLoadCenterInverterLookUpTable*/
typedef std::vector<ElectricLoadCenterInverterLookUpTable> ElectricLoadCenterInverterLookUpTableVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERINVERTERLOOKUPTABLE_HPP

