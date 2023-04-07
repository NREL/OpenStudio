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

#ifndef MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_IMPL_HPP
#define MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

  class ElectricEquipmentITEAirCooledDefinition;
  class ElectricEquipmentITEAirCooled;
  class Curve;
  class Schedule;

  namespace detail {

    /** ElectricEquipmentITEAirCooled_Impl is a SpaceLoadInstance_Impl that is the implementation class for ElectricEquipmentITEAirCooled.*/
    class MODEL_API ElectricEquipmentITEAirCooled_Impl : public SpaceLoadInstance_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricEquipmentITEAirCooled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricEquipmentITEAirCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricEquipmentITEAirCooled_Impl(const ElectricEquipmentITEAirCooled_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ElectricEquipmentITEAirCooled_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool hardSize() override;

      virtual bool hardApplySchedules() override;

      virtual double multiplier() const override;

      virtual bool isMultiplierDefaulted() const override;

      virtual bool isAbsolute() const override;
      //@}
      /** @name Getters */
      //@{

      ElectricEquipmentITEAirCooledDefinition electricEquipmentITEAirCooledDefinition() const;

      // Return the Design Power Input Schedule. If not specified, default to always 1.
      boost::optional<Schedule> designPowerInputSchedule() const;

      bool isDesignPowerInputScheduleDefaulted() const;

      // Return the CPU Loading Schedule. If not specified, default to always 1.
      boost::optional<Schedule> cPULoadingSchedule() const;

      bool isCPULoadingScheduleDefaulted() const;

      std::string cPUEndUseSubcategory() const;

      bool isCPUEndUseSubcategoryDefaulted() const;

      std::string fanEndUseSubcategory() const;

      bool isFanEndUseSubcategoryDefaulted() const;

      std::string electricPowerSupplyEndUseSubcategory() const;

      bool isElectricPowerSupplyEndUseSubcategoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setElectricEquipmentITEAirCooledDefinition(const ElectricEquipmentITEAirCooledDefinition& electricEquipmentITEAirCooledDefinition);

      /** Sets the definition of this instance. */
      virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

      // Note Schedules are passed by reference, not const reference.
      bool setDesignPowerInputSchedule(Schedule& schedule);

      void resetDesignPowerInputSchedule();

      // Note Schedules are passed by reference, not const reference.
      bool setCPULoadingSchedule(Schedule& schedule);

      void resetCPULoadingSchedule();

      bool setMultiplier(double multiplier);

      void resetMultiplier();

      bool setCPUEndUseSubcategory(const std::string& cPUEndUseSubcategory);

      void resetCPUEndUseSubcategory();

      bool setFanEndUseSubcategory(const std::string& fanEndUseSubcategory);

      void resetFanEndUseSubcategory();

      bool setElectricPowerSupplyEndUseSubcategory(const std::string& electricPowerSupplyEndUseSubcategory);

      void resetElectricPowerSupplyEndUseSubcategory();

      //@}
      /** @name Other */
      //@{

      boost::optional<double> wattsperUnit() const;

      boost::optional<double> wattsperZoneFloorArea() const;

      double getWattsperUnit(double floorArea) const;

      double getWattsperZoneFloorArea(double floorArea) const;

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

      // index of the definition name
      virtual int definitionIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.ElectricEquipmentITEAirCooled");

      boost::optional<ModelObject> scheduleAsModelObject() const;
      boost::optional<ModelObject> electricEquipmentITEAirCooledDefinitionAsModelObject() const;

      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setElectricEquipmentITEAirCooledDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_IMPL_HPP
