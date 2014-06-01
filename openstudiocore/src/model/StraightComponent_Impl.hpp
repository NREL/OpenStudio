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

#ifndef MODEL_STRAIGHTCOMPONENT_IMPL_HPP
#define MODEL_STRAIGHTCOMPONENT_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class AirLoopHVAC;

namespace detail {

  class MODEL_API StraightComponent_Impl : public HVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    StraightComponent_Impl(IddObjectType type, Model_Impl* model);

    StraightComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    StraightComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    StraightComponent_Impl(const StraightComponent_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~StraightComponent_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual boost::optional<ParentObject> parent() const;

    virtual std::vector<ModelObject> children() const;

    /** This function returns a vector of HVACComponent that are directly downstream
     *  from this object on an AirLoopHVAC or PlantLoop. 
     *  @param[in]  isDemandComponent  Boolean passed in whether object is a demand or supply component
    **/
    virtual std::vector<HVACComponent> edges(bool isDemandComponent);

    //@}
    /** @name Getters */
    //@{

    virtual unsigned inletPort() = 0;

    virtual unsigned outletPort() = 0;

    virtual boost::optional<ModelObject> inletModelObject();

    virtual boost::optional<ModelObject> outletModelObject();

    virtual boost::optional<AirLoopHVAC> airLoopHVAC() const;

    //@}
    /** @name Setters */
    //@{

    virtual bool addToNode(Node & node);

    virtual std::vector<openstudio::IdfObject> remove();

    void disconnect();

    //@}
   private:

    REGISTER_LOGGER("openstudio.model.StraightComponent");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_STRAIGHTCOMPONENT_IMPL_HPP
