/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class AvailabilityManagerNightCycle_Impl;

} // detail

/** AvailabilityManagerNightCycle is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManager:NightCycle'. */
class MODEL_API AvailabilityManagerNightCycle : public AvailabilityManager {
 public:

  explicit AvailabilityManagerNightCycle(const Model& model);

  virtual ~AvailabilityManagerNightCycle() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlTypeValues();


  std::string controlType() const;

  bool setControlType(std::string controlType);

  void resetControlType();

  bool isControlTypeDefaulted() const;


  double thermostatTolerance() const;

  Quantity getThermostatTolerance(bool returnIP=false) const;

  bool setThermostatTolerance(const Quantity& thermostatTolerance);

  void setThermostatTolerance(double thermostatTolerance);

  bool isThermostatToleranceDefaulted() const;

  void resetThermostatTolerance();


  double cyclingRunTime() const;

  void setCyclingRunTime(double cyclingRunTime);

  bool setCyclingRunTime(const Quantity& cyclingRunTime);

  void resetCyclingRunTime();

  Quantity getCyclingRunTime(bool returnIP=false) const;

  bool isCyclingRunTimeDefaulted() const;


  boost::optional<ThermalZone> controlThermalZone() const;

  bool setControlThermalZone(const ThermalZone& thermalZone);

  void resetControlThermalZone();

 protected:

  /// @cond
  typedef detail::AvailabilityManagerNightCycle_Impl ImplType;

  explicit AvailabilityManagerNightCycle(std::shared_ptr<detail::AvailabilityManagerNightCycle_Impl> impl);

  friend class detail::AvailabilityManagerNightCycle_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:
  REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightCycle");
};

/** \relates AvailabilityManagerNightCycle*/
typedef boost::optional<AvailabilityManagerNightCycle> OptionalAvailabilityManagerNightCycle;

/** \relates AvailabilityManagerNightCycle*/
typedef std::vector<AvailabilityManagerNightCycle> AvailabilityManagerNightCycleVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP

