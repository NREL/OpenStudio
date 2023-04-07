/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

      virtual ~DaylightingControl_Impl() = default;

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
