/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADOWCALCULATION_HPP
#define MODEL_SHADOWCALCULATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  // Forward declarations
  class ThermalZone;

  namespace detail {
    class ShadowCalculation_Impl;
  }  // namespace detail

  /** ShadowCalculation derives from ModelObject and is an interface to the OpenStudio IDD object named "ShadowCalculation".
 *
 *  ShadowCalculation defines how often shadowing calculations should be performed in EnergyPlus simulations
 *  as well as how many overlapping figures can be considered in a shadow calculation.
 *  ShadowCalculation does not have a public constructor because it is a unique ModelObject.
 *  To get the ShadowCalculation object for a Model or create one if it does not yet exist use model.getUniqueObject<ShadowCalculation>().
 *  To get the ShadowCalculation object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<ShadowCalculation>().
 */
  class MODEL_API ShadowCalculation : public ModelObject
  {
   public:
    virtual ~ShadowCalculation() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ShadowCalculation(const ShadowCalculation& other) = default;
    ShadowCalculation(ShadowCalculation&& other) = default;
    ShadowCalculation& operator=(const ShadowCalculation&) = default;
    ShadowCalculation& operator=(ShadowCalculation&&) = default;

    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> shadingCalculationMethodValues();
    static std::vector<std::string> validShadingCalculationMethodValues();

    static std::vector<std::string> shadingCalculationUpdateFrequencyMethodValues();
    static std::vector<std::string> validShadingCalculationUpdateFrequencyMethodValues();

    static std::vector<std::string> polygonClippingAlgorithmValues();
    static std::vector<std::string> validPolygonClippingAlgorithmValues();

    static std::vector<std::string> skyDiffuseModelingAlgorithmValues();
    static std::vector<std::string> validSkyDiffuseModelingAlgorithmValues();

    //@}
    /** @name Getters */
    //@{

    // new field
    std::string shadingCalculationMethod() const;
    // bool isShadingCalculationMethodDefaulted() const;

    // renamed method, used to be 'calculationMethod'
    std::string shadingCalculationUpdateFrequencyMethod() const;
    bool isShadingCalculationUpdateFrequencyMethodDefaulted() const;

    int shadingCalculationUpdateFrequency() const;
    bool isShadingCalculationUpdateFrequencyDefaulted() const;

    int maximumFiguresInShadowOverlapCalculations() const;
    bool isMaximumFiguresInShadowOverlapCalculationsDefaulted() const;

    std::string polygonClippingAlgorithm() const;

    int pixelCountingResolution() const;

    std::string skyDiffuseModelingAlgorithm() const;

    bool outputExternalShadingCalculationResults() const;

    bool disableSelfShadingWithinShadingZoneGroups() const;

    bool disableSelfShadingFromShadingZoneGroupstoOtherZones() const;

    unsigned int numberofShadingZoneGroups() const;
    std::vector<ThermalZone> getShadingZoneGroup(unsigned groupIndex) const;

    //@}
    /** @name Setters */
    //@{

    bool setShadingCalculationMethod(const std::string& shadingCalculationMethod);
    // void resetShadingCalculationMethod();

    bool setShadingCalculationUpdateFrequencyMethod(const std::string& shadingCalculationUpdateFrequencyMethod);
    void resetShadingCalculationUpdateFrequencyMethod();

    bool setShadingCalculationUpdateFrequency(int shadingCalculationUpdateFrequency);
    void resetShadingCalculationUpdateFrequency();

    bool setMaximumFiguresInShadowOverlapCalculations(int maximumFiguresInShadowOverlapCalculations);
    void resetMaximumFiguresInShadowOverlapCalculations();

    bool setPolygonClippingAlgorithm(const std::string& polygonClippingAlgorithm);
    void resetPolygonClippingAlgorithm();

    bool setPixelCountingResolution(int pixelCountingResolution);
    // void resetPixelCountingResolution();

    bool setSkyDiffuseModelingAlgorithm(const std::string& skyDiffuseModelingAlgorithm);
    void resetSkyDiffuseModelingAlgorithm();

    bool setOutputExternalShadingCalculationResults(bool outputExternalShadingCalculationResults);

    bool setDisableSelfShadingWithinShadingZoneGroups(bool disableSelfShadingWithinShadingZoneGroups);

    bool setDisableSelfShadingFromShadingZoneGroupstoOtherZones(bool disableSelfShadingFromShadingZoneGroupstoOtherZones);

    bool addShadingZoneGroup(const std::vector<ThermalZone>& thermalZones);
    bool removeShadingZoneGroup(unsigned groupIndex);
    void removeAllShadingZoneGroups();

    //@}

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new ShadowCalculation object in the model.
    explicit ShadowCalculation(const Model& model);

    //@}

    /// @cond

    using ImplType = detail::ShadowCalculation_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit ShadowCalculation(std::shared_ptr<detail::ShadowCalculation_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ShadowCalculation");

    /// @endcond
  };

  /** \relates ShadowCalculation */
  using OptionalShadowCalculation = boost::optional<ShadowCalculation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADOWCALCULATION_HPP
