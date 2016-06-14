/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
    virtual int spaceIndex() const override;

   private:
    REGISTER_LOGGER("openstudio.model.DaylightingControl");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_DAYLIGHTINGCONTROL_IMPL_HPP

