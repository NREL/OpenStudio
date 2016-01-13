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

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Node;

namespace detail {

  /** AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctVAVHeatAndCoolNoReheat.*/
  class MODEL_API AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    virtual ~AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    std::string airOutlet() const;

    std::string airInlet() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    double zoneMinimumAirFlowFraction() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    void setAirOutlet(std::string airOutlet);

    void setAirInlet(std::string airInlet);

    bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;

    unsigned outletPort() override;

    bool addToNode(Node & node) override;

    std::vector<IdfObject> remove() override;

    bool isRemovable() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVHeatAndCoolNoReheat");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_IMPL_HPP

