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

#ifndef MODEL_HUMIDIFIERSTEAMELECTRIC_IMPL_HPP
#define MODEL_HUMIDIFIERSTEAMELECTRIC_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class WaterStorageTank;

namespace detail {

  /** HumidifierSteamElectric_Impl is a StraightComponent_Impl that is the implementation class for HumidifierSteamElectric.*/
  class MODEL_API HumidifierSteamElectric_Impl : public StraightComponent_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    HumidifierSteamElectric_Impl(const IdfObject& idfObject,
                                 Model_Impl* model,
                                 bool keepHandle);

    HumidifierSteamElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    HumidifierSteamElectric_Impl(const HumidifierSteamElectric_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    virtual ~HumidifierSteamElectric_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> ratedCapacity() const;

    boost::optional<double> ratedPower() const;

    bool isRatedPowerAutosized() const;

    boost::optional<double> ratedFanPower() const;

    boost::optional<double> standbyPower() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> airInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> airOutletNode() const;

    // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
    boost::optional<WaterStorageTank> waterStorageTank() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setRatedCapacity(boost::optional<double> ratedCapacity);

    void resetRatedCapacity();

    bool setRatedPower(boost::optional<double> ratedPower);

    void resetRatedPower();

    void autosizeRatedPower();

    bool setRatedFanPower(boost::optional<double> ratedFanPower);

    void resetRatedFanPower();

    bool setStandbyPower(boost::optional<double> standbyPower);

    void resetStandbyPower();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirInletNode(const boost::optional<Connection>& connection);

    void resetAirInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirOutletNode(const boost::optional<Connection>& connection);

    void resetAirOutletNode();

    // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
    bool setWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    void resetWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.HumidifierSteamElectric");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_HUMIDIFIERSTEAMELECTRIC_IMPL_HPP

