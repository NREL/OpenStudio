/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned supplyInletPort() override;

    virtual unsigned supplyOutletPort() override;

    virtual unsigned demandInletPort() override;

    virtual unsigned demandOutletPort() override;

    virtual unsigned tertiaryInletPort() const override;

    virtual unsigned tertiaryOutletPort() const override;

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

