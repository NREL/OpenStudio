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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** EnergyManagementSystemProgram_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemProgram.*/
  class MODEL_API EnergyManagementSystemProgram_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EnergyManagementSystemProgram_Impl(const IdfObject& idfObject,
                                       Model_Impl* model,
                                       bool keepHandle);

    EnergyManagementSystemProgram_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    EnergyManagementSystemProgram_Impl(const EnergyManagementSystemProgram_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    virtual ~EnergyManagementSystemProgram_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string body() const;

    std::vector<std::string> lines() const;

    std::vector<ModelObject> referencedObjects() const;

    std::vector<std::string> invalidReferencedObjects() const;


    //@}
    /** @name Setters */
    //@{

    bool setBody(const std::string& body);

    bool resetBody();

    bool addLine(const std::string& body);

    bool setLines(const std::vector<std::string>& body);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemProgram");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_IMPL_HPP

