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

#ifndef MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** AirTerminalSingleDuctInletSideMixer_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctInletSideMixer.*/
  class MODEL_API AirTerminalSingleDuctInletSideMixer_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctInletSideMixer_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    AirTerminalSingleDuctInletSideMixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    AirTerminalSingleDuctInletSideMixer_Impl(const AirTerminalSingleDuctInletSideMixer_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~AirTerminalSingleDuctInletSideMixer_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{


    //@}
    /** @name Setters */
    //@{


    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;
    unsigned outletPort() override;
    bool addToNode(Node& node) override;
    std::vector<openstudio::IdfObject> remove() override;
    bool isRemovable() const override;

    unsigned secondaryAirInletPort() const;
    boost::optional<Node> secondaryAirInletNode() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctInletSideMixer");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_IMPL_HPP

