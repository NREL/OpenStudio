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

#ifndef MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_IMPL_HPP
#define MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Node;
class DesignSpecificationOutdoorAir;

namespace detail {

  /** AirTerminalDualDuctVAVOutdoorAir_Impl is a Mixer_Impl that is the implementation class for AirTerminalDualDuctVAVOutdoorAir.*/
  class MODEL_API AirTerminalDualDuctVAVOutdoorAir_Impl : public Mixer_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalDualDuctVAVOutdoorAir_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    AirTerminalDualDuctVAVOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    AirTerminalDualDuctVAVOutdoorAir_Impl(const AirTerminalDualDuctVAVOutdoorAir_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~AirTerminalDualDuctVAVOutdoorAir_Impl() {}

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

    unsigned outletPort() const;

    unsigned inletPort(unsigned branchIndex) const;

    unsigned nextInletPort() const;

    unsigned newInletPortAfterBranch(unsigned branchIndex);

    void removePortForBranch(unsigned branchIndex);

    boost::optional<Node> OutdoorAirInletNode() const;

    boost::optional<Node> RecirculatedAirInletNode() const;

    boost::optional<double> maximumTerminalAirFlowRate() const;

    bool isMaximumTerminalAirFlowRateAutosized() const;

    std::string perPersonVentilationRateMode() const;

    //@}
    /** @name Setters */
    //@{

  // Note Schedules are passed by reference, not const reference.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate);

    void autosizeMaximumTerminalAirFlowRate();

    bool setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir);

    bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAVOutdoorAir");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<DesignSpecificationOutdoorAir> optionalDesignSpecificationOutdoorAirObject() const;
    std::vector<IdfObject> remove();
    bool addToNode(Node & node);
    ModelObject clone(Model model) const;
    bool isRemovable() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_IMPL_HPP

