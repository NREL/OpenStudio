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

#ifndef MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_IMPL_HPP
#define MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class AirToAirComponent;

namespace detail {

  /** CoilSystemCoolingDXHeatExchangerAssisted_Impl is a StraightComponent_Impl that is the implementation class for CoilSystemCoolingDXHeatExchangerAssisted.*/
  class MODEL_API CoilSystemCoolingDXHeatExchangerAssisted_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilSystemCoolingDXHeatExchangerAssisted_Impl(const IdfObject& idfObject,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    CoilSystemCoolingDXHeatExchangerAssisted_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    CoilSystemCoolingDXHeatExchangerAssisted_Impl(const CoilSystemCoolingDXHeatExchangerAssisted_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    virtual ~CoilSystemCoolingDXHeatExchangerAssisted_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    AirToAirComponent heatExchanger() const;

    StraightComponent coolingCoil() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatExchanger(const AirToAirComponent& heatExchanger);

    bool setCoolingCoil(const StraightComponent& coolingCoil);

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;
    unsigned outletPort() override;

    bool addToNode(Node & node) override;
    std::vector<ModelObject> children() const override;
    ModelObject clone(Model model) const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilSystemCoolingDXHeatExchangerAssisted");

    boost::optional<AirToAirComponent> optionalHeatExchanger() const;
    boost::optional<StraightComponent> optionalCoolingCoil() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_IMPL_HPP

