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

#ifndef MODEL_FANCONSTANTVOLUME_IMPL_HPP
#define MODEL_FANCONSTANTVOLUME_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  class MODEL_API FanConstantVolume_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(double fanEfficiency READ fanEfficiency WRITE setFanEfficiency);
    Q_PROPERTY(double pressureRise READ pressureRise WRITE setPressureRise);
    Q_PROPERTY(boost::optional<double> maximumFlowRate READ maximumFlowRate WRITE setMaximumFlowRate);
    Q_PROPERTY(double motorEfficiency READ motorEfficiency WRITE setMotorEfficiency);
    Q_PROPERTY(double motorInAirstreamFraction READ motorInAirstreamFraction WRITE setMotorInAirstreamFraction);
    Q_PROPERTY(std::string endUseSubcategory READ endUseSubcategory WRITE setEndUseSubcategory);
    Q_PROPERTY( boost::optional<openstudio::model::ModelObject> availabilitySchedule
                READ availabilityScheduleAsModelObject
                WRITE setAvailibiltyScheduleAsModelObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    FanConstantVolume_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    FanConstantVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    // copy constructor
    FanConstantVolume_Impl(const FanConstantVolume_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    // virtual destructor
    virtual ~FanConstantVolume_Impl();

    //@}
    /** @name Virtual Methods */
    //@{

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToNode(Node & node);

    //@}
    /** @name Getters and Setters */
    //@{

    Schedule availabilitySchedule() const;
    bool setAvailabilitySchedule(Schedule& s);

    // Get FanEfficiency
    double fanEfficiency();

    // Set fanEfficiency
    void setFanEfficiency(double val);

    // Get PressureRise
    double pressureRise();

    // Set PressureRise
    void setPressureRise(double val);

    // Get MotorEfficiency
    double motorEfficiency();

    // Set MotorEfficiency
    void setMotorEfficiency(double val);

    // Get MotorInAirstreamFraction
    double motorInAirstreamFraction();

    // Set MotorInAirstreamFraction
    void setMotorInAirstreamFraction(double val);

    // Get EndUseSubcategory
    std::string endUseSubcategory();

    // Set EndUseSubcategory
    void setEndUseSubcategory(std::string val);

    boost::optional<double> maximumFlowRate() const;

    OSOptionalQuantity getMaximumFlowRate(bool returnIP=false) const;

    bool isMaximumFlowRateAutosized() const;

    bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

    bool setMaximumFlowRate(const OSOptionalQuantity& maximumFlowRate);

    void resetMaximumFlowRate();

    void autosizeMaximumFlowRate();

    //@}
   private:

    REGISTER_LOGGER("openstudio.model.FanConstantVolume");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailibiltyScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

    virtual boost::optional<HVACComponent> containingHVACComponent() const;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_FANCONSTANTVOLUME_IMPL_HPP

