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

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class ZoneHVACLowTempRadiantVarFlow_Impl;

} // detail

/** ZoneHVACLowTempRadiantVarFlow is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:LowTemperatureRadiant:VariableFlow'. */
class MODEL_API ZoneHVACLowTempRadiantVarFlow : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACLowTempRadiantVarFlow(const Model& model,
                                                                                  Schedule& availabilitySchedule,
                                                                                  HVACComponent& heatingCoil,
                                                                                 HVACComponent& coolingCoil);

  virtual ~ZoneHVACLowTempRadiantVarFlow() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> radiantSurfaceTypeValues();

  static std::vector<std::string> temperatureControlTypeValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  HVACComponent heatingCoil() const;

  HVACComponent coolingCoil() const;

  boost::optional<std::string> radiantSurfaceType() const;

  std::vector<Surface> surfaces() const;

  double hydronicTubingInsideDiameter() const;

  bool isHydronicTubingInsideDiameterDefaulted() const;

  boost::optional<double> hydronicTubingLength() const;

  bool isHydronicTubingLengthDefaulted() const;

  bool isHydronicTubingLengthAutosized() const;

  std::string temperatureControlType() const;

  bool isTemperatureControlTypeDefaulted() const;

  std::string numberofCircuits() const;

  double circuitLength() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setRadiantSurfaceType(std::string radiantSurfaceType);

  void resetRadiantSurfaceType();

  bool setHeatingCoil(HVACComponent& heatingCoil);

  bool setCoolingCoil(HVACComponent& coolingCoil);

  bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

  void resetHydronicTubingInsideDiameter();

  bool setHydronicTubingLength(double hydronicTubingLength);

  void resetHydronicTubingLength();

  void autosizeHydronicTubingLength();

  bool setTemperatureControlType(std::string temperatureControlType);

  void resetTemperatureControlType();

  bool setNumberofCircuits(std::string numberofCircuits);

  bool setCircuitLength(double circuitLength);

  boost::optional<ThermalZone> thermalZone() const;

  bool addToThermalZone(ThermalZone & thermalZone);

  void removeFromThermalZone();

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedHydronicTubingLength() const ;



  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACLowTempRadiantVarFlow_Impl ImplType;

  explicit ZoneHVACLowTempRadiantVarFlow(std::shared_ptr<detail::ZoneHVACLowTempRadiantVarFlow_Impl> impl);

  friend class detail::ZoneHVACLowTempRadiantVarFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACLowTempRadiantVarFlow");
};

/** \relates ZoneHVACLowTempRadiantVarFlow*/
typedef boost::optional<ZoneHVACLowTempRadiantVarFlow> OptionalZoneHVACLowTempRadiantVarFlow;

/** \relates ZoneHVACLowTempRadiantVarFlow*/
typedef std::vector<ZoneHVACLowTempRadiantVarFlow> ZoneHVACLowTempRadiantVarFlowVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_HPP
