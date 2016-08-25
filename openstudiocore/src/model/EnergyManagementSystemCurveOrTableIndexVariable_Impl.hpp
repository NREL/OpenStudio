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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class AllCurves;

namespace detail {

  /** EnergyManagementSystemCurveOrTableIndexVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemCurveOrTableIndexVariable.*/
  class MODEL_API EnergyManagementSystemCurveOrTableIndexVariable_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EnergyManagementSystemCurveOrTableIndexVariable_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle);

    EnergyManagementSystemCurveOrTableIndexVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle);

    EnergyManagementSystemCurveOrTableIndexVariable_Impl(const EnergyManagementSystemCurveOrTableIndexVariable_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle);

    virtual ~EnergyManagementSystemCurveOrTableIndexVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: AllCurves.
    ModelObject curveorTableObject() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: AllCurves.
    bool setCurveorTableObject(const ModelObject& allCurves);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemCurveOrTableIndexVariable");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<ModelObject> optionalCurveorTableObject() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_IMPL_HPP

