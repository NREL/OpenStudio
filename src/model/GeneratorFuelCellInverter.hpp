/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLINVERTER_HPP
#define MODEL_GENERATORFUELCELLINVERTER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class CurveQuadratic;
  class GeneratorFuelCell;

  namespace detail {

    class GeneratorFuelCellInverter_Impl;

  }  // namespace detail

  /** GeneratorFuelCellInverter is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:Inverter'. */
  class MODEL_API GeneratorFuelCellInverter : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCellInverter(const Model& model);

    explicit GeneratorFuelCellInverter(const Model& model, const CurveQuadratic& powerCurve);

    virtual ~GeneratorFuelCellInverter() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCellInverter(const GeneratorFuelCellInverter& other) = default;
    GeneratorFuelCellInverter(GeneratorFuelCellInverter&& other) = default;
    GeneratorFuelCellInverter& operator=(const GeneratorFuelCellInverter&) = default;
    GeneratorFuelCellInverter& operator=(GeneratorFuelCellInverter&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> inverterEfficiencyCalculationModeValues();

    /** @name Getters */
    //@{

    std::string inverterEfficiencyCalculationMode() const;

    double inverterEfficiency() const;

    boost::optional<CurveQuadratic> efficiencyFunctionofDCPowerCurve() const;

    // Return optional parent generator
    boost::optional<GeneratorFuelCell> fuelCell() const;

    //@}
    /** @name Setters */
    //@{

    bool setInverterEfficiencyCalculationMode(const std::string& inverterEfficiencyCalculationMode);

    void resetInverterEfficiencyCalculationMode();

    bool setInverterEfficiency(double inverterEfficiency);

    void resetInverterEfficiency();

    bool setEfficiencyFunctionofDCPowerCurve(const CurveQuadratic& quadraticCurves);

    void resetEfficiencyFunctionofDCPowerCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCellInverter_Impl;

    explicit GeneratorFuelCellInverter(std::shared_ptr<detail::GeneratorFuelCellInverter_Impl> impl);

    friend class detail::GeneratorFuelCellInverter_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellInverter");
  };

  /** \relates GeneratorFuelCellInverter*/
  using OptionalGeneratorFuelCellInverter = boost::optional<GeneratorFuelCellInverter>;

  /** \relates GeneratorFuelCellInverter*/
  using GeneratorFuelCellInverterVector = std::vector<GeneratorFuelCellInverter>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLINVERTER_HPP
