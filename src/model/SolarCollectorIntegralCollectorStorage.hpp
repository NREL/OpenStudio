/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_HPP
#define MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class PlanarSurface;
  class SolarCollectorPerformanceIntegralCollectorStorage;

  namespace detail {

    class SolarCollectorIntegralCollectorStorage_Impl;

  }  // namespace detail

  /** SolarCollectorIntegralCollectorStorage is a StraightComponent that wraps the OpenStudio IDD object 'OS:SolarCollector:IntegralCollectorStorage'. */
  class MODEL_API SolarCollectorIntegralCollectorStorage : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SolarCollectorIntegralCollectorStorage(const Model& model);

    virtual ~SolarCollectorIntegralCollectorStorage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorIntegralCollectorStorage(const SolarCollectorIntegralCollectorStorage& other) = default;
    SolarCollectorIntegralCollectorStorage(SolarCollectorIntegralCollectorStorage&& other) = default;
    SolarCollectorIntegralCollectorStorage& operator=(const SolarCollectorIntegralCollectorStorage&) = default;
    SolarCollectorIntegralCollectorStorage& operator=(SolarCollectorIntegralCollectorStorage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> bottomSurfaceBoundaryConditionsTypeValues();

    /** @name Getters */
    //@{

    SolarCollectorPerformanceIntegralCollectorStorage solarCollectorPerformance() const;

    boost::optional<PlanarSurface> surface() const;

    std::string bottomSurfaceBoundaryConditionsType() const;

    bool isBottomSurfaceBoundaryConditionsTypeDefaulted() const;

    boost::optional<double> maximumFlowRate() const;

    //@}
    /** @name Setters */
    //@{

    /// Deletes the current parameters and clones the parameters passed in
    bool setSolarCollectorPerformance(const SolarCollectorPerformanceIntegralCollectorStorage& solarCollectorPerformance);

    /// Deletes the current parameters and constructs a new default set of parameters
    void resetSolarCollectorPerformance();

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SolarCollectorIntegralCollectorStorage_Impl;

    explicit SolarCollectorIntegralCollectorStorage(std::shared_ptr<detail::SolarCollectorIntegralCollectorStorage_Impl> impl);

    friend class detail::SolarCollectorIntegralCollectorStorage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorIntegralCollectorStorage");
  };

  /** \relates SolarCollectorIntegralCollectorStorage*/
  using OptionalSolarCollectorIntegralCollectorStorage = boost::optional<SolarCollectorIntegralCollectorStorage>;

  /** \relates SolarCollectorIntegralCollectorStorage*/
  using SolarCollectorIntegralCollectorStorageVector = std::vector<SolarCollectorIntegralCollectorStorage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_HPP
