/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  bool setRatedMaximumContinuousOutputPower(double ratedMaximumContinuousOutputPower);

  void resetRatedMaximumContinuousOutputPower();

  bool setNightTareLossPower(double nightTareLossPower);

  void resetNightTareLossPower();

  bool setNominalVoltageInput(double nominalVoltageInput);

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
