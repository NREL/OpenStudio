/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WINDOWPROPERTYFRAMEANDDIVIDER_HPP
#define MODEL_WINDOWPROPERTYFRAMEANDDIVIDER_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class WindowPropertyFrameAndDivider_Impl;

  }  // namespace detail

  /** WindowPropertyFrameAndDivider is a ResourceObject that wraps the OpenStudio IDD object 'OS:WindowProperty:FrameAndDivider'. */
  class MODEL_API WindowPropertyFrameAndDivider : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit WindowPropertyFrameAndDivider(const Model& model);

    virtual ~WindowPropertyFrameAndDivider() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WindowPropertyFrameAndDivider(const WindowPropertyFrameAndDivider& other) = default;
    WindowPropertyFrameAndDivider(WindowPropertyFrameAndDivider&& other) = default;
    WindowPropertyFrameAndDivider& operator=(const WindowPropertyFrameAndDivider&) = default;
    WindowPropertyFrameAndDivider& operator=(WindowPropertyFrameAndDivider&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> dividerTypeValues();

    /** @name Getters */
    //@{

    double frameWidth() const;

    bool isFrameWidthDefaulted() const;

    double frameOutsideProjection() const;

    bool isFrameOutsideProjectionDefaulted() const;

    double frameInsideProjection() const;

    bool isFrameInsideProjectionDefaulted() const;

    double frameConductance() const;

    bool isFrameConductanceDefaulted() const;

    double ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance() const;

    bool isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const;

    double frameSolarAbsorptance() const;

    bool isFrameSolarAbsorptanceDefaulted() const;

    double frameVisibleAbsorptance() const;

    bool isFrameVisibleAbsorptanceDefaulted() const;

    double frameThermalHemisphericalEmissivity() const;

    bool isFrameThermalHemisphericalEmissivityDefaulted() const;

    std::string dividerType() const;

    bool isDividerTypeDefaulted() const;

    double dividerWidth() const;

    bool isDividerWidthDefaulted() const;

    int numberOfHorizontalDividers() const;

    bool isNumberOfHorizontalDividersDefaulted() const;

    int numberOfVerticalDividers() const;

    bool isNumberOfVerticalDividersDefaulted() const;

    double dividerOutsideProjection() const;

    bool isDividerOutsideProjectionDefaulted() const;

    double dividerInsideProjection() const;

    bool isDividerInsideProjectionDefaulted() const;

    double dividerConductance() const;

    bool isDividerConductanceDefaulted() const;

    double ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance() const;

    bool isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const;

    double dividerSolarAbsorptance() const;

    bool isDividerSolarAbsorptanceDefaulted() const;

    double dividerVisibleAbsorptance() const;

    bool isDividerVisibleAbsorptanceDefaulted() const;

    double dividerThermalHemisphericalEmissivity() const;

    bool isDividerThermalHemisphericalEmissivityDefaulted() const;

    double outsideRevealDepth() const;

    bool isOutsideRevealDepthDefaulted() const;

    double outsideRevealSolarAbsorptance() const;

    bool isOutsideRevealSolarAbsorptanceDefaulted() const;

    double insideSillDepth() const;

    bool isInsideSillDepthDefaulted() const;

    double insideSillSolarAbsorptance() const;

    bool isInsideSillSolarAbsorptanceDefaulted() const;

    double insideRevealDepth() const;

    bool isInsideRevealDepthDefaulted() const;

    double insideRevealSolarAbsorptance() const;

    bool isInsideRevealSolarAbsorptanceDefaulted() const;

    std::string nfrcProductTypeforAssemblyCalculations() const;

    bool isNFRCProductTypeforAssemblyCalculationsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setFrameWidth(double frameWidth);

    void resetFrameWidth();

    bool setFrameOutsideProjection(double frameOutsideProjection);

    void resetFrameOutsideProjection();

    bool setFrameInsideProjection(double frameInsideProjection);

    void resetFrameInsideProjection();

    bool setFrameConductance(double frameConductance);

    void resetFrameConductance();

    bool setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance(double ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance);

    void resetRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance();

    bool setFrameSolarAbsorptance(double frameSolarAbsorptance);

    void resetFrameSolarAbsorptance();

    bool setFrameVisibleAbsorptance(double frameVisibleAbsorptance);

    void resetFrameVisibleAbsorptance();

    bool setFrameThermalHemisphericalEmissivity(double frameThermalHemisphericalEmissivity);

    void resetFrameThermalHemisphericalEmissivity();

    bool setDividerType(const std::string& dividerType);

    void resetDividerType();

    bool setDividerWidth(double dividerWidth);

    void resetDividerWidth();

    bool setNumberOfHorizontalDividers(int numberOfHorizontalDividers);

    void resetNumberOfHorizontalDividers();

    bool setNumberOfVerticalDividers(int numberOfVerticalDividers);

    void resetNumberOfVerticalDividers();

    bool setDividerOutsideProjection(double dividerOutsideProjection);

    void resetDividerOutsideProjection();

    bool setDividerInsideProjection(double dividerInsideProjection);

    void resetDividerInsideProjection();

    bool setDividerConductance(double dividerConductance);

    void resetDividerConductance();

    bool setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance(double ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance);

    void resetRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance();

    bool setDividerSolarAbsorptance(double dividerSolarAbsorptance);

    void resetDividerSolarAbsorptance();

    bool setDividerVisibleAbsorptance(double dividerVisibleAbsorptance);

    void resetDividerVisibleAbsorptance();

    bool setDividerThermalHemisphericalEmissivity(double dividerThermalHemisphericalEmissivity);

    void resetDividerThermalHemisphericalEmissivity();

    bool setOutsideRevealDepth(double outsideRevealDepth);

    void resetOutsideRevealDepth();

    bool setOutsideRevealSolarAbsorptance(double outsideRevealSolarAbsorptance);

    void resetOutsideRevealSolarAbsorptance();

    bool setInsideSillDepth(double insideSillDepth);

    void resetInsideSillDepth();

    bool setInsideSillSolarAbsorptance(double insideSillSolarAbsorptance);

    void resetInsideSillSolarAbsorptance();

    bool setInsideRevealDepth(double insideRevealDepth);

    void resetInsideRevealDepth();

    bool setInsideRevealSolarAbsorptance(double insideRevealSolarAbsorptance);

    void resetInsideRevealSolarAbsorptance();

    bool setNFRCProductTypeforAssemblyCalculations(const std::string& nfrcProductTypeforAssemblyCalculations);

    void resetNFRCProductTypeforAssemblyCalculations();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::WindowPropertyFrameAndDivider_Impl;

    explicit WindowPropertyFrameAndDivider(std::shared_ptr<detail::WindowPropertyFrameAndDivider_Impl> impl);

    friend class detail::WindowPropertyFrameAndDivider_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.WindowPropertyFrameAndDivider");
  };

  /** \relates WindowPropertyFrameAndDivider*/
  using OptionalWindowPropertyFrameAndDivider = boost::optional<WindowPropertyFrameAndDivider>;

  /** \relates WindowPropertyFrameAndDivider*/
  using WindowPropertyFrameAndDividerVector = std::vector<WindowPropertyFrameAndDivider>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WINDOWPROPERTYFRAMEANDDIVIDER_HPP
