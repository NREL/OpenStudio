/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_HPP
#define MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl;

  }  // namespace detail

  /** SiteGroundTemperatureUndisturbedKusudaAchenbach is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_UndisturbedKusudaAchenbach'. */
  class MODEL_API SiteGroundTemperatureUndisturbedKusudaAchenbach : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureUndisturbedKusudaAchenbach(const Model& model);

    virtual ~SiteGroundTemperatureUndisturbedKusudaAchenbach() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundTemperatureUndisturbedKusudaAchenbach(const SiteGroundTemperatureUndisturbedKusudaAchenbach& other) = default;
    SiteGroundTemperatureUndisturbedKusudaAchenbach(SiteGroundTemperatureUndisturbedKusudaAchenbach&& other) = default;
    SiteGroundTemperatureUndisturbedKusudaAchenbach& operator=(const SiteGroundTemperatureUndisturbedKusudaAchenbach&) = default;
    SiteGroundTemperatureUndisturbedKusudaAchenbach& operator=(SiteGroundTemperatureUndisturbedKusudaAchenbach&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    double soilThermalConductivity() const;

    double soilDensity() const;

    double soilSpecificHeat() const;

    boost::optional<double> averageSoilSurfaceTemperature() const;

    boost::optional<double> averageAmplitudeofSurfaceTemperature() const;

    boost::optional<double> phaseShiftofMinimumSurfaceTemperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setSoilThermalConductivity(double soilThermalConductivity);

    bool setSoilDensity(double soilDensity);

    bool setSoilSpecificHeat(double soilSpecificHeat);

    bool setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature);

    void resetAverageSoilSurfaceTemperature();

    bool setAverageAmplitudeofSurfaceTemperature(double averageAmplitudeofSurfaceTemperature);

    void resetAverageAmplitudeofSurfaceTemperature();

    bool setPhaseShiftofMinimumSurfaceTemperature(double phaseShiftofMinimumSurfaceTemperature);

    void resetPhaseShiftofMinimumSurfaceTemperature();

    //@}
   protected:
    /// @cond
    using ImplType = detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl;

    explicit SiteGroundTemperatureUndisturbedKusudaAchenbach(std::shared_ptr<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureUndisturbedKusudaAchenbach");
  };

  /** \relates SiteGroundTemperatureUndisturbedKusudaAchenbach*/
  using OptionalSiteGroundTemperatureUndisturbedKusudaAchenbach = boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach>;

  /** \relates SiteGroundTemperatureUndisturbedKusudaAchenbach*/
  using SiteGroundTemperatureUndisturbedKusudaAchenbachVector = std::vector<SiteGroundTemperatureUndisturbedKusudaAchenbach>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_HPP
