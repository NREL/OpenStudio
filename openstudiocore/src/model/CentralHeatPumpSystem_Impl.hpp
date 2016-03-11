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

#ifndef MODEL_CENTRALHEATPUMPSYSTEM_IMPL_HPP
#define MODEL_CENTRALHEATPUMPSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ModelObjectList;
class CentralHeatPumpSystemModule;

namespace detail {

  /** CentralHeatPumpSystem_Impl is a WaterToWaterComponent_Impl that is the implementation class for CentralHeatPumpSystem.*/
  class MODEL_API CentralHeatPumpSystem_Impl : public WaterToWaterComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CentralHeatPumpSystem_Impl(const IdfObject& idfObject,
                               Model_Impl* model,
                               bool keepHandle);

    CentralHeatPumpSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    CentralHeatPumpSystem_Impl(const CentralHeatPumpSystem_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    virtual ~CentralHeatPumpSystem_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned supplyInletPort() override;

    virtual unsigned supplyOutletPort() override;

    virtual unsigned demandInletPort() override;

    virtual unsigned demandOutletPort() override;

    //@}
    /** @name Getters */
    //@{

    std::string controlMethod() const;

    double ancillaryPower() const;

    boost::optional<Schedule> ancillaryOperationSchedule() const;

    boost::optional<ModelObjectList> chillerHeaterModuleList() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlMethod(std::string controlMethod);

    bool setAncillaryPower(double ancillaryPower);

    bool setAncillaryOperationSchedule(Schedule& schedule);

    void resetAncillaryOperationSchedule();

    //@}
    /** @name Other */
    //@{

    bool addModule( const CentralHeatPumpSystemModule & centralHeatPumpSystemModule);

    void removeModule( const CentralHeatPumpSystemModule & centralHeatPumpSystemModule);

    void removeAllModules();

    std::vector<CentralHeatPumpSystemModule> modules() const;

    bool setChillerHeaterModuleList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetChillerHeaterModuleList();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystem");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CENTRALHEATPUMPSYSTEM_IMPL_HPP

