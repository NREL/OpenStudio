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

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_IMPL_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACLowTempRadiantVarFlow_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACLowTempRadiantVarFlow.*/
    class MODEL_API ZoneHVACLowTempRadiantVarFlow_Impl : public ZoneHVACComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACLowTempRadiantVarFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTempRadiantVarFlow_Impl(const ZoneHVACLowTempRadiantVarFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACLowTempRadiantVarFlow_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<HVACComponent> heatingCoil() const;

      boost::optional<HVACComponent> coolingCoil() const;

      boost::optional<std::string> radiantSurfaceType() const;

      std::vector<Surface> surfaces() const;

      std::string fluidtoRadiantSurfaceHeatTransferModel() const;

      bool isFluidtoRadiantSurfaceHeatTransferModelDefaulted() const;

      double hydronicTubingInsideDiameter() const;

      bool isHydronicTubingInsideDiameterDefaulted() const;

      double hydronicTubingOutsideDiameter() const;

      bool isHydronicTubingOutsideDiameterDefaulted() const;

      boost::optional<double> hydronicTubingLength() const;

      bool isHydronicTubingLengthDefaulted() const;

      bool isHydronicTubingLengthAutosized() const;

      double hydronicTubingConductivity() const;

      bool isHydronicTubingConductivityDefaulted() const;

      std::string temperatureControlType() const;

      bool isTemperatureControlTypeDefaulted() const;

      std::string setpointControlType() const;

      bool isSetpointControlTypeDefaulted() const;

      std::string numberofCircuits() const;

      double circuitLength() const;

      boost::optional<Schedule> changeoverDelayTimePeriodSchedule() const;

      boost::optional<double> autosizedHydronicTubingLength() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setHeatingCoil(HVACComponent& heatingCoil);
      void resetHeatingCoil();

      bool setCoolingCoil(HVACComponent& coolingCoil);
      void resetCoolingCoil();

      bool setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType);

      void resetRadiantSurfaceType();

      bool setFluidtoRadiantSurfaceHeatTransferModel(const std::string& fluidtoRadiantSurfaceHeatTransferModel);

      void resetFluidtoRadiantSurfaceHeatTransferModel();

      bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

      void resetHydronicTubingInsideDiameter();

      bool setHydronicTubingOutsideDiameter(double hydronicTubingOutsideDiameter);

      void resetHydronicTubingOutsideDiameter();

      bool setHydronicTubingLength(boost::optional<double> hydronicTubingLength);

      void resetHydronicTubingLength();

      void autosizeHydronicTubingLength();

      bool setHydronicTubingConductivity(double hydronicTubingConductivity);

      void resetHydronicTubingConductivity();

      bool setTemperatureControlType(const std::string& temperatureControlType);

      void resetTemperatureControlType();

      bool setSetpointControlType(const std::string& setpointControlType);

      void resetSetpointControlType();

      bool setNumberofCircuits(const std::string& numberofCircuits);

      bool setCircuitLength(double circuitLength);

      bool setChangeoverDelayTimePeriodSchedule(Schedule& schedule);

      void resetChangeoverDelayTimePeriodSchedule();

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACLowTempRadiantVarFlow");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;
      boost::optional<HVACComponent> optionalCoolingCoil() const;

      std::vector<std::string> radiantSurfaceTypeValues() const;
      std::vector<std::string> temperatureControlTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_IMPL_HPP
