/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILHEATINGDESUPERHEATER_HPP
#define MODEL_COILHEATINGDESUPERHEATER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ModelObject;

namespace detail {

  class CoilHeatingDesuperheater_Impl;

} // detail

/** CoilHeatingDesuperheater is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Desuperheater'. 

  This coil can be used in air loop simulations but can’t be used by certain compound objects 
  (e.g., AirLoopHVACUnitaryHeatPumpAirToAir, AirLoopHVACUnitaryHeatPumpWaterToAir, or DehumidifierDesiccantNoFans) 
  or any air distribution equipment (e.g., AirTerminalSingleDuctConstantVolumeReheat, AirTerminalSingleDuctVAVReheat, 
  or AirTerminalSingleDuctSeriesPIUReheat).
*/
class MODEL_API CoilHeatingDesuperheater : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingDesuperheater(const Model& model);

  virtual ~CoilHeatingDesuperheater() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  double heatReclaimRecoveryEfficiency() const;

  bool isHeatReclaimRecoveryEfficiencyDefaulted() const;

  boost::optional<ModelObject> heatingSource() const;

  double parasiticElectricLoad() const;

  bool isParasiticElectricLoadDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setHeatReclaimRecoveryEfficiency(double heatReclaimRecoveryEfficiency);

  void resetHeatReclaimRecoveryEfficiency();

  /** \fn bool setHeatingSource(const ModelObject& heatingSource)
      \brief Sets the heating source from which the desuperheater heating coil recovers energy.
      \param[in] heatingSource The heating source.

      Valid inputs: \n
      \ref CoilCoolingDXSingleSpeed \n
      \ref CoilCoolingDXTwoSpeed \n
      \ref RefrigerationCondenserAirCooled \n
      \ref RefrigerationCondenserEvaporativeCooled \n
      \ref RefrigerationCondenserWaterCooled \n
  */
  bool setHeatingSource(const ModelObject& modelObject);

  void resetHeatingSource();

  bool setParasiticElectricLoad(double parasiticElectricLoad);

  void resetParasiticElectricLoad();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingDesuperheater_Impl ImplType;

  explicit CoilHeatingDesuperheater(std::shared_ptr<detail::CoilHeatingDesuperheater_Impl> impl);

  friend class detail::CoilHeatingDesuperheater_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingDesuperheater");
};

/** \relates CoilHeatingDesuperheater*/
typedef boost::optional<CoilHeatingDesuperheater> OptionalCoilHeatingDesuperheater;

/** \relates CoilHeatingDesuperheater*/
typedef std::vector<CoilHeatingDesuperheater> CoilHeatingDesuperheaterVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGDESUPERHEATER_HPP

