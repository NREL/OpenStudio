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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class OutputVariableMeter;

namespace detail {

  /** EnergyManagementSystemSensor_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemSensor.*/
  class MODEL_API EnergyManagementSystemSensor_Impl : public ModelObject_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    EnergyManagementSystemSensor_Impl(const IdfObject& idfObject,
                                      Model_Impl* model,
                                      bool keepHandle);

    EnergyManagementSystemSensor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    EnergyManagementSystemSensor_Impl(const EnergyManagementSystemSensor_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    virtual ~EnergyManagementSystemSensor_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<std::string> outputVariableorOutputMeterIndexKeyName() const;

    // TODO: Check return type. From object lists, some candidates are: OutputVariableMeter.
    OutputVariableMeter outputVariableorOutputMeter() const;

    //@}
    /** @name Setters */
    //@{

    void setOutputVariableorOutputMeterIndexKeyName(const std::string& outputVariableorOutputMeterIndexKeyName);

    void resetOutputVariableorOutputMeterIndexKeyName();

    // TODO: Check argument type. From object lists, some candidates are: OutputVariableMeter.
    bool setOutputVariableorOutputMeter(const OutputVariableMeter& outputVariableMeter);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemSensor");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<OutputVariableMeter> optionalOutputVariableorOutputMeter() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_IMPL_HPP

