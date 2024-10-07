/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLELECTRICALSTORAGE_HPP
#define MODEL_GENERATORFUELCELLELECTRICALSTORAGE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class GeneratorFuelCell;

  namespace detail {

    class GeneratorFuelCellElectricalStorage_Impl;

  }  // namespace detail

  /** GeneratorFuelCellElectricalStorage is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:ElectricalStorage'. */
  class MODEL_API GeneratorFuelCellElectricalStorage : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCellElectricalStorage(const Model& model);

    virtual ~GeneratorFuelCellElectricalStorage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCellElectricalStorage(const GeneratorFuelCellElectricalStorage& other) = default;
    GeneratorFuelCellElectricalStorage(GeneratorFuelCellElectricalStorage&& other) = default;
    GeneratorFuelCellElectricalStorage& operator=(const GeneratorFuelCellElectricalStorage&) = default;
    GeneratorFuelCellElectricalStorage& operator=(GeneratorFuelCellElectricalStorage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> choiceofModelValues();

    /** @name Getters */
    //@{

    std::string choiceofModel() const;

    double nominalChargingEnergeticEfficiency() const;

    double nominalDischargingEnergeticEfficiency() const;

    double simpleMaximumCapacity() const;

    double simpleMaximumPowerDraw() const;

    double simpleMaximumPowerStore() const;

    double initialChargeState() const;

    // Return optional parent generator
    boost::optional<GeneratorFuelCell> fuelCell() const;

    //@}
    /** @name Setters */
    //@{

    bool setChoiceofModel(const std::string& choiceofModel);

    void resetChoiceofModel();

    bool setNominalChargingEnergeticEfficiency(double nominalChargingEnergeticEfficiency);

    void resetNominalChargingEnergeticEfficiency();

    bool setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency);

    void resetNominalDischargingEnergeticEfficiency();

    bool setSimpleMaximumCapacity(double simpleMaximumCapacity);

    void resetSimpleMaximumCapacity();

    bool setSimpleMaximumPowerDraw(double simpleMaximumPowerDraw);

    void resetSimpleMaximumPowerDraw();

    bool setSimpleMaximumPowerStore(double simpleMaximumPowerStore);

    void resetSimpleMaximumPowerStore();

    bool setInitialChargeState(double initialChargeState);

    void resetInitialChargeState();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCellElectricalStorage_Impl;

    explicit GeneratorFuelCellElectricalStorage(std::shared_ptr<detail::GeneratorFuelCellElectricalStorage_Impl> impl);

    friend class detail::GeneratorFuelCellElectricalStorage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellElectricalStorage");
  };

  /** \relates GeneratorFuelCellElectricalStorage*/
  using OptionalGeneratorFuelCellElectricalStorage = boost::optional<GeneratorFuelCellElectricalStorage>;

  /** \relates GeneratorFuelCellElectricalStorage*/
  using GeneratorFuelCellElectricalStorageVector = std::vector<GeneratorFuelCellElectricalStorage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLELECTRICALSTORAGE_HPP
