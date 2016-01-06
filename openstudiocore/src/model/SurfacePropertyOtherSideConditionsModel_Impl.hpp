/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_IMPL_HPP
#define MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** SurfacePropertyOtherSideConditionsModel_Impl is a ResourceObject_Impl that is the implementation class for SurfacePropertyOtherSideConditionsModel.*/
  class MODEL_API SurfacePropertyOtherSideConditionsModel_Impl : public ResourceObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SurfacePropertyOtherSideConditionsModel_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    SurfacePropertyOtherSideConditionsModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    SurfacePropertyOtherSideConditionsModel_Impl(const SurfacePropertyOtherSideConditionsModel_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    virtual ~SurfacePropertyOtherSideConditionsModel_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string typeOfModeling() const;

    bool isTypeOfModelingDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setTypeOfModeling(const std::string& typeOfModeling);

    void resetTypeOfModeling();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyOtherSideConditionsModel");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_IMPL_HPP

