/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    boost::optional<int> i = workspaceObject.getInt(WindowProperty_FrameAndDividerFields::NumberofHorizontalDividers);
    if (i) {
      result.setNumberOfHorizontalDividers(*i);
    }

    i = workspaceObject.getInt(WindowProperty_FrameAndDividerFields::NumberofVerticalDividers);
    if (i) {
      result.setNumberOfVerticalDividers(*i);
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

    s = workspaceObject.getString(WindowProperty_FrameAndDividerFields::NFRCProductTypeforAssemblyCalculations);
    if (s) {
      result.setNFRCProductTypeforAssemblyCalculations(*s);
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
