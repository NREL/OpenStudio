/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    //if (!modelObject.isFrameConductanceDefaulted()){
    idfObject.setDouble(WindowProperty_FrameAndDividerFields::FrameConductance, modelObject.frameConductance());
    //}

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
    idfObject.setInt(WindowProperty_FrameAndDividerFields::NumberofHorizontalDividers, modelObject.numberOfHorizontalDividers());
    //}

    //if (!modelObject.isNumberOfVerticalDividersDefaulted()){
    idfObject.setInt(WindowProperty_FrameAndDividerFields::NumberofVerticalDividers, modelObject.numberOfVerticalDividers());
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

    //if (!modelObject.isNFRCProductTypeforAssemblyCalculationsDefaulted()){
    idfObject.setString(WindowProperty_FrameAndDividerFields::NFRCProductTypeforAssemblyCalculations,
                        modelObject.nfrcProductTypeforAssemblyCalculations());
    //}

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
