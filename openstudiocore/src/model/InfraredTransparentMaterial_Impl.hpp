/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_INFRAREDTRANSPARENTMATERIAL_IMPL_HPP
#define MODEL_INFRAREDTRANSPARENTMATERIAL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelPartitionMaterial_Impl.hpp>

namespace openstudio {
namespace model {

namespace detail {

  /** InfraredTransparentMaterial_Impl is a ModelPartitionMaterial_Impl that is the implementation class for InfraredTransparentMaterial.*/
  class MODEL_API InfraredTransparentMaterial_Impl : public ModelPartitionMaterial_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    InfraredTransparentMaterial_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle);

    InfraredTransparentMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    InfraredTransparentMaterial_Impl(const InfraredTransparentMaterial_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    virtual ~InfraredTransparentMaterial_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.InfraredTransparentMaterial");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_INFRAREDTRANSPARENTMATERIAL_IMPL_HPP

