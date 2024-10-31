/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELL_HPP
#define MODEL_GENERATORFUELCELL_HPP

#include "ModelAPI.hpp"
#include "Generator.hpp"

#include "GeneratorFuelCellPowerModule.hpp"
#include "GeneratorFuelCellPowerModule_Impl.hpp"
#include "GeneratorFuelCellAirSupply.hpp"
#include "GeneratorFuelCellAirSupply_Impl.hpp"
#include "GeneratorFuelCellWaterSupply.hpp"
#include "GeneratorFuelCellWaterSupply_Impl.hpp"
#include "GeneratorFuelCellAuxiliaryHeater.hpp"
#include "GeneratorFuelCellAuxiliaryHeater_Impl.hpp"
#include "GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl.hpp"
#include "GeneratorFuelCellElectricalStorage.hpp"
#include "GeneratorFuelCellElectricalStorage_Impl.hpp"
#include "GeneratorFuelCellInverter.hpp"
#include "GeneratorFuelCellInverter_Impl.hpp"
#include "GeneratorFuelCellStackCooler.hpp"
#include "GeneratorFuelCellStackCooler_Impl.hpp"
#include "GeneratorFuelSupply.hpp"
#include "GeneratorFuelSupply_Impl.hpp"

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

    class GeneratorFuelCell_Impl;

  }  // namespace detail

  /** GeneratorFuelCell is a Generator that wraps the OpenStudio IDD object 'OS:Generator:FuelCell'. */
  class MODEL_API GeneratorFuelCell : public Generator
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCell(const Model& model, const GeneratorFuelCellPowerModule& fCPM, const GeneratorFuelCellAirSupply& fCAS,
                               const GeneratorFuelCellWaterSupply& fCWS, const GeneratorFuelCellAuxiliaryHeater& fCAH,
                               const GeneratorFuelCellExhaustGasToWaterHeatExchanger& fCExhaustHX, const GeneratorFuelCellElectricalStorage& fCES,
                               const GeneratorFuelCellInverter& fCInverter, const GeneratorFuelSupply& fS);

    explicit GeneratorFuelCell(const Model& model);

    virtual ~GeneratorFuelCell() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCell(const GeneratorFuelCell& other) = default;
    GeneratorFuelCell(GeneratorFuelCell&& other) = default;
    GeneratorFuelCell& operator=(const GeneratorFuelCell&) = default;
    GeneratorFuelCell& operator=(GeneratorFuelCell&&) = default;

    //@}

    static IddObjectType iddObjectType();

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

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCell_Impl;

    explicit GeneratorFuelCell(std::shared_ptr<detail::GeneratorFuelCell_Impl> impl);

    friend class detail::GeneratorFuelCell_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCell");
  };

  /** \relates GeneratorFuelCell*/
  using OptionalGeneratorFuelCell = boost::optional<GeneratorFuelCell>;

  /** \relates GeneratorFuelCell*/
  using GeneratorFuelCellVector = std::vector<GeneratorFuelCell>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELL_HPP
