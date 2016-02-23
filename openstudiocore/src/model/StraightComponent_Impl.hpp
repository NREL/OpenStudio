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

    virtual ModelObject clone(Model model) const override;

    virtual boost::optional<ParentObject> parent() const override;

    virtual std::vector<ModelObject> children() const override;

    virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent> & prev) override;

    //@}
    /** @name Getters */
    //@{

    virtual unsigned inletPort() = 0;

    virtual unsigned outletPort() = 0;

    virtual boost::optional<ModelObject> inletModelObject();

    virtual boost::optional<ModelObject> outletModelObject();

    virtual boost::optional<AirLoopHVAC> airLoopHVAC() const override;

    //@}
    /** @name Setters */
    //@{


    //@}
    /** @name Other */
    //@{
    virtual bool addToNode(Node & node) override;

    virtual std::vector<openstudio::IdfObject> remove() override;

    bool removeFromLoop();

    void disconnect() override;

    //@}
   private:

    REGISTER_LOGGER("openstudio.model.StraightComponent");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_STRAIGHTCOMPONENT_IMPL_HPP
