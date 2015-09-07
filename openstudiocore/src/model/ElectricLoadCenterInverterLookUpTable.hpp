/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include <model/ModelAPI.hpp>
#include "Inverter.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
//class Zone;

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

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> availabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Zone.
  // DLM: the zone methods should be moved to the Inverter base class
  //boost::optional<Zone> zone() const;

  boost::optional<double> radiativeFraction() const;

  boost::optional<double> ratedMaximumContinuousOutputPower() const;

  boost::optional<double> nightTareLossPower() const;

  boost::optional<double> nominalVoltageInput() const;

  // DLM: the IDD for these fields looks weird, there is no default and the field is not required but I don't
  // see how the object would work without values for these fields
  // DLM: rename, up case all words
  boost::optional<double> efficiencyat10PowerandNominalVoltage() const;

  // DLM: rename, up case all words
  boost::optional<double> efficiencyat20PowerandNominalVoltage() const;

  // DLM: rename, up case all words
  boost::optional<double> efficiencyat30PowerandNominalVoltage() const;

  // DLM: rename, up case all words
  boost::optional<double> efficiencyat50PowerandNominalVoltage() const;

  // DLM: rename, up case all words
  boost::optional<double> efficiencyat75PowerandNominalVoltage() const;

  // DLM: rename, up case all words
  boost::optional<double> efficiencyat100PowerandNominalVoltage() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  // TODO: Check argument type. From object lists, some candidates are: Zone.
  //bool setZone(const Zone& zone);

  void resetZone();

  bool setRadiativeFraction(double radiativeFraction);

  void resetRadiativeFraction();

  void setRatedMaximumContinuousOutputPower(double ratedMaximumContinuousOutputPower);

  void resetRatedMaximumContinuousOutputPower();

  void setNightTareLossPower(double nightTareLossPower);

  void resetNightTareLossPower();

  void setNominalVoltageInput(double nominalVoltageInput);

  void resetNominalVoltageInput();

  // DLM: rename, up case all words
  bool setEfficiencyat10PowerandNominalVoltage(double efficiencyat10PowerandNominalVoltage);

  // DLM: rename, up case all words
  void resetEfficiencyat10PowerandNominalVoltage();

  // DLM: rename, up case all words
  bool setEfficiencyat20PowerandNominalVoltage(double efficiencyat20PowerandNominalVoltage);

  // DLM: rename, up case all words
  void resetEfficiencyat20PowerandNominalVoltage();

  // DLM: rename, up case all words
  bool setEfficiencyat30PowerandNominalVoltage(double efficiencyat30PowerandNominalVoltage);

  // DLM: rename, up case all words
  void resetEfficiencyat30PowerandNominalVoltage();

  // DLM: rename, up case all words
  bool setEfficiencyat50PowerandNominalVoltage(double efficiencyat50PowerandNominalVoltage);

  // DLM: rename, up case all words
  void resetEfficiencyat50PowerandNominalVoltage();

  // DLM: rename, up case all words
  bool setEfficiencyat75PowerandNominalVoltage(double efficiencyat75PowerandNominalVoltage);

  // DLM: rename, up case all words
  void resetEfficiencyat75PowerandNominalVoltage();

  // DLM: rename, up case all words
  bool setEfficiencyat100PowerandNominalVoltage(double efficiencyat100PowerandNominalVoltage);

  // DLM: rename, up case all words
  void resetEfficiencyat100PowerandNominalVoltage();

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

