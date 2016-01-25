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

#ifndef MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_HPP
#define MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class CoilPerformanceDXCooling;

namespace detail {

  class CoilCoolingDXTwoStageWithHumidityControlMode_Impl;

} // detail

/** CoilCoolingDXTwoStageWithHumidityControlMode is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:TwoStageWithHumidityControlMode'. */
class MODEL_API CoilCoolingDXTwoStageWithHumidityControlMode : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingDXTwoStageWithHumidityControlMode(const Model& model);

  virtual ~CoilCoolingDXTwoStageWithHumidityControlMode() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  double crankcaseHeaterCapacity() const;

  double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

  int numberofCapacityStages() const;

  int numberofEnhancedDehumidificationModes() const;

  boost::optional<CoilPerformanceDXCooling> normalModeStage1CoilPerformance() const;

  boost::optional<CoilPerformanceDXCooling> normalModeStage1Plus2CoilPerformance() const;

  boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1CoilPerformance() const;

  boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1Plus2CoilPerformance() const;

  double basinHeaterCapacity() const;

  double basinHeaterSetpointTemperature() const;

  boost::optional<Schedule> basinHeaterOperatingSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

  bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

  bool setNumberofCapacityStages(int numberofCapacityStages);

  bool setNumberofEnhancedDehumidificationModes(int numberofEnhancedDehumidificationModes);

  bool setNormalModeStage1CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

  void resetNormalModeStage1CoilPerformance();

  bool setNormalModeStage1Plus2CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

  void resetNormalModeStage1Plus2CoilPerformance();

  bool setDehumidificationMode1Stage1CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

  void resetDehumidificationMode1Stage1CoilPerformance();

  bool setDehumidificationMode1Stage1Plus2CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

  void resetDehumidificationMode1Stage1Plus2CoilPerformance();

  bool setBasinHeaterCapacity(double basinHeaterCapacity);

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

  bool setBasinHeaterOperatingSchedule(Schedule& schedule);

  void resetBasinHeaterOperatingSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl ImplType;

  explicit CoilCoolingDXTwoStageWithHumidityControlMode(std::shared_ptr<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl> impl);

  friend class detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingDXTwoStageWithHumidityControlMode");
};

/** \relates CoilCoolingDXTwoStageWithHumidityControlMode*/
typedef boost::optional<CoilCoolingDXTwoStageWithHumidityControlMode> OptionalCoilCoolingDXTwoStageWithHumidityControlMode;

/** \relates CoilCoolingDXTwoStageWithHumidityControlMode*/
typedef std::vector<CoilCoolingDXTwoStageWithHumidityControlMode> CoilCoolingDXTwoStageWithHumidityControlModeVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_HPP

