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

#ifndef MODEL_COILCOOLINGDXMULTISPEED_HPP
#define MODEL_COILCOOLINGDXMULTISPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class CoilCoolingDXMultiSpeedStageData;

namespace detail {

  class CoilCoolingDXMultiSpeed_Impl;

} // detail

/** CoilCoolingDXMultiSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:MultiSpeed'.
 * Performance data for each stage is contained in a separate ModelObject called CoilCoolingDXMultiSpeedStageData **/
class MODEL_API CoilCoolingDXMultiSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingDXMultiSpeed(const Model& model);

  virtual ~CoilCoolingDXMultiSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> condenserTypeValues();

  static std::vector<std::string> fuelTypeValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  std::string condenserType() const;

  bool applyPartLoadFractiontoSpeedsGreaterthan1() const;

  bool applyLatentDegradationtoSpeedsGreaterthan1() const;

  bool isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted() const;

  double crankcaseHeaterCapacity() const;

  double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

  double basinHeaterCapacity() const;

  double basinHeaterSetpointTemperature() const;

  boost::optional<Schedule> basinHeaterOperatingSchedule() const;

  std::string fuelType() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setCondenserType(std::string condenserType);

  bool setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

  bool setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1);

  void resetApplyLatentDegradationtoSpeedsGreaterthan1();

  bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

  bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

  bool setBasinHeaterCapacity(double basinHeaterCapacity);

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

  bool setBasinHeaterOperatingSchedule(Schedule& schedule);

  void resetBasinHeaterOperatingSchedule();

  bool setFuelType(std::string fuelType);

  //@}
  /** @name Other */
  //@{

  /** Return the performance data for each stage. **/
  std::vector<CoilCoolingDXMultiSpeedStageData> stages() const;

  /** Add a new stage after all of the existing stages **/
  void addStage(CoilCoolingDXMultiSpeedStageData& stage);

  //@}
 protected:
  /// @cond
  typedef detail::CoilCoolingDXMultiSpeed_Impl ImplType;

  explicit CoilCoolingDXMultiSpeed(std::shared_ptr<detail::CoilCoolingDXMultiSpeed_Impl> impl);

  friend class detail::CoilCoolingDXMultiSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingDXMultiSpeed");
};

/** \relates CoilCoolingDXMultiSpeed*/
typedef boost::optional<CoilCoolingDXMultiSpeed> OptionalCoilCoolingDXMultiSpeed;

/** \relates CoilCoolingDXMultiSpeed*/
typedef std::vector<CoilCoolingDXMultiSpeed> CoilCoolingDXMultiSpeedVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXMULTISPEED_HPP
