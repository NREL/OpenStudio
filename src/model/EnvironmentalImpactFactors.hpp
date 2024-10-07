/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENVIRONMENTALIMPACTFACTORS_HPP
#define MODEL_ENVIRONMENTALIMPACTFACTORS_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class EnvironmentalImpactFactors_Impl;

  }  // namespace detail

  /** EnvironmentalImpactFactors is a ModelObject that wraps the OpenStudio IDD object 'OS:EnvironmentalImpactFactors'. */
  class MODEL_API EnvironmentalImpactFactors : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~EnvironmentalImpactFactors() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnvironmentalImpactFactors(const EnvironmentalImpactFactors& other) = default;
    EnvironmentalImpactFactors(EnvironmentalImpactFactors&& other) = default;
    EnvironmentalImpactFactors& operator=(const EnvironmentalImpactFactors&) = default;
    EnvironmentalImpactFactors& operator=(EnvironmentalImpactFactors&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double districtHeatingWaterEfficiency() const;
    OS_DEPRECATED(3, 7, 0) double districtHeatingEfficiency() const;

    double districtCoolingCOP() const;

    double districtHeatingSteamConversionEfficiency() const;
    OS_DEPRECATED(3, 7, 0) double steamConversionEfficiency() const;

    double totalCarbonEquivalentEmissionFactorFromN2O() const;

    double totalCarbonEquivalentEmissionFactorFromCH4() const;

    double totalCarbonEquivalentEmissionFactorFromCO2() const;

    //@}
    /** @name Setters */
    //@{

    bool setDistrictHeatingWaterEfficiency(double districtHeatingWaterEfficiency);
    OS_DEPRECATED(3, 7, 0) bool setDistrictHeatingEfficiency(double districtHeatingEfficiency);

    bool setDistrictCoolingCOP(double districtCoolingCOP);

    bool setDistrictHeatingSteamConversionEfficiency(double districtHeatingSteamConversionEfficiency);
    OS_DEPRECATED(3, 7, 0) bool setSteamConversionEfficiency(double steamConversionEfficiency);

    bool setTotalCarbonEquivalentEmissionFactorFromN2O(double totalCarbonEquivalentEmissionFactorFromN2O);

    bool setTotalCarbonEquivalentEmissionFactorFromCH4(double totalCarbonEquivalentEmissionFactorFromCH4);

    bool setTotalCarbonEquivalentEmissionFactorFromCO2(double totalCarbonEquivalentEmissionFactorFromCO2);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnvironmentalImpactFactors_Impl;

    explicit EnvironmentalImpactFactors(std::shared_ptr<detail::EnvironmentalImpactFactors_Impl> impl);

    friend class detail::EnvironmentalImpactFactors_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit EnvironmentalImpactFactors(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnvironmentalImpactFactors");
  };

  /** \relates EnvironmentalImpactFactors*/
  using OptionalEnvironmentalImpactFactors = boost::optional<EnvironmentalImpactFactors>;

  /** \relates EnvironmentalImpactFactors*/
  using EnvironmentalImpactFactorsVector = std::vector<EnvironmentalImpactFactors>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENVIRONMENTALIMPACTFACTORS_HPP
