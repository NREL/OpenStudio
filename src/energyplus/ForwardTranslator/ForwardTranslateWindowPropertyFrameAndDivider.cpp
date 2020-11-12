/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../model/SubSurface.hpp"

#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/WindowProperty_FrameAndDivider_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateWindowPropertyFrameAndDivider(WindowPropertyFrameAndDivider& modelObject) {
    IdfObject idfObject(IddObjectType::WindowProperty_FrameAndDivider);
    m_idfObjects.push_back(idfObject);

    // DLM: do we need to make sure this is unique? not empty?
    boost::optional<std::string> s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // DLM: write OpenStudio defaults to IDF
    //if (!modelObject.isFrameWidthDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::FrameWidth, modelObject.frameWidth());
    //}

    //if (!modelObject.isFrameOutsideProjectionDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::FrameOutsideProjection, modelObject.frameOutsideProjection());
    //}

    //if (!modelObject.isFrameInsideProjectionDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::FrameInsideProjection, modelObject.frameInsideProjection());
    //}

    if (modelObject.frameConductance()) {
      idfObject.setDouble(WindowProperty_FrameAndDividerFields::FrameConductance, modelObject.frameConductance().get());
    }

    //if (!modelObject.isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::RatioofFrameEdgeGlassConductancetoCenterOfGlassConductance,
                        modelObject.ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance());
    //}

    //if (!modelObject.isFrameSolarAbsorptanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::FrameSolarAbsorptance, modelObject.frameSolarAbsorptance());
    //}

    //if (!modelObject.isFrameVisibleAbsorptanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::FrameVisibleAbsorptance, modelObject.frameVisibleAbsorptance());
    //}

    //if (!modelObject.isFrameThermalHemisphericalEmissivityDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::FrameThermalHemisphericalEmissivity, modelObject.frameThermalHemisphericalEmissivity());
    //}

    //if (!modelObject.isDividerTypeDefaulted()){
    idfObject.setString(WindowProperty_FrameAndDividerFields::DividerType, modelObject.dividerType());
    //}

    //if (!modelObject.isDividerWidthDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::DividerWidth, modelObject.dividerWidth());
    //}

    //if (!modelObject.isNumberOfHorizontalDividersDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::NumberofHorizontalDividers, modelObject.numberOfHorizontalDividers());
    //}

    //if (!modelObject.isNumberOfVerticalDividersDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::NumberofVerticalDividers, modelObject.numberOfVerticalDividers());
    //}

    //if (!modelObject.isDividerOutsideProjectionDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::DividerOutsideProjection, modelObject.dividerOutsideProjection());
    //}

    //if (!modelObject.isDividerInsideProjectionDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::DividerInsideProjection, modelObject.dividerInsideProjection());
    //}

    //if (!modelObject.isDividerConductanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::DividerConductance, modelObject.dividerConductance());
    //}

    //if (!modelObject.isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::RatioofDividerEdgeGlassConductancetoCenterOfGlassConductance,
                        modelObject.ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance());
    //}

    //if (!modelObject.isDividerSolarAbsorptanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::DividerSolarAbsorptance, modelObject.dividerSolarAbsorptance());
    //}

    //if (!modelObject.isDividerVisibleAbsorptanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::DividerVisibleAbsorptance, modelObject.dividerVisibleAbsorptance());
    //}

    //if (!modelObject.isDividerThermalHemisphericalEmissivityDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::DividerThermalHemisphericalEmissivity,
                        modelObject.dividerThermalHemisphericalEmissivity());
    //}

    // outsideRevealDepth mapped to surface vertices

    //if (!modelObject.isOutsideRevealSolarAbsorptanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::OutsideRevealSolarAbsorptance, modelObject.outsideRevealSolarAbsorptance());
    //}

    //if (!modelObject.isInsideSillDepthDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::InsideSillDepth, modelObject.insideSillDepth());
    //}

    //if (!modelObject.isInsideSillSolarAbsorptanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::InsideSillSolarAbsorptance, modelObject.insideSillSolarAbsorptance());
    //}

    //if (!modelObject.isInsideRevealDepthDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::InsideRevealDepth, modelObject.insideRevealDepth());
    //}

    //if (!modelObject.isInsideRevealSolarAbsorptanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::InsideRevealSolarAbsorptance, modelObject.insideRevealSolarAbsorptance());
    //}

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
