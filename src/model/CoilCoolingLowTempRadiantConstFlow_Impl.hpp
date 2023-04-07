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

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** CoilCoolingLowTempRadiantConstFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingLowTempRadiantConstFlow.*/
    class MODEL_API CoilCoolingLowTempRadiantConstFlow_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingLowTempRadiantConstFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingLowTempRadiantConstFlow_Impl(const CoilCoolingLowTempRadiantConstFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingLowTempRadiantConstFlow_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> coolingHighWaterTemperatureSchedule() const;

      boost::optional<Schedule> coolingLowWaterTemperatureSchedule() const;

      boost::optional<Schedule> coolingHighControlTemperatureSchedule() const;

      boost::optional<Schedule> coolingLowControlTemperatureSchedule() const;

      std::string condensationControlType() const;

      bool isCondensationControlTypeDefaulted() const;

      double condensationControlDewpointOffset() const;

      bool isCondensationControlDewpointOffsetDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setCoolingHighWaterTemperatureSchedule(Schedule& schedule);

      void resetCoolingHighWaterTemperatureSchedule();

      bool setCoolingLowWaterTemperatureSchedule(Schedule& schedule);

      void resetCoolingLowWaterTemperatureSchedule();

      bool setCoolingHighControlTemperatureSchedule(Schedule& schedule);

      void resetCoolingHighControlTemperatureSchedule();

      bool setCoolingLowControlTemperatureSchedule(Schedule& schedule);

      void resetCoolingLowControlTemperatureSchedule();

      bool setCondensationControlType(const std::string& condensationControlType);

      void resetCondensationControlType();

      bool setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

      void resetCondensationControlDewpointOffset();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingLowTempRadiantConstFlow");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals

      std::vector<std::string> condensationControlTypeValues() const;

      boost::optional<ModelObject> coolingHighWaterTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> coolingLowWaterTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> coolingHighControlTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> coolingLowControlTemperatureScheduleAsModelObject() const;

      bool setCoolingHighWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingLowWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingHighControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingLowControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
