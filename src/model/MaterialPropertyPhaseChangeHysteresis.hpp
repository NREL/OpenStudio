/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIALPROPERTYPHASECHANGEHYSTERESIS_HPP
#define MODEL_MATERIALPROPERTYPHASECHANGEHYSTERESIS_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Material;
  class StandardOpaqueMaterial;
  class MasslessOpaqueMaterial;

  namespace detail {

    class MaterialPropertyPhaseChangeHysteresis_Impl;

  }  // namespace detail

  /** MaterialPropertyPhaseChangeHysteresis is a ModelObject that wraps the OpenStudio IDD object 'OS:MaterialProperty:PhaseChangeHysteresis'. */
  class MODEL_API MaterialPropertyPhaseChangeHysteresis : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit MaterialPropertyPhaseChangeHysteresis(StandardOpaqueMaterial& material);

    explicit MaterialPropertyPhaseChangeHysteresis(MasslessOpaqueMaterial& material);

    virtual ~MaterialPropertyPhaseChangeHysteresis() override = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string materialName() const;

    Material material() const;

    double latentHeatduringtheEntirePhaseChangeProcess() const;

    double liquidStateThermalConductivity() const;

    double liquidStateDensity() const;

    double liquidStateSpecificHeat() const;

    double highTemperatureDifferenceofMeltingCurve() const;

    double peakMeltingTemperature() const;

    double lowTemperatureDifferenceofMeltingCurve() const;

    double solidStateThermalConductivity() const;

    double solidStateDensity() const;

    double solidStateSpecificHeat() const;

    double highTemperatureDifferenceofFreezingCurve() const;

    double peakFreezingTemperature() const;

    double lowTemperatureDifferenceofFreezingCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setLatentHeatduringtheEntirePhaseChangeProcess(double latentHeatduringtheEntirePhaseChangeProcess);

    bool setLiquidStateThermalConductivity(double liquidStateThermalConductivity);

    bool setLiquidStateDensity(double liquidStateDensity);

    bool setLiquidStateSpecificHeat(double liquidStateSpecificHeat);

    bool setHighTemperatureDifferenceofMeltingCurve(double highTemperatureDifferenceofMeltingCurve);

    bool setPeakMeltingTemperature(double peakMeltingTemperature);

    bool setLowTemperatureDifferenceofMeltingCurve(double lowTemperatureDifferenceofMeltingCurve);

    bool setSolidStateThermalConductivity(double solidStateThermalConductivity);

    bool setSolidStateDensity(double solidStateDensity);

    bool setSolidStateSpecificHeat(double solidStateSpecificHeat);

    bool setHighTemperatureDifferenceofFreezingCurve(double highTemperatureDifferenceofFreezingCurve);

    bool setPeakFreezingTemperature(double peakFreezingTemperature);

    bool setLowTemperatureDifferenceofFreezingCurve(double lowTemperatureDifferenceofFreezingCurve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::MaterialPropertyPhaseChangeHysteresis_Impl;

    explicit MaterialPropertyPhaseChangeHysteresis(std::shared_ptr<detail::MaterialPropertyPhaseChangeHysteresis_Impl> impl);

    friend class detail::MaterialPropertyPhaseChangeHysteresis_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.MaterialPropertyPhaseChangeHysteresis");
  };

  /** \relates MaterialPropertyPhaseChangeHysteresis*/
  using OptionalMaterialPropertyPhaseChangeHysteresis = boost::optional<MaterialPropertyPhaseChangeHysteresis>;

  /** \relates MaterialPropertyPhaseChangeHysteresis*/
  using MaterialPropertyPhaseChangeHysteresisVector = std::vector<MaterialPropertyPhaseChangeHysteresis>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIALPROPERTYPHASECHANGEHYSTERESIS_HPP
