/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTINGCONTROL_HPP
#define MODEL_DAYLIGHTINGCONTROL_HPP

#include "ModelAPI.hpp"
#include "SpaceItem.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

  namespace detail {

    class DaylightingControl_Impl;

  }  // namespace detail

  /** DaylightingControl is a SpaceItem that wraps the OpenStudio IDD object 'OS:Daylighting:Control'. */
  class MODEL_API DaylightingControl : public SpaceItem
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DaylightingControl(const Model& model);

    virtual ~DaylightingControl() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DaylightingControl(const DaylightingControl& other) = default;
    DaylightingControl(DaylightingControl&& other) = default;
    DaylightingControl& operator=(const DaylightingControl&) = default;
    DaylightingControl& operator=(DaylightingControl&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validLightingControlTypeValues();

    //@}
    /** @name Getters */
    //@{

    double positionXCoordinate() const;

    double positionYCoordinate() const;

    double positionZCoordinate() const;

    double psiRotationAroundXAxis() const;

    bool isPsiRotationAroundXAxisDefaulted() const;

    double thetaRotationAroundYAxis() const;

    bool isThetaRotationAroundYAxisDefaulted() const;

    double phiRotationAroundZAxis() const;

    bool isPhiRotationAroundZAxisDefaulted() const;

    double illuminanceSetpoint() const;

    bool isIlluminanceSetpointDefaulted() const;

    std::string lightingControlType() const;

    bool isLightingControlTypeDefaulted() const;

    double minimumInputPowerFractionforContinuousDimmingControl() const;

    bool isMinimumInputPowerFractionforContinuousDimmingControlDefaulted() const;

    double minimumLightOutputFractionforContinuousDimmingControl() const;

    bool isMinimumLightOutputFractionforContinuousDimmingControlDefaulted() const;

    int numberofSteppedControlSteps() const;

    bool isNumberofSteppedControlStepsDefaulted() const;

    double probabilityLightingwillbeResetWhenNeededinManualSteppedControl() const;

    bool isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted() const;

    int numberofDaylightingViews() const;

    bool isNumberofDaylightingViewsDefaulted() const;

    boost::optional<double> maximumAllowableDiscomfortGlareIndex() const;

    //@}
    /** @name Setters */
    //@{

    bool setPositionXCoordinate(double positionXCoordinate);

    bool setPositionYCoordinate(double positionYCoordinate);

    bool setPositionZCoordinate(double positionZCoordinate);

    bool setPsiRotationAroundXAxis(double psiRotationAroundXAxis);

    void resetPsiRotationAroundXAxis();

    bool setThetaRotationAroundYAxis(double thetaRotationAroundYAxis);

    void resetThetaRotationAroundYAxis();

    bool setPhiRotationAroundZAxis(double phiRotationAroundZAxis);

    void resetPhiRotationAroundZAxis();

    bool setIlluminanceSetpoint(double illuminanceSetpoint);

    void resetIlluminanceSetpoint();

    bool setLightingControlType(const std::string& lightingControlType);

    void resetLightingControlType();

    bool setMinimumInputPowerFractionforContinuousDimmingControl(double minimumInputPowerFractionforContinuousDimmingControl);

    void resetMinimumInputPowerFractionforContinuousDimmingControl();

    bool setMinimumLightOutputFractionforContinuousDimmingControl(double minimumLightOutputFractionforContinuousDimmingControl);

    void resetMinimumLightOutputFractionforContinuousDimmingControl();

    bool setNumberofSteppedControlSteps(int numberofSteppedControlSteps);

    void resetNumberofSteppedControlSteps();

    bool setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(double probabilityLightingwillbeResetWhenNeededinManualSteppedControl);

    void resetProbabilityLightingwillbeResetWhenNeededinManualSteppedControl();

    bool setNumberofDaylightingViews(int numberofDaylightingViews);

    void resetNumberofDaylightingViews();

    bool setMaximumAllowableDiscomfortGlareIndex(double maximumAllowableDiscomfortGlareIndex);

    void resetMaximumAllowableDiscomfortGlareIndex();

    //@}

    /// Returns a Point3d representing position only in meters relative to the Space.
    openstudio::Point3d position() const;

    /// Sets a Point3d representing position only position in meters relative to the Space.
    /// Does not change the orientation of the DaylightingControl.
    bool setPosition(const openstudio::Point3d& position);

    /// Returns a Transformation representing position and orientation in meters relative to the Space.
    openstudio::Transformation transformation() const;

    /** Sets a Transformation representing position and orientation in meters relative to the Space. */
    bool setTransformation(const openstudio::Transformation& transformation);

    /// Is this the primary control point for the containing ThermalZone.
    bool isPrimaryDaylightingControl() const;

    /// Is this the secondary control point for the containing ThermalZone.
    bool isSecondaryDaylightingControl() const;

    /// Keeps current position and orients the view towards the target, target is in space coordinates.
    bool aimAt(const Point3d& target);

   protected:
    /// @cond
    using ImplType = detail::DaylightingControl_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit DaylightingControl(std::shared_ptr<detail::DaylightingControl_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DaylightingControl");
  };

  /** \relates DaylightingControl*/
  using OptionalDaylightingControl = boost::optional<DaylightingControl>;

  /** \relates DaylightingControl*/
  using DaylightingControlVector = std::vector<DaylightingControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGCONTROL_HPP
