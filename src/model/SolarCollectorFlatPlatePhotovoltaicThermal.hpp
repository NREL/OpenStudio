/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_HPP
#define MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class PlanarSurface;
  class GeneratorPhotovoltaic;
  class SolarCollectorPerformancePhotovoltaicThermalBIPVT;
  class SolarCollectorPerformancePhotovoltaicThermalSimple;

  namespace detail {

    class SolarCollectorFlatPlatePhotovoltaicThermal_Impl;

  }  // namespace detail

  /** SolarCollectorFlatPlatePhotovoltaicThermal is a StraightComponent that wraps the OpenStudio IDD object 'OS:SolarCollector:FlatPlate:PhotovoltaicThermal'. */
  class MODEL_API SolarCollectorFlatPlatePhotovoltaicThermal : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SolarCollectorFlatPlatePhotovoltaicThermal(const Model& model);
    explicit SolarCollectorFlatPlatePhotovoltaicThermal(const SolarCollectorPerformancePhotovoltaicThermalBIPVT& performance);
    explicit SolarCollectorFlatPlatePhotovoltaicThermal(const SolarCollectorPerformancePhotovoltaicThermalSimple& performance);

    virtual ~SolarCollectorFlatPlatePhotovoltaicThermal() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorFlatPlatePhotovoltaicThermal(const SolarCollectorFlatPlatePhotovoltaicThermal& other) = default;
    SolarCollectorFlatPlatePhotovoltaicThermal(SolarCollectorFlatPlatePhotovoltaicThermal&& other) = default;
    SolarCollectorFlatPlatePhotovoltaicThermal& operator=(const SolarCollectorFlatPlatePhotovoltaicThermal&) = default;
    SolarCollectorFlatPlatePhotovoltaicThermal& operator=(SolarCollectorFlatPlatePhotovoltaicThermal&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ModelObject solarCollectorPerformance() const;

    boost::optional<PlanarSurface> surface() const;

    boost::optional<GeneratorPhotovoltaic> generatorPhotovoltaic() const;

    boost::optional<double> designFlowRate() const;

    bool isDesignFlowRateAutosized() const;

    //@}
    /** @name Setters */
    //@{

    /// Deletes the current performance and clones the performance passed in
    bool setSolarCollectorPerformance(const SolarCollectorPerformancePhotovoltaicThermalBIPVT& performance);
    bool setSolarCollectorPerformance(const SolarCollectorPerformancePhotovoltaicThermalSimple& performance);

    /// Deletes the current performance and constructs a new default set of performance
    void resetSolarCollectorPerformance();

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    bool setGeneratorPhotovoltaic(const GeneratorPhotovoltaic& generatorPhotovoltaic);

    void resetGeneratorPhotovoltaic();

    bool setDesignFlowRate(double designFlowRate);

    void resetDesignFlowRate();

    void autosizeDesignFlowRate();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedDesignFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl;

    explicit SolarCollectorFlatPlatePhotovoltaicThermal(std::shared_ptr<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl> impl);

    friend class detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorFlatPlatePhotovoltaicThermal");
  };

  /** \relates SolarCollectorFlatPlatePhotovoltaicThermal*/
  using OptionalSolarCollectorFlatPlatePhotovoltaicThermal = boost::optional<SolarCollectorFlatPlatePhotovoltaicThermal>;

  /** \relates SolarCollectorFlatPlatePhotovoltaicThermal*/
  using SolarCollectorFlatPlatePhotovoltaicThermalVector = std::vector<SolarCollectorFlatPlatePhotovoltaicThermal>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_HPP
