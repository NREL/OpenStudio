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

#ifndef MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_IMPL_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PhotovoltaicPerformance_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** PhotovoltaicPerformanceSimple_Impl is a PhotovoltaicPerformance_Impl that is the implementation class for PhotovoltaicPerformanceSimple.*/
  class MODEL_API PhotovoltaicPerformanceSimple_Impl : public PhotovoltaicPerformance_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    PhotovoltaicPerformanceSimple_Impl(const IdfObject& idfObject,
                                       Model_Impl* model,
                                       bool keepHandle);

    PhotovoltaicPerformanceSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    PhotovoltaicPerformanceSimple_Impl(const PhotovoltaicPerformanceSimple_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    virtual ~PhotovoltaicPerformanceSimple_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    double fractionOfSurfaceAreaWithActiveSolarCells() const;

    bool isfractionOfSurfaceAreaWithActiveSolarCellsDefaulted() const;

    std::string conversionEfficiencyInputMode() const;

    boost::optional<double> fixedEfficiency() const;

    boost::optional<Schedule> efficiencySchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setFractionOfSurfaceAreaWithActiveSolarCells(double fractionOfSurfaceAreaWithActiveSolarCells);

    void resetFractionOfSurfaceAreaWithActiveSolarCells();

    bool setConversionEfficiencyInputMode(const std::string& conversionEfficiencyInputMode);

    bool setFixedEfficiency(double fixedEfficiency);

    void resetFixedEfficiency();

    bool setEfficiencySchedule(Schedule& schedule);

    void resetEfficiencySchedule();

    void setDefaultFixedEfficiency();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformanceSimple");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_IMPL_HPP

