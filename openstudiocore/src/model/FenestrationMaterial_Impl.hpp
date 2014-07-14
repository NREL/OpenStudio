/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_FENESTRATIONMATERIAL_IMPL_HPP
#define MODEL_FENESTRATIONMATERIAL_IMPL_HPP

#include "Material_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class FenestrationMaterial;

namespace detail {

  class MODEL_API FenestrationMaterial_Impl : public Material_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    // Construct completely new object.
    FenestrationMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed 
    // from Workspace).
    FenestrationMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                              Model_Impl* model, 
                              bool keepHandle);

    // Clone copy constructor.
    FenestrationMaterial_Impl(const FenestrationMaterial_Impl& other, 
                              Model_Impl* model, 
                              bool keepHandle);

    virtual ~FenestrationMaterial_Impl() {}

    //@}

   private:

    REGISTER_LOGGER("openstudio.model.FenestrationMaterial");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_FENESTRATIONMATERIAL_IMPL_HPP
