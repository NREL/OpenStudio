/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
  class MODEL_API DaylightingControl_Impl : public SpaceItem_Impl {
    Q_OBJECT;
    Q_PROPERTY(double positionXCoordinate READ positionXCoordinate WRITE setPositionXCoordinate);
    Q_PROPERTY(double positionYCoordinate READ positionYCoordinate WRITE setPositionYCoordinate);
    Q_PROPERTY(double positionZCoordinate READ positionZCoordinate WRITE setPositionZCoordinate);
    Q_PROPERTY(double psiRotationAroundXAxis READ psiRotationAroundXAxis WRITE setPsiRotationAroundXAxis RESET resetPsiRotationAroundXAxis);
    Q_PROPERTY(bool isPsiRotationAroundXAxisDefaulted READ isPsiRotationAroundXAxisDefaulted);
    Q_PROPERTY(double thetaRotationAroundYAxis READ thetaRotationAroundYAxis WRITE setThetaRotationAroundYAxis RESET resetThetaRotationAroundYAxis);
    Q_PROPERTY(bool isThetaRotationAroundYAxisDefaulted READ isThetaRotationAroundYAxisDefaulted);
    Q_PROPERTY(double phiRotationAroundZAxis READ phiRotationAroundZAxis WRITE setPhiRotationAroundZAxis RESET resetPhiRotationAroundZAxis);
    Q_PROPERTY(bool isPhiRotationAroundZAxisDefaulted READ isPhiRotationAroundZAxisDefaulted);
    Q_PROPERTY(double illuminanceSetpoint READ illuminanceSetpoint WRITE setIlluminanceSetpoint RESET resetIlluminanceSetpoint);
    Q_PROPERTY(bool isIlluminanceSetpointDefaulted READ isIlluminanceSetpointDefaulted);
    Q_PROPERTY(std::string lightingControlType READ lightingControlType WRITE setLightingControlType RESET resetLightingControlType);
    Q_PROPERTY(bool isLightingControlTypeDefaulted READ isLightingControlTypeDefaulted);
    Q_PROPERTY(double minimumInputPowerFractionforContinuousDimmingControl READ minimumInputPowerFractionforContinuousDimmingControl WRITE setMinimumInputPowerFractionforContinuousDimmingControl RESET resetMinimumInputPowerFractionforContinuousDimmingControl);
    Q_PROPERTY(bool isMinimumInputPowerFractionforContinuousDimmingControlDefaulted READ isMinimumInputPowerFractionforContinuousDimmingControlDefaulted);
    Q_PROPERTY(double minimumLightOutputFractionforContinuousDimmingControl READ minimumLightOutputFractionforContinuousDimmingControl WRITE setMinimumLightOutputFractionforContinuousDimmingControl RESET resetMinimumLightOutputFractionforContinuousDimmingControl);
    Q_PROPERTY(bool isMinimumLightOutputFractionforContinuousDimmingControlDefaulted READ isMinimumLightOutputFractionforContinuousDimmingControlDefaulted);
    Q_PROPERTY(int numberofSteppedControlSteps READ numberofSteppedControlSteps WRITE setNumberofSteppedControlSteps RESET resetNumberofSteppedControlSteps);
    Q_PROPERTY(bool isNumberofSteppedControlStepsDefaulted READ isNumberofSteppedControlStepsDefaulted);
    Q_PROPERTY(double probabilityLightingwillbeResetWhenNeededinManualSteppedControl READ probabilityLightingwillbeResetWhenNeededinManualSteppedControl WRITE setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl RESET resetProbabilityLightingwillbeResetWhenNeededinManualSteppedControl);
    Q_PROPERTY(bool isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted READ isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted);
    Q_PROPERTY(int numberofDaylightingViews READ numberofDaylightingViews WRITE setNumberofDaylightingViews RESET resetNumberofDaylightingViews);
    Q_PROPERTY(bool isNumberofDaylightingViewsDefaulted READ isNumberofDaylightingViewsDefaulted);
    Q_PROPERTY(boost::optional<double> maximumAllowableDiscomfortGlareIndex READ maximumAllowableDiscomfortGlareIndex WRITE setMaximumAllowableDiscomfortGlareIndex RESET resetMaximumAllowableDiscomfortGlareIndex);
   public:
    /** @name Constructors and Destructors */
    //@{

    DaylightingControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    DaylightingControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    DaylightingControl_Impl(const DaylightingControl_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    virtual ~DaylightingControl_Impl() {}

    //@}

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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

    void setPositionXCoordinate(double positionXCoordinate);

    void setPositionYCoordinate(double positionYCoordinate);

    void setPositionZCoordinate(double positionZCoordinate);


    void setPsiRotationAroundXAxis(double psiRotationAroundXAxis);

    void resetPsiRotationAroundXAxis();

    void setThetaRotationAroundYAxis(double thetaRotationAroundYAxis);

    void resetThetaRotationAroundYAxis();

    void setPhiRotationAroundZAxis(double phiRotationAroundZAxis);

    void resetPhiRotationAroundZAxis();

    bool setIlluminanceSetpoint(double illuminanceSetpoint);

    void resetIlluminanceSetpoint();

    bool setLightingControlType(std::string lightingControlType);

    void resetLightingControlType();

    bool setMinimumInputPowerFractionforContinuousDimmingControl(double minimumInputPowerFractionforContinuousDimmingControl);

    void resetMinimumInputPowerFractionforContinuousDimmingControl();

    bool setMinimumLightOutputFractionforContinuousDimmingControl(double minimumLightOutputFractionforContinuousDimmingControl);

    void resetMinimumLightOutputFractionforContinuousDimmingControl();

    void setNumberofSteppedControlSteps(int numberofSteppedControlSteps);

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

    bool aimAt(const Point3d& point);

   protected: 

    // index of the space name
    virtual int spaceIndex() const;

   private:
    REGISTER_LOGGER("openstudio.model.DaylightingControl");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_DAYLIGHTINGCONTROL_IMPL_HPP

