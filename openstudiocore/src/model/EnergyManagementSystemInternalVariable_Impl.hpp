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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** EnergyManagementSystemInternalVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemInternalVariable.*/
  class MODEL_API EnergyManagementSystemInternalVariable_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EnergyManagementSystemInternalVariable_Impl(const IdfObject& idfObject,
                                                Model_Impl* model,
                                                bool keepHandle);

    EnergyManagementSystemInternalVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    EnergyManagementSystemInternalVariable_Impl(const EnergyManagementSystemInternalVariable_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~EnergyManagementSystemInternalVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string internalDataIndexKeyName() const;

    std::string internalDataType() const;

    //@}
    /** @name Setters */
    //@{

    bool setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName);

    void resetInternalDataIndexKeyName();

    bool setInternalDataType(const std::string& internalDataType);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemInternalVariable");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_IMPL_HPP

