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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** EnergyManagementSystemTrendVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemTrendVariable.*/
  class MODEL_API EnergyManagementSystemTrendVariable_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EnergyManagementSystemTrendVariable_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    EnergyManagementSystemTrendVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    EnergyManagementSystemTrendVariable_Impl(const EnergyManagementSystemTrendVariable_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~EnergyManagementSystemTrendVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string emsVariableName() const;

    int numberOfTimestepsToBeLogged() const;

    boost::optional<ModelObject> emsVariableObject() const;

    boost::optional<EnergyManagementSystemActuator> emsActuator() const;
    boost::optional<EnergyManagementSystemSensor> emsSensor() const;
    boost::optional<EnergyManagementSystemGlobalVariable> emsGlobalVariable() const;
    boost::optional<EnergyManagementSystemTrendVariable> emsTrendVariable() const;
    boost::optional<EnergyManagementSystemInternalVariable> emsInternalVariable() const;
    boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> emsCurveOrTableIndexVariable() const;
    boost::optional<EnergyManagementSystemConstructionIndexVariable> emsConstructionIndexVariable() const;

    //@}
    /** @name Setters */
    //@{

    bool setEMSVariableName(const std::string& eMSVariableName);

    bool setNumberOfTimestepsToBeLogged(int numberofTimestepstobeLogged);

    bool setEMSVariableName(const EnergyManagementSystemActuator& object);
    bool setEMSVariableName(const EnergyManagementSystemSensor& object);
    bool setEMSVariableName(const EnergyManagementSystemGlobalVariable& object);
    bool setEMSVariableName(const EnergyManagementSystemTrendVariable& object);
    bool setEMSVariableName(const EnergyManagementSystemInternalVariable& object);
    bool setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object);
    bool setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemTrendVariable");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_IMPL_HPP

