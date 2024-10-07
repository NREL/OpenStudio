/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDXING_HPP
#define MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDXING_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundTemperatureUndisturbedXing_Impl;

  }  // namespace detail

  /** SiteGroundTemperatureUndisturbedXing is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_UndisturbedXing'. */
  class MODEL_API SiteGroundTemperatureUndisturbedXing : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureUndisturbedXing(const Model& model);

    virtual ~SiteGroundTemperatureUndisturbedXing() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundTemperatureUndisturbedXing(const SiteGroundTemperatureUndisturbedXing& other) = default;
    SiteGroundTemperatureUndisturbedXing(SiteGroundTemperatureUndisturbedXing&& other) = default;
    SiteGroundTemperatureUndisturbedXing& operator=(const SiteGroundTemperatureUndisturbedXing&) = default;
    SiteGroundTemperatureUndisturbedXing& operator=(SiteGroundTemperatureUndisturbedXing&&) = default;

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

    double averageSoilSurfaceTemperature() const;

    double soilSurfaceTemperatureAmplitude1() const;

    double soilSurfaceTemperatureAmplitude2() const;

    double phaseShiftofTemperatureAmplitude1() const;

    double phaseShiftofTemperatureAmplitude2() const;

    //@}
    /** @name Setters */
    //@{

    bool setSoilThermalConductivity(double soilThermalConductivity);

    bool setSoilDensity(double soilDensity);

    bool setSoilSpecificHeat(double soilSpecificHeat);

    bool setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature);

    bool setSoilSurfaceTemperatureAmplitude1(double soilSurfaceTemperatureAmplitude1);

    bool setSoilSurfaceTemperatureAmplitude2(double soilSurfaceTemperatureAmplitude2);

    bool setPhaseShiftofTemperatureAmplitude1(double phaseShiftofTemperatureAmplitude1);

    bool setPhaseShiftofTemperatureAmplitude2(double phaseShiftofTemperatureAmplitude2);

    //@}
   protected:
    /// @cond
    using ImplType = detail::SiteGroundTemperatureUndisturbedXing_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureUndisturbedXing_Impl;

    explicit SiteGroundTemperatureUndisturbedXing(std::shared_ptr<detail::SiteGroundTemperatureUndisturbedXing_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureUndisturbedXing");
  };

  /** \relates SiteGroundTemperatureUndisturbedXing*/
  using OptionalSiteGroundTemperatureUndisturbedXing = boost::optional<SiteGroundTemperatureUndisturbedXing>;

  /** \relates SiteGroundTemperatureUndisturbedXing*/
  using SiteGroundTemperatureUndisturbedXingVector = std::vector<SiteGroundTemperatureUndisturbedXing>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDXING_HPP
