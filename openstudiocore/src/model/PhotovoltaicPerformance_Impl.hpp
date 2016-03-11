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

#ifndef MODEL_PHOTOVOLTAICPERFORMANCE_IMPL_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCE_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API PhotovoltaicPerformance_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    PhotovoltaicPerformance_Impl(IddObjectType type, Model_Impl* model);

    PhotovoltaicPerformance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    PhotovoltaicPerformance_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    PhotovoltaicPerformance_Impl(const PhotovoltaicPerformance_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~PhotovoltaicPerformance_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /// do not allow this object to be removed if it is referenced by a PhotovoltaicGenerator
    virtual std::vector<openstudio::IdfObject> remove() override;

    //@}
    /** @name Getters */
    //@{

    // DLM: really don't want to implement this for this object, feels too kludgy
    //virtual boost::optional<ParentObject> parent() override;

    //@}
    /** @name Setters */
    //@{

    // DLM: really don't want to implement this for this object, feels too kludgy
    //virtual bool setParent(ParentObject& newParent) override;

    //@}
    /** @name Other */
    //@{
 
    //@}
   private:

    REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformance");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PHOTOVOLTAICPERFORMANCE_IMPL_HPP
