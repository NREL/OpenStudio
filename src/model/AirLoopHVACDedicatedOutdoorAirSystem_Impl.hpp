/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACDEDICATEDOUTDOORAIRSYSTEM_IMPL_HPP
#define MODEL_AIRLOOPHVACDEDICATEDOUTDOORAIRSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class AirLoopHVACOutdoorAirSystem;
  class Schedule;
  class AirLoopHVAC;

  namespace detail {

    /** AirLoopHVACDedicatedOutdoorAirSystem_Impl is a ModelObject_Impl that is the implementation class for AirLoopHVACDedicatedOutdoorAirSystem.*/
    class MODEL_API AirLoopHVACDedicatedOutdoorAirSystem_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirLoopHVACDedicatedOutdoorAirSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVACDedicatedOutdoorAirSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVACDedicatedOutdoorAirSystem_Impl(const AirLoopHVACDedicatedOutdoorAirSystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirLoopHVACDedicatedOutdoorAirSystem_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      AirLoopHVACOutdoorAirSystem airLoopHVACOutdoorAirSystem() const;

      Schedule availabilitySchedule() const;

      double preheatDesignTemperature() const;

      double preheatDesignHumidityRatio() const;

      double precoolDesignTemperature() const;

      double precoolDesignHumidityRatio() const;

      unsigned int numberofAirLoops() const;

      std::vector<AirLoopHVAC> airLoops() const;

      boost::optional<unsigned> airLoopIndex(const AirLoopHVAC& airLoopHVAC) const;

      //@}
      /** @name Setters */
      //@{

      bool setAirLoopHVACOutdoorAirSystem(const AirLoopHVACOutdoorAirSystem& airLoopHVACOutdoorAirSystem);

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setPreheatDesignTemperature(double preheatDesignTemperature);

      bool setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio);

      bool setPrecoolDesignTemperature(double precoolDesignTemperature);

      bool setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio);

      bool addAirLoop(const AirLoopHVAC& airLoopHVAC);

      bool removeAirLoop(const AirLoopHVAC& airLoopHVAC);

      bool removeAirLoop(unsigned groupIndex);

      void removeAllAirLoops();

      virtual std::vector<openstudio::IdfObject> remove() override;

      bool addAirLoops(const std::vector<AirLoopHVAC>& airLoopHVACs);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACDedicatedOutdoorAirSystem");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACDEDICATEDOUTDOORAIRSYSTEM_IMPL_HPP
