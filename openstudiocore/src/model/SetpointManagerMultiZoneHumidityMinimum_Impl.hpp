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

#ifndef MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMINIMUM_IMPL_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMINIMUM_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  /** SetpointManagerMultiZoneHumidityMinimum_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerMultiZoneHumidityMinimum.*/
  class MODEL_API SetpointManagerMultiZoneHumidityMinimum_Impl : public SetpointManager_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SetpointManagerMultiZoneHumidityMinimum_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    SetpointManagerMultiZoneHumidityMinimum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    SetpointManagerMultiZoneHumidityMinimum_Impl(const SetpointManagerMultiZoneHumidityMinimum_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    virtual ~SetpointManagerMultiZoneHumidityMinimum_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    double minimumSetpointHumidityRatio() const;

    bool isMinimumSetpointHumidityRatioDefaulted() const;

    double maximumSetpointHumidityRatio() const;

    bool isMaximumSetpointHumidityRatioDefaulted() const;

    virtual boost::optional<Node> setpointNode() const override;

    virtual std::string controlVariable() const override;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

    void resetMinimumSetpointHumidityRatio();

    bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

    void resetMaximumSetpointHumidityRatio();

    virtual bool setControlVariable(const std::string& controlVariable) override;

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    virtual bool setSetpointNode(const Node& node) override;

    virtual void resetSetpointNode() override;

    REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneHumidityMinimum");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMINIMUM_IMPL_HPP

