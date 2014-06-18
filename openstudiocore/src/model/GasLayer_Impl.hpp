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

#ifndef MODEL_GASLAYER_IMPL_HPP
#define MODEL_GASLAYER_IMPL_HPP

#include "FenestrationMaterial_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class GasLayer;

namespace detail {

  class MODEL_API GasLayer_Impl : public FenestrationMaterial_Impl {
   public:
   
    /** @name Constructors and Destructors */
    //@{

    // Construct completely new object.
    GasLayer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed 
    // from Workspace).
    GasLayer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                  Model_Impl* model, 
                  bool keepHandle);

    // Clone copy constructor.
    GasLayer_Impl(const GasLayer_Impl& other,Model_Impl* model,bool keepHandle);

    virtual ~GasLayer_Impl() {}

    //@}
    /** @name Getters */
    //@{

    virtual boost::optional<double> getVisibleTransmittance() const;

    virtual boost::optional<double> interiorVisibleAbsorptance() const;

    virtual boost::optional<double> exteriorVisibleAbsorptance() const;

    //@}

   private:

    REGISTER_LOGGER("openstudio.model.GasLayer");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GASLAYER_IMPL_HPP
