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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"
#include "EnergyManagementSystemSubroutine_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** EnergyManagementSystemMeteredOutputVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemMeteredOutputVariable.*/
  class MODEL_API EnergyManagementSystemMeteredOutputVariable_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EnergyManagementSystemMeteredOutputVariable_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    EnergyManagementSystemMeteredOutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    EnergyManagementSystemMeteredOutputVariable_Impl(const EnergyManagementSystemMeteredOutputVariable_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    virtual ~EnergyManagementSystemMeteredOutputVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string emsVariableName() const;

    std::string updateFrequency() const;

    std::string emsProgramOrSubroutineName() const;

    boost::optional<EnergyManagementSystemProgram> emsProgram() const;

    boost::optional<EnergyManagementSystemSubroutine> emsSubroutine() const;

    std::string resourceType() const;

    std::string groupType() const;

    std::string endUseCategory() const;

    std::string endUseSubcategory() const;

    std::string units() const;

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

    bool setUpdateFrequency(const std::string& updateFrequency);

    bool setEMSProgramOrSubroutineName(const EnergyManagementSystemProgram& program);

    bool setEMSProgramOrSubroutineName(const EnergyManagementSystemSubroutine& subroutine);

    void resetEMSProgramOrSubroutineName();

    bool setResourceType(const std::string& resourceType);

    bool setGroupType(const std::string& groupType);

    bool setEndUseCategory(const std::string& endUseCategory);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    bool setUnits(const std::string& units);

    void resetUnits();

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
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemMeteredOutputVariable");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_IMPL_HPP

