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

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_IMPL_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/SetpointManager_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Node;
class ThermalZone;

namespace detail {

  /** SetpointManagerSingleZoneHumidityMinimum_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerSingleZoneHumidityMinimum.*/
  class MODEL_API SetpointManagerSingleZoneHumidityMinimum_Impl : public SetpointManager_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    SetpointManagerSingleZoneHumidityMinimum_Impl(const IdfObject& idfObject,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    SetpointManagerSingleZoneHumidityMinimum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    SetpointManagerSingleZoneHumidityMinimum_Impl(const SetpointManagerSingleZoneHumidityMinimum_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    virtual ~SetpointManagerSingleZoneHumidityMinimum_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string controlVariable() const;

    bool isControlVariableDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: Node.
    Node setpointNodeorNodeList() const;

    // TODO: Check return type. From object lists, some candidates are: ThermalZone.
    ThermalZone controlZone() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(std::string controlVariable);

    void resetControlVariable();

    // TODO: Check argument type. From object lists, some candidates are: Node.
    bool setSetpointNodeorNodeList(const Node& node);

    // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
    bool setControlZone(const ThermalZone& thermalZone);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneHumidityMinimum");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Node> optionalSetpointNodeorNodeList() const;
    boost::optional<ThermalZone> optionalControlZone() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_IMPL_HPP

