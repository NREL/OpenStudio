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

#ifndef MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_IMPL_HPP
#define MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Node;

namespace detail {

  /** AirTerminalDualDuctConstantVolume_Impl is a Mixer_Impl that is the implementation class for AirTerminalDualDuctConstantVolume.*/
  class MODEL_API AirTerminalDualDuctConstantVolume_Impl : public Mixer_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalDualDuctConstantVolume_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle);

    AirTerminalDualDuctConstantVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    AirTerminalDualDuctConstantVolume_Impl(const AirTerminalDualDuctConstantVolume_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    virtual ~AirTerminalDualDuctConstantVolume_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    //@}
    /** @name Setters */
    //@{

  // Note Schedules are passed by reference, not const reference.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumAirFlowRate(double maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    //@}
    /** @name Other */
    //@{

    unsigned outletPort() const;

    unsigned inletPort(unsigned branchIndex) const;

    unsigned nextInletPort() const;

    unsigned newInletPortAfterBranch(unsigned branchIndex);

    void removePortForBranch(unsigned branchIndex);

    boost::optional<Node> HotAirInletNode() const;

    boost::optional<Node> ColdAirInletNode() const;

    bool addToNode(Node & node) override;

    std::vector<IdfObject> remove() override;

    virtual ModelObject clone(Model model) const override;

    bool isRemovable() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctConstantVolume");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_IMPL_HPP

