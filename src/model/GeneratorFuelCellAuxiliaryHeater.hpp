/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLAUXILIARYHEATER_HPP
#define MODEL_GENERATORFUELCELLAUXILIARYHEATER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class GeneratorFuelCell;

  namespace detail {

    class GeneratorFuelCellAuxiliaryHeater_Impl;

  }  // namespace detail

  /** GeneratorFuelCellAuxiliaryHeater is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:AuxiliaryHeater'. */
  class MODEL_API GeneratorFuelCellAuxiliaryHeater : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCellAuxiliaryHeater(const Model& model);

    virtual ~GeneratorFuelCellAuxiliaryHeater() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCellAuxiliaryHeater(const GeneratorFuelCellAuxiliaryHeater& other) = default;
    GeneratorFuelCellAuxiliaryHeater(GeneratorFuelCellAuxiliaryHeater&& other) = default;
    GeneratorFuelCellAuxiliaryHeater& operator=(const GeneratorFuelCellAuxiliaryHeater&) = default;
    GeneratorFuelCellAuxiliaryHeater& operator=(GeneratorFuelCellAuxiliaryHeater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> skinLossDestinationValues();

    static std::vector<std::string> heatingCapacityUnitsValues();

    /** @name Getters */
    //@{

    double excessAirRatio() const;

    double ancillaryPowerConstantTerm() const;

    double ancillaryPowerLinearTerm() const;

    double skinLossUFactorTimesAreaValue() const;

    std::string skinLossDestination() const;

    boost::optional<ThermalZone> zonetoReceiveSkinLosses() const;

    std::string heatingCapacityUnits() const;

    double maximumHeatingCapacityinWatts() const;

    double minimumHeatingCapacityinWatts() const;

    double maximumHeatingCapacityinKmolperSecond() const;

    double minimumHeatingCapacityinKmolperSecond() const;

    // Return optional parent generator
    boost::optional<GeneratorFuelCell> fuelCell() const;

    //@}
    /** @name Setters */
    //@{

    bool setExcessAirRatio(double excessAirRatio);

    void resetExcessAirRatio();

    bool setAncillaryPowerConstantTerm(double ancillaryPowerConstantTerm);

    void resetAncillaryPowerConstantTerm();

    bool setAncillaryPowerLinearTerm(double ancillaryPowerLinearTerm);

    void resetAncillaryPowerLinearTerm();

    bool setSkinLossUFactorTimesAreaValue(double skinLossUFactorTimesAreaValue);

    void resetSkinLossUFactorTimesAreaValue();

    bool setSkinLossDestination(const std::string& skinLossDestination);

    void resetSkinLossDestination();

    bool setZonetoReceiveSkinLosses(const ThermalZone& zone);

    void resetZonetoReceiveSkinLosses();

    bool setHeatingCapacityUnits(const std::string& heatingCapacityUnits);

    void resetHeatingCapacityUnits();

    bool setMaximumHeatingCapacityinWatts(double maximumHeatingCapacityinWatts);

    void resetMaximumHeatingCapacityinWatts();

    bool setMinimumHeatingCapacityinWatts(double minimumHeatingCapacityinWatts);

    void resetMinimumHeatingCapacityinWatts();

    bool setMaximumHeatingCapacityinKmolperSecond(double maximumHeatingCapacityinKmolperSecond);

    void resetMaximumHeatingCapacityinKmolperSecond();

    bool setMinimumHeatingCapacityinKmolperSecond(double minimumHeatingCapacityinKmolperSecond);

    void resetMinimumHeatingCapacityinKmolperSecond();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCellAuxiliaryHeater_Impl;

    explicit GeneratorFuelCellAuxiliaryHeater(std::shared_ptr<detail::GeneratorFuelCellAuxiliaryHeater_Impl> impl);

    friend class detail::GeneratorFuelCellAuxiliaryHeater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellAuxiliaryHeater");
  };

  /** \relates GeneratorFuelCellAuxiliaryHeater*/
  using OptionalGeneratorFuelCellAuxiliaryHeater = boost::optional<GeneratorFuelCellAuxiliaryHeater>;

  /** \relates GeneratorFuelCellAuxiliaryHeater*/
  using GeneratorFuelCellAuxiliaryHeaterVector = std::vector<GeneratorFuelCellAuxiliaryHeater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLAUXILIARYHEATER_HPP
