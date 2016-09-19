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

    std::string eMSVariableName() const;

    std::string updateFrequency() const;

    boost::optional<std::string> eMSProgramorSubroutineName() const;

    std::string resourceType() const;

    std::string groupType() const;

    std::string endUseCategory() const;

    boost::optional<std::string> endUseSubcategory() const;

    boost::optional<std::string> units() const;

    //@}
    /** @name Setters */
    //@{

    void setEMSVariableName(const std::string& eMSVariableName);

    bool setUpdateFrequency(const std::string& updateFrequency);

    bool setEMSProgramorSubroutineName(const EnergyManagementSystemProgram& program);

    bool setEMSProgramorSubroutineName(const EnergyManagementSystemSubroutine& subroutine);

    void resetEMSProgramorSubroutineName();

    bool setResourceType(const std::string& resourceType);

    bool setGroupType(const std::string& groupType);

    bool setEndUseCategory(const std::string& endUseCategory);

    void setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    void setUnits(const std::string& units);

    void resetUnits();

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

