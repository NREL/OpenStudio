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

#ifndef MODEL_AVAILABILITYMANAGEROPTIMUMSTART_IMPL_HPP
#define MODEL_AVAILABILITYMANAGEROPTIMUMSTART_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** AvailabilityManagerOptimumStart_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerOptimumStart.*/
  class MODEL_API AvailabilityManagerOptimumStart_Impl : public AvailabilityManager_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AvailabilityManagerOptimumStart_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    AvailabilityManagerOptimumStart_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    AvailabilityManagerOptimumStart_Impl(const AvailabilityManagerOptimumStart_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~AvailabilityManagerOptimumStart_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    Schedule applicabilitySchedule() const;

    std::string controlType() const;

    boost::optional<ThermalZone> controlZone() const;

    double maximumValueforOptimumStartTime() const;

    std::string controlAlgorithm() const;

    double constantTemperatureGradientduringCooling() const;

    double constantTemperatureGradientduringHeating() const;

    double initialTemperatureGradientduringCooling() const;

    double initialTemperatureGradientduringHeating() const;

    double constantStartTime() const;

    int numberofPreviousDays() const;

    //@}
    /** @name Setters */
    //@{

    bool setApplicabilitySchedule(Schedule& schedule);

    bool setControlType(std::string controlType);

    bool setControlZone(const boost::optional<ThermalZone>& thermalZone);

    void resetControlZone();

    void setMaximumValueforOptimumStartTime(double maximumValueforOptimumStartTime);

    bool setControlAlgorithm(std::string controlAlgorithm);

    void setConstantTemperatureGradientduringCooling(double constantTemperatureGradientduringCooling);

    void setConstantTemperatureGradientduringHeating(double constantTemperatureGradientduringHeating);

    void setInitialTemperatureGradientduringCooling(double initialTemperatureGradientduringCooling);

    void setInitialTemperatureGradientduringHeating(double initialTemperatureGradientduringHeating);

    void setConstantStartTime(double constantStartTime);

    bool setNumberofPreviousDays(int numberofPreviousDays);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerOptimumStart");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalApplicabilitySchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGEROPTIMUMSTART_IMPL_HPP

