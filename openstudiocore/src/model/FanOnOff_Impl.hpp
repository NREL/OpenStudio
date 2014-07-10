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

#ifndef MODEL_FANONOFF_IMPL_HPP
#define MODEL_FANONOFF_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;

namespace detail {

  /** FanOnOff_Impl is a StraightComponent_Impl that is the implementation class for FanOnOff.*/
  class MODEL_API FanOnOff_Impl : public StraightComponent_Impl 
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    FanOnOff_Impl(const IdfObject& idfObject,
                  Model_Impl* model,
                  bool keepHandle);

    FanOnOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    FanOnOff_Impl(const FanOnOff_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    virtual ~FanOnOff_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;
    
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual std::vector<ModelObject> children() const;

    virtual bool addToNode(Node & node);

    //@}
    /** @name Getters and Setters */
    //@{

    // Field Availability Schedule Name

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    // Field Fan Efficiency

    double fanEfficiency() const;

    bool isFanEfficiencyDefaulted() const;

    bool setFanEfficiency(double fanEfficiency);

    void resetFanEfficiency();

    // Field Pressure Rise

    double pressureRise() const;

    void setPressureRise(double pressureRise);

    // Field Maximum Flow Rate

    boost::optional<double> maximumFlowRate() const;

    bool isMaximumFlowRateAutosized() const;

    bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

    void resetMaximumFlowRate();

    void autosizeMaximumFlowRate();

    // Field Motor Efficiency

    double motorEfficiency() const;

    bool isMotorEfficiencyDefaulted() const;

    bool setMotorEfficiency(double motorEfficiency);

    void resetMotorEfficiency();

    // Field Motor In Airstream Fraction

    boost::optional<double> motorInAirstreamFraction() const;

    bool setMotorInAirstreamFraction(boost::optional<double> motorInAirstreamFraction);

    void resetMotorInAirstreamFraction();

    // Field End-Use Subcategory

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Curves */
    //@{

    Curve fanPowerRatioFunctionofSpeedRatioCurve() const;
    bool setFanPowerRatioFunctionofSpeedRatioCurve( const Curve& curve );

    Curve fanEfficiencyRatioFunctionofSpeedRatioCurve() const;
    bool setFanEfficiencyRatioFunctionofSpeedRatioCurve( const Curve& curve );

    //@}
   //protected:
   private:
    REGISTER_LOGGER("openstudio.model.FanOnOff");

    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    virtual boost::optional<HVACComponent> containingHVACComponent() const;

  };



} // detail

} // model
} // openstudio

#endif // MODEL_FANONOFF_IMPL_HPP

