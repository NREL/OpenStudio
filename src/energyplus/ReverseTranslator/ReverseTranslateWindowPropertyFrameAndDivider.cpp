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

#include "../ReverseTranslator.hpp"

#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../utilities/idd/IddObject.hpp"

#include <utilities/idd/WindowProperty_FrameAndDivider_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateWindowPropertyFrameAndDivider(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::WindowProperty_FrameAndDivider) {
      LOG(Error, "WorkspaceObject is not IddObjectType: WindowProperty_FrameAndDivider");
      return boost::none;
    }

    model::WindowPropertyFrameAndDivider result(m_model);

    boost::optional<std::string> s = workspaceObject.getString(WindowProperty_FrameAndDividerFields::Name);
    if (s) {
      result.setName(*s);
    }

    boost::optional<double> d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::FrameWidth);
    if (d) {
      result.setFrameWidth(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::FrameOutsideProjection);
    if (d) {
      result.setFrameOutsideProjection(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::FrameInsideProjection);
    if (d) {
      result.setFrameInsideProjection(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::FrameConductance);
    if (d) {
      result.setFrameConductance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::RatioofFrameEdgeGlassConductancetoCenterOfGlassConductance);
    if (d) {
      result.setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::FrameSolarAbsorptance);
    if (d) {
      result.setFrameSolarAbsorptance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::FrameVisibleAbsorptance);
    if (d) {
      result.setFrameVisibleAbsorptance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::FrameThermalHemisphericalEmissivity);
    if (d) {
      result.setFrameThermalHemisphericalEmissivity(*d);
    }

    s = workspaceObject.getString(WindowProperty_FrameAndDividerFields::DividerType);
    if (s) {
      result.setDividerType(*s);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::DividerWidth);
    if (d) {
      result.setDividerWidth(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::NumberofHorizontalDividers);
    if (d) {
      result.setNumberOfHorizontalDividers(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::NumberofVerticalDividers);
    if (d) {
      result.setNumberOfVerticalDividers(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::DividerOutsideProjection);
    if (d) {
      result.setDividerOutsideProjection(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::DividerInsideProjection);
    if (d) {
      result.setDividerInsideProjection(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::DividerConductance);
    if (d) {
      result.setDividerConductance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::RatioofDividerEdgeGlassConductancetoCenterOfGlassConductance);
    if (d) {
      result.setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::DividerSolarAbsorptance);
    if (d) {
      result.setDividerSolarAbsorptance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::DividerVisibleAbsorptance);
    if (d) {
      result.setDividerVisibleAbsorptance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::DividerThermalHemisphericalEmissivity);
    if (d) {
      result.setDividerThermalHemisphericalEmissivity(*d);
    }

    // DLM: could attempt to set outside reveal depth here but that would require projecting sub surfaces to surfaces
    // and making unique frame and divider object for each sub surface

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::OutsideRevealSolarAbsorptance);
    if (d) {
      result.setOutsideRevealSolarAbsorptance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::InsideSillDepth);
    if (d) {
      result.setInsideSillDepth(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::InsideSillSolarAbsorptance);
    if (d) {
      result.setInsideSillSolarAbsorptance(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::InsideRevealDepth);
    if (d) {
      result.setInsideRevealDepth(*d);
    }

    d = workspaceObject.getDouble(WindowProperty_FrameAndDividerFields::InsideRevealSolarAbsorptance);
    if (d) {
      result.setInsideRevealSolarAbsorptance(*d);
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
