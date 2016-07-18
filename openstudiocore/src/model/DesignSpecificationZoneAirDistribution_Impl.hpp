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

#ifndef MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_IMPL_HPP
#define MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** DesignSpecificationZoneAirDistribution_Impl is a ModelObject_Impl that is the implementation class for DesignSpecificationZoneAirDistribution.*/
  class MODEL_API DesignSpecificationZoneAirDistribution_Impl : public ModelObject_Impl {
    

    
    
    
    

    
    
    
    

    
    
    
    

    
   public:
    /** @name Constructors and Destructors */
    //@{

    DesignSpecificationZoneAirDistribution_Impl(const IdfObject& idfObject,
                                                Model_Impl* model,
                                                bool keepHandle);

    DesignSpecificationZoneAirDistribution_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    DesignSpecificationZoneAirDistribution_Impl(const DesignSpecificationZoneAirDistribution_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~DesignSpecificationZoneAirDistribution_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    double zoneAirDistributionEffectivenessinCoolingMode() const;

    Quantity getZoneAirDistributionEffectivenessinCoolingMode(bool returnIP=false) const;

    bool isZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

    double zoneAirDistributionEffectivenessinHeatingMode() const;

    Quantity getZoneAirDistributionEffectivenessinHeatingMode(bool returnIP=false) const;

    bool isZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

    boost::optional<Schedule> zoneAirDistributionEffectivenessSchedule() const;

    double zoneSecondaryRecirculationFraction() const;

    Quantity getZoneSecondaryRecirculationFraction(bool returnIP=false) const;

    bool isZoneSecondaryRecirculationFractionDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setZoneAirDistributionEffectivenessinCoolingMode(double zoneAirDistributionEffectivenessinCoolingMode);

    bool setZoneAirDistributionEffectivenessinCoolingMode(const Quantity& zoneAirDistributionEffectivenessinCoolingMode);

    void resetZoneAirDistributionEffectivenessinCoolingMode();

    bool setZoneAirDistributionEffectivenessinHeatingMode(double zoneAirDistributionEffectivenessinHeatingMode);

    bool setZoneAirDistributionEffectivenessinHeatingMode(const Quantity& zoneAirDistributionEffectivenessinHeatingMode);

    void resetZoneAirDistributionEffectivenessinHeatingMode();

    bool setZoneAirDistributionEffectivenessSchedule(Schedule& schedule);

    void resetZoneAirDistributionEffectivenessSchedule();

    bool setZoneSecondaryRecirculationFraction(double zoneSecondaryRecirculationFraction);

    bool setZoneSecondaryRecirculationFraction(const Quantity& zoneSecondaryRecirculationFraction);

    void resetZoneSecondaryRecirculationFraction();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.DesignSpecificationZoneAirDistribution");

    openstudio::Quantity zoneAirDistributionEffectivenessinCoolingMode_SI() const;
    openstudio::Quantity zoneAirDistributionEffectivenessinCoolingMode_IP() const;
    openstudio::Quantity zoneAirDistributionEffectivenessinHeatingMode_SI() const;
    openstudio::Quantity zoneAirDistributionEffectivenessinHeatingMode_IP() const;
    openstudio::Quantity zoneSecondaryRecirculationFraction_SI() const;
    openstudio::Quantity zoneSecondaryRecirculationFraction_IP() const;

    boost::optional<ModelObject> zoneAirDistributionEffectivenessScheduleAsModelObject() const;

    bool setZoneAirDistributionEffectivenessScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_IMPL_HPP

