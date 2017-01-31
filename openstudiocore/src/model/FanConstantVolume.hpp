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

#ifndef MODEL_FANCONSTANTVOLUME_HPP
#define MODEL_FANCONSTANTVOLUME_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {
  class FanConstantVolume_Impl;
} // detail

/** FanConstantVolume is a StraightComponent that wraps the IDD object named
 *  "OS:Fan:ConstantVolume". */
class MODEL_API FanConstantVolume : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new FanConstantVolume object and places it inside the
   *  model.  The object is fully initialized with all companion objects. */
  FanConstantVolume(const Model& model,Schedule& s);

  FanConstantVolume(const Model& model);

  virtual ~FanConstantVolume() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  /** Returns the value of the FanEfficiency field. **/
  double fanEfficiency() const;

  /** Returns the value of the PressureRise field. **/
  double pressureRise() const;

  /** Returns the value of the MotorEfficiency field. **/
  double motorEfficiency() const;

  /** Returns the value of the MotorInAirStreamFraction field. **/
  double motorInAirstreamFraction() const;

  /** Returns the value of the EndUseSubcategory field. **/
  std::string endUseSubcategory() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& s);

  /** Sets the value of the FanEfficiency field. **/
  void setFanEfficiency(double value);

  /** Sets the value of the PressureRise field. **/
  void setPressureRise(double value);

  /** Sets the value of the MotorEfficiency field. **/
  void setMotorEfficiency(double value);

  /** Sets the value of the MotorInAirStreamFraction field. **/
  void setMotorInAirstreamFraction(double value);

  /** Sets the value of the EndUseSubcategory field. **/
  void setEndUseSubcategory(std::string value);

  //@}

  boost::optional<double> maximumFlowRate() const;

  OSOptionalQuantity getMaximumFlowRate(bool returnIP=false) const;

  bool isMaximumFlowRateAutosized() const;

  bool setMaximumFlowRate(double maximumFlowRate);

  bool setMaximumFlowRate(const Quantity& maximumFlowRate);

  void resetMaximumFlowRate();

  void autosizeMaximumFlowRate();

 protected:
  friend class Model;
  friend class openstudio::IdfObject;

  /// @cond
  typedef detail::FanConstantVolume_Impl ImplType;
  explicit FanConstantVolume(std::shared_ptr<detail::FanConstantVolume_Impl> impl);
 private:
  REGISTER_LOGGER("openstudio.model.FanConstantVolume");
  /// @endcond
};

/** \relates FanConstantVolume */
typedef boost::optional<FanConstantVolume> OptionalFanConstantVolume;

/** \relates FanConstantVolume */
typedef std::vector<FanConstantVolume> FanConstantVolumeVector;

} // model
} // openstudio

#endif // MODEL_FANCONSTANTVOLUME_HPP

