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

  void setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

  void setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1);

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

