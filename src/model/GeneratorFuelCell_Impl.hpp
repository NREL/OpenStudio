/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~GeneratorFuelCell_Impl() override = default;

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
