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

// TODO: Check the following class names against object getters and setters.
//class AFNTerminalUnit;
// AFNCoilNames
class CoilCoolingDXSingleSpeed;
class CoilCoolingDXTwoStageWithHumidityControlMode;
class CoilCoolingDXMultiSpeed;
class CoilCoolingWater;
class CoilHeatingDXSingleSpeed;
class CoilHeatingElectric;
class CoilHeatingGas;
class CoilHeatingDesuperheater;
class CoilHeatingWater;
class CoilHeatingDXMultiSpeed;

class StraightComponent;

namespace detail {

  class StraightComponent_Impl;
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

  boost::optional<StraightComponent> component() const;

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

  AirflowNetworkEquivalentDuct(const Model& model, const Handle &handle);

  bool setComponent(const StraightComponent &component);
  void resetComponent();

  friend class detail::StraightComponent_Impl;
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

