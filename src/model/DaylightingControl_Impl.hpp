/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTINGCONTROL_IMPL_HPP
#define MODEL_DAYLIGHTINGCONTROL_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceItem_Impl.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

  class DaylightingControl;

  namespace detail {

    /** DaylightingControl_Impl is a SpaceItem_Impl that is the implementation class for DaylightingControl.*/
    class MODEL_API DaylightingControl_Impl : public SpaceItem_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DaylightingControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DaylightingControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DaylightingControl_Impl(const DaylightingControl_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DaylightingControl_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

      bool setMaximumAllowableDiscomfortGlareIndex(boost::optional<double> maximumAllowableDiscomfortGlareIndex);

      void resetMaximumAllowableDiscomfortGlareIndex();

      //@}

      openstudio::Point3d position() const;

      bool setPosition(const openstudio::Point3d& position);

      openstudio::Transformation transformation() const;

      bool setTransformation(const openstudio::Transformation& transformation);

      bool isPrimaryDaylightingControl() const;

      bool isSecondaryDaylightingControl() const;

      bool aimAt(const Point3d& target);

     protected:
      // index of the space name
      virtual int spaceIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.DaylightingControl");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGCONTROL_IMPL_HPP
