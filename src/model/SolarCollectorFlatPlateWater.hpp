/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORFLATPLATEWATER_HPP
#define MODEL_SOLARCOLLECTORFLATPLATEWATER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class SolarCollectorPerformanceFlatPlate;
  class PlanarSurface;

  namespace detail {

    class SolarCollectorFlatPlateWater_Impl;

  }  // namespace detail

  /** SolarCollectorFlatPlateWater is a StraightComponent that wraps the OpenStudio IDD object 'OS:SolarCollector:FlatPlate:Water'. */
  class MODEL_API SolarCollectorFlatPlateWater : public StraightComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SolarCollectorFlatPlateWater(const Model& model);

    virtual ~SolarCollectorFlatPlateWater() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorFlatPlateWater(const SolarCollectorFlatPlateWater& other) = default;
    SolarCollectorFlatPlateWater(SolarCollectorFlatPlateWater&& other) = default;
    SolarCollectorFlatPlateWater& operator=(const SolarCollectorFlatPlateWater&) = default;
    SolarCollectorFlatPlateWater& operator=(SolarCollectorFlatPlateWater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    SolarCollectorPerformanceFlatPlate solarCollectorPerformance() const;

    boost::optional<PlanarSurface> surface() const;

    boost::optional<double> maximumFlowRate() const;

    //@}
    /** @name Setters */
    //@{

    /// Deletes the current parameters and clones the parameters passed in
    bool setSolarCollectorPerformance(const SolarCollectorPerformanceFlatPlate& parameters);

    /// Deletes the current parameters and constructs a new default set of parameters
    void resetSolarCollectorPerformance();

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    //bool setInletNode(const Connection& connection);

    //bool setOutletNode(const Connection& connection);

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SolarCollectorFlatPlateWater_Impl;

    explicit SolarCollectorFlatPlateWater(std::shared_ptr<detail::SolarCollectorFlatPlateWater_Impl> impl);

    friend class detail::SolarCollectorFlatPlateWater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorFlatPlateWater");
  };

  /** \relates SolarCollectorFlatPlateWater*/
  using OptionalSolarCollectorFlatPlateWater = boost::optional<SolarCollectorFlatPlateWater>;

  /** \relates SolarCollectorFlatPlateWater*/
  using SolarCollectorFlatPlateWaterVector = std::vector<SolarCollectorFlatPlateWater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORFLATPLATEWATER_HPP
