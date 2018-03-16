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

#ifndef MODEL_AIRFLOWNETWORKEQUIVALENTDUCT_HPP
#define MODEL_AIRFLOWNETWORKEQUIVALENTDUCT_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class StraightComponent;

// TODO: Check the following class names against object getters and setters.
// AFNTerminalUnit;
// AFNCoilNames
// AFNHeatExchangerNames
//   HeatExchanger:AirToAir:FlatPlate
//   HeatExchanger:Desiccant:BalancedFlow


namespace detail {

  class HeatExchangerAirToAirSensibleAndLatent_Impl;
  class CoilHeatingDXMultiSpeed_Impl;
  class CoilHeatingWater_Impl;
  class CoilHeatingDesuperheater_Impl;
  class CoilHeatingGas_Impl;
  class CoilHeatingElectric_Impl;
  class CoilHeatingDXSingleSpeed_Impl;
  class CoilCoolingWater_Impl;
  class CoilCoolingDXMultiSpeed_Impl;
  class CoilCoolingDXTwoStageWithHumidityControlMode_Impl;
  class CoilHeatingDXSingleSpeed_Impl;
  class CoilCoolingDXSingleSpeed_Impl;
  class AirTerminalSingleDuctConstantVolumeReheat_Impl;
  class AirTerminalSingleDuctVAVReheat_Impl;
  class AirflowNetworkEquivalentDuct_Impl;

} // detail

/** AirflowNetworkEquivalentDuct is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetwork:EquivalentDuct'. */
class MODEL_API AirflowNetworkEquivalentDuct : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~AirflowNetworkEquivalentDuct() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<StraightComponent> straightComponent() const;
  boost::optional<std::string> coilObjectType() const;
  boost::optional<std::string> heatExchangerObjectType() const;
  boost::optional<std::string> terminalUnitObjectType() const;

  double airPathLength() const;

  double airPathHydraulicDiameter() const;

  //@}
  /** @name Setters */
  //@{

  bool setAirPathLength(double airPathLength);

  bool setAirPathHydraulicDiameter(double airPathHydraulicDiameter);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirflowNetworkEquivalentDuct_Impl ImplType;

  explicit AirflowNetworkEquivalentDuct(std::shared_ptr<detail::AirflowNetworkEquivalentDuct_Impl> impl);

  AirflowNetworkEquivalentDuct(const Model& model, double length, double diameter, const Handle &handle);

  void resetComponent();

  friend class detail::AirTerminalSingleDuctConstantVolumeReheat_Impl;
  friend class detail::AirTerminalSingleDuctVAVReheat_Impl;
  friend class detail::CoilCoolingDXSingleSpeed_Impl;
  friend class detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl;
  friend class detail::CoilCoolingDXMultiSpeed_Impl;
  friend class detail::CoilCoolingWater_Impl;
  friend class detail::CoilHeatingDXSingleSpeed_Impl;
  friend class detail::CoilHeatingElectric_Impl;
  friend class detail::CoilHeatingGas_Impl;
  friend class detail::CoilHeatingDesuperheater_Impl;
  friend class detail::CoilHeatingWater_Impl;
  friend class detail::CoilHeatingDXMultiSpeed_Impl;
  friend class detail::HeatExchangerAirToAirSensibleAndLatent_Impl;

  friend class detail::AirflowNetworkEquivalentDuct_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirflowNetworkEquivalentDuct");
};

/** \relates AirflowNetworkEquivalentDuct*/
typedef boost::optional<AirflowNetworkEquivalentDuct> OptionalAirflowNetworkEquivalentDuct;

/** \relates AirflowNetworkEquivalentDuct*/
typedef std::vector<AirflowNetworkEquivalentDuct> AirflowNetworkEquivalentDuctVector;

} // model
} // openstudio

#endif // MODEL_AIRFLOWNETWORKEQUIVALENTDUCT_HPP

