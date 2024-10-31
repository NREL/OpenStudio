/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORPHOTOVOLTAIC_HPP
#define MODEL_GENERATORPHOTOVOLTAIC_HPP

#include "ModelAPI.hpp"
#include "Generator.hpp"

namespace openstudio {

namespace model {

  class PlanarSurface;
  class PhotovoltaicPerformance;
  class Schedule;

  namespace detail {

    class GeneratorPhotovoltaic_Impl;

  }  // namespace detail

  /** GeneratorPhotovoltaic is a Generator that wraps the OpenStudio IDD object 'OS:Generator:Photovoltaic'. */
  class MODEL_API GeneratorPhotovoltaic : public Generator
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /// Creates a GeneratorPhotovoltaic object with PhotovoltaicPerformanceSimple performance
    static GeneratorPhotovoltaic simple(const Model& model);

    /// Creates a GeneratorPhotovoltaic object with PhotovoltaicPerformanceEquivalentOneDiode performance
    static GeneratorPhotovoltaic equivalentOneDiode(const Model& model);

    /// Creates a GeneratorPhotovoltaic object with PhotovoltaicPerformanceSandia performance, using the default Ctor
    static GeneratorPhotovoltaic sandia(const Model& model);

    /// Factory method to creates a GeneratorPhotovoltaic object with PhotovoltaicPerformanceSandia by looking up characteristics in the embedded
    // Sandia database by its name. Please use the PhotovoltaicPerformanceSandia::sandiaModulePerformanceNames() static method
    // to look up the valid names as it will throw if it cannot find it
    static GeneratorPhotovoltaic fromSandiaDatabase(const Model& model, const std::string& sandiaModulePerformanceName);

    virtual ~GeneratorPhotovoltaic() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorPhotovoltaic(const GeneratorPhotovoltaic& other) = default;
    GeneratorPhotovoltaic(GeneratorPhotovoltaic&& other) = default;
    GeneratorPhotovoltaic& operator=(const GeneratorPhotovoltaic&) = default;
    GeneratorPhotovoltaic& operator=(GeneratorPhotovoltaic&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatTransferIntegrationModeValues();

    /** @name Getters */
    //@{

    boost::optional<PlanarSurface> surface() const;

    PhotovoltaicPerformance photovoltaicPerformance() const;

    std::string heatTransferIntegrationMode() const;

    bool isHeatTransferIntegrationModeDefaulted() const;

    double numberOfModulesInParallel() const;

    bool isNumberOfModulesInParallelDefaulted() const;

    double numberOfModulesInSeries() const;

    bool isNumberOfModulesInSeriesDefaulted() const;

    boost::optional<double> ratedElectricPowerOutput() const;

    boost::optional<Schedule> availabilitySchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    bool setHeatTransferIntegrationMode(const std::string& heatTransferIntegrationMode);

    void resetHeatTransferIntegrationMode();

    bool setNumberOfModulesInParallel(double numberOfModulesInParallel);

    void resetNumberOfModulesInParallel();

    bool setNumberOfModulesInSeries(double numberOfModulesInSeries);

    void resetNumberOfModulesInSeries();

    bool setRatedElectricPowerOutput(double ratedElectricPowerOutput);

    void resetRatedElectricPowerOutput();

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorPhotovoltaic_Impl;

    GeneratorPhotovoltaic(const Model& model, const PhotovoltaicPerformance& performance);

    explicit GeneratorPhotovoltaic(std::shared_ptr<detail::GeneratorPhotovoltaic_Impl> impl);

    friend class detail::GeneratorPhotovoltaic_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorPhotovoltaic");
  };

  /** \relates GeneratorPhotovoltaic*/
  using OptionalGeneratorPhotovoltaic = boost::optional<GeneratorPhotovoltaic>;

  /** \relates GeneratorPhotovoltaic*/
  using GeneratorPhotovoltaicVector = std::vector<GeneratorPhotovoltaic>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORPHOTOVOLTAIC_HPP
