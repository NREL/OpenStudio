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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"
#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"
#include "OutputMeter.hpp"
#include "OutputMeter_Impl.hpp"

namespace openstudio {

namespace energyplus {
  class ReverseTranslator;
}

namespace model {

class OutputVariable;
class OutputMeter;

namespace detail {

  class EnergyManagementSystemSensor_Impl;

} // detail

/** EnergyManagementSystemSensor is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:Sensor'. */
class MODEL_API EnergyManagementSystemSensor : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemSensor(const Model& model, const OutputVariable& outvar);

  explicit EnergyManagementSystemSensor(const Model& model, const OutputMeter& outvar);

  explicit EnergyManagementSystemSensor(const Model& model, const std::string& outputVariableOrMeterName);

  virtual ~EnergyManagementSystemSensor() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  std::string keyName() const;

  boost::optional<OutputVariable> outputVariable() const;
  boost::optional<OutputMeter> outputMeter() const;
  std::string outputVariableOrMeterName() const;

  //@}
  /** @name Setters */
  //@{

  bool setKeyName(const std::string& keyName);

  bool setOutputVariable(const OutputVariable& outputVariable);
  bool setOutputMeter(const OutputMeter& outputMeter);
  bool setOutputVariableOrMeterName(const std::string& outputVariableOrMeterName);
  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemSensor_Impl ImplType;

  explicit EnergyManagementSystemSensor(std::shared_ptr<detail::EnergyManagementSystemSensor_Impl> impl);

  friend class detail::EnergyManagementSystemSensor_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class energyplus::ReverseTranslator;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemSensor");
  explicit EnergyManagementSystemSensor(const Model& model);
};

/** \relates EnergyManagementSystemSensor*/
typedef boost::optional<EnergyManagementSystemSensor> OptionalEnergyManagementSystemSensor;

/** \relates EnergyManagementSystemSensor*/
typedef std::vector<EnergyManagementSystemSensor> EnergyManagementSystemSensorVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_HPP

