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

#ifndef MODEL_DESIGNSPECIFICATIONOUTDOORAIR_IMPL_HPP
#define MODEL_DESIGNSPECIFICATIONOUTDOORAIR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** DesignSpecificationOutdoorAir_Impl is a ModelObject_Impl that is the
   *  implementation class for DesignSpecificationOutdoorAir.*/
  class MODEL_API DesignSpecificationOutdoorAir_Impl : public ResourceObject_Impl {
    

    
    
    

    
    
    
    

    
    
    
    

    
    
    
    

    
    
    
    

    
   public:
    /** @name Constructors and Destructors */
    //@{

    DesignSpecificationOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    DesignSpecificationOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    DesignSpecificationOutdoorAir_Impl(const DesignSpecificationOutdoorAir_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    virtual ~DesignSpecificationOutdoorAir_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    std::string outdoorAirMethod() const;

    bool isOutdoorAirMethodDefaulted() const;

    double outdoorAirFlowperPerson() const;

    Quantity getOutdoorAirFlowperPerson(bool returnIP=false) const;

    bool isOutdoorAirFlowperPersonDefaulted() const;

    double outdoorAirFlowperFloorArea() const;

    Quantity getOutdoorAirFlowperFloorArea(bool returnIP=false) const;

    bool isOutdoorAirFlowperFloorAreaDefaulted() const;

    double outdoorAirFlowRate() const;

    Quantity getOutdoorAirFlowRate(bool returnIP=false) const;

    bool isOutdoorAirFlowRateDefaulted() const;

    double outdoorAirFlowAirChangesperHour() const;

    Quantity getOutdoorAirFlowAirChangesperHour(bool returnIP=false) const;

    bool isOutdoorAirFlowAirChangesperHourDefaulted() const;

    boost::optional<Schedule> outdoorAirFlowRateFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setOutdoorAirMethod(std::string outdoorAirMethod);

    void resetOutdoorAirMethod();

    bool setOutdoorAirFlowperPerson(double outdoorAirFlowperPerson);

    bool setOutdoorAirFlowperPerson(const Quantity& outdoorAirFlowperPerson);

    void resetOutdoorAirFlowperPerson();

    bool setOutdoorAirFlowperFloorArea(double outdoorAirFlowperFloorArea);

    bool setOutdoorAirFlowperFloorArea(const Quantity& outdoorAirFlowperFloorArea);

    void resetOutdoorAirFlowperFloorArea();

    bool setOutdoorAirFlowRate(double outdoorAirFlowRate);

    bool setOutdoorAirFlowRate(const Quantity& outdoorAirFlowRate);

    void resetOutdoorAirFlowRate();

    bool setOutdoorAirFlowAirChangesperHour(double outdoorAirFlowAirChangesperHour);

    bool setOutdoorAirFlowAirChangesperHour(const Quantity& outdoorAirFlowAirChangesperHour);

    void resetOutdoorAirFlowAirChangesperHour();

    bool setOutdoorAirFlowRateFractionSchedule(Schedule& schedule);

    void resetOutdoorAirFlowRateFractionSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.DesignSpecificationOutdoorAir");

    std::vector<std::string> outdoorAirMethodValues() const;
    openstudio::Quantity outdoorAirFlowperPerson_SI() const;
    openstudio::Quantity outdoorAirFlowperPerson_IP() const;
    openstudio::Quantity outdoorAirFlowperFloorArea_SI() const;
    openstudio::Quantity outdoorAirFlowperFloorArea_IP() const;
    openstudio::Quantity outdoorAirFlowRate_SI() const;
    openstudio::Quantity outdoorAirFlowRate_IP() const;
    openstudio::Quantity outdoorAirFlowAirChangesperHour_SI() const;
    openstudio::Quantity outdoorAirFlowAirChangesperHour_IP() const;

    boost::optional<ModelObject> outdoorAirFlowRateFractionScheduleAsModelObject() const;

    bool setOutdoorAirFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_DESIGNSPECIFICATIONOUTDOORAIR_IMPL_HPP

