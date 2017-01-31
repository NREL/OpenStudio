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

#ifndef MODEL_AIRTERMINALSINGLEDUCTUNCONTROLLED_HPP
#define MODEL_AIRTERMINALSINGLEDUCTUNCONTROLLED_HPP

#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"
#include "ModelAPI.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class AirTerminalSingleDuctUncontrolled_Impl;

} // detail

/** AirTerminalSingleDuctUncontrolled is an interface to the EnergyPlus IDD object
 *  named "AirTerminal:SingleDuct:Uncontrolled"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  AirTerminal:SingleDuct:Uncontrolled objects in energyplus.  
 */
class MODEL_API AirTerminalSingleDuctUncontrolled : public StraightComponent {

 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new AirTerminalSingleDuctUncontrolled object and places it inside the 
   *  model.  The object is fully initialized with all companion objects. */
  AirTerminalSingleDuctUncontrolled(const Model& model, Schedule & availabilitySchedule);

  virtual ~AirTerminalSingleDuctUncontrolled() {}

  //@}

  static IddObjectType iddObjectType();

  /** Returns the Schedule referred to by the AvailabilityScheduleName field. **/
  Schedule availabilitySchedule() const;

  /** Sets the Schedule referred to by the AvailabilityScheduleName field. **/
  bool setAvailabilitySchedule(Schedule& schedule);


  boost::optional<double> maximumAirFlowRate() const;

  OSOptionalQuantity getMaximumAirFlowRate(bool returnIP=false) const;

  bool isMaximumAirFlowRateAutosized() const;

  bool setMaximumAirFlowRate(double maximumAirFlowRate);

  bool setMaximumAirFlowRate(const Quantity& maximumAirFlowRate);

  void autosizeMaximumAirFlowRate();

  //@}
 protected:
  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @cond 
  typedef detail::AirTerminalSingleDuctUncontrolled_Impl ImplType;

  explicit AirTerminalSingleDuctUncontrolled(std::shared_ptr<detail::AirTerminalSingleDuctUncontrolled_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctUncontrolled");
  /// @endcond 
};

typedef boost::optional<AirTerminalSingleDuctUncontrolled> OptionalAirTerminalSingleDuctUncontrolled;

typedef std::vector<AirTerminalSingleDuctUncontrolled> AirTerminalSingleDuctUncontrolledVector;

} // model

} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTUNCONTROLLED_HPP

