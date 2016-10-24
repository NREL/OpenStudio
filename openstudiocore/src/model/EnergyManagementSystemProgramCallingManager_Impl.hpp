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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** EnergyManagementSystemProgramCallingManager_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemProgramCallingManager.*/
  class MODEL_API EnergyManagementSystemProgramCallingManager_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EnergyManagementSystemProgramCallingManager_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    EnergyManagementSystemProgramCallingManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    EnergyManagementSystemProgramCallingManager_Impl(const EnergyManagementSystemProgramCallingManager_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    virtual ~EnergyManagementSystemProgramCallingManager_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string callingPoint() const;

    std::vector<EnergyManagementSystemProgram> programs() const;

    boost::optional<EnergyManagementSystemProgram> getProgram(unsigned index) const;

    //@}
    /** @name Setters */
    //@{

    bool setCallingPoint(const std::string& callingPoint);

    bool eraseProgram(unsigned index);

    void erasePrograms();

    bool addProgram(const EnergyManagementSystemProgram& program);

    bool setProgram(const EnergyManagementSystemProgram& program, unsigned index);

    bool setPrograms(const std::vector<EnergyManagementSystemProgram>& programs);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemProgramCallingManager");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_IMPL_HPP

