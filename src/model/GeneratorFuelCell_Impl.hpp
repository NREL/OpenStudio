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

#ifndef MODEL_GENERATORFUELCELL_IMPL_HPP
#define MODEL_GENERATORFUELCELL_IMPL_HPP

#include "ModelAPI.hpp"
#include "Generator_Impl.hpp"

namespace openstudio {
namespace model {

  class GeneratorFuelCellPowerModule;
  class GeneratorFuelCellAirSupply;
  class GeneratorFuelSupply;
  class GeneratorFuelCellWaterSupply;
  class GeneratorFuelCellAuxiliaryHeater;
  class GeneratorFuelCellExhaustGasToWaterHeatExchanger;
  class GeneratorFuelCellElectricalStorage;
  class GeneratorFuelCellInverter;
  class GeneratorFuelCellStackCooler;

  namespace detail {

    /** GeneratorFuelCell_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCell.*/
    class MODEL_API GeneratorFuelCell_Impl : public Generator_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCell_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCell_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCell_Impl(const GeneratorFuelCell_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCell_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::string generatorObjectType() const override;

      virtual boost::optional<double> ratedElectricPowerOutput() const override;

      virtual boost::optional<Schedule> availabilitySchedule() const override;

      virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const override;

      //@}
      /** @name Getters */
      //@{

      GeneratorFuelCellPowerModule powerModule() const;

      GeneratorFuelCellAirSupply airSupply() const;

      GeneratorFuelSupply fuelSupply() const;

      GeneratorFuelCellWaterSupply waterSupply() const;

      GeneratorFuelCellAuxiliaryHeater auxiliaryHeater() const;

      GeneratorFuelCellExhaustGasToWaterHeatExchanger heatExchanger() const;

      GeneratorFuelCellElectricalStorage electricalStorage() const;

      GeneratorFuelCellInverter inverter() const;

      boost::optional<GeneratorFuelCellStackCooler> stackCooler() const;

      //@}
      /** @name Setters */
      //@{

      bool setPowerModule(const GeneratorFuelCellPowerModule& fCPM);

      bool setAirSupply(const GeneratorFuelCellAirSupply& fCAirSup);

      bool setFuelSupply(const GeneratorFuelSupply& genFuelSup);

      bool setWaterSupply(const GeneratorFuelCellWaterSupply& fCWaterSup);

      bool setAuxiliaryHeater(const GeneratorFuelCellAuxiliaryHeater& fCAuxHeat);

      bool setHeatExchanger(const GeneratorFuelCellExhaustGasToWaterHeatExchanger& fCExhaustHX);

      bool setElectricalStorage(const GeneratorFuelCellElectricalStorage& fCStorage);

      bool setInverter(const GeneratorFuelCellInverter& fCInverter);

      bool setStackCooler(const GeneratorFuelCellStackCooler& fCStackCooler);

      void resetStackCooler();

      //@}
      /** @name Other */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelCell");

      boost::optional<GeneratorFuelCellPowerModule> optionalPowerModule() const;
      boost::optional<GeneratorFuelCellAirSupply> optionalAirSupply() const;
      boost::optional<GeneratorFuelSupply> optionalFuelSupply() const;
      boost::optional<GeneratorFuelCellWaterSupply> optionalWaterSupply() const;
      boost::optional<GeneratorFuelCellAuxiliaryHeater> optionalAuxiliaryHeater() const;
      boost::optional<GeneratorFuelCellExhaustGasToWaterHeatExchanger> optionalHeatExchanger() const;
      boost::optional<GeneratorFuelCellElectricalStorage> optionalElectricalStorage() const;
      boost::optional<GeneratorFuelCellInverter> optionalInverter() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELL_IMPL_HPP
