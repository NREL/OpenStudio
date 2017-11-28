/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "WindowPropertyFrameAndDivider.hpp"
#include "WindowPropertyFrameAndDivider_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_WindowProperty_FrameAndDivider_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  WindowPropertyFrameAndDivider_Impl::WindowPropertyFrameAndDivider_Impl(const IdfObject& idfObject,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WindowPropertyFrameAndDivider::iddObjectType());
  }

  WindowPropertyFrameAndDivider_Impl::WindowPropertyFrameAndDivider_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WindowPropertyFrameAndDivider::iddObjectType());
  }

  WindowPropertyFrameAndDivider_Impl::WindowPropertyFrameAndDivider_Impl(const WindowPropertyFrameAndDivider_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& WindowPropertyFrameAndDivider_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType WindowPropertyFrameAndDivider_Impl::iddObjectType() const {
    return WindowPropertyFrameAndDivider::iddObjectType();
  }

  double WindowPropertyFrameAndDivider_Impl::frameWidth() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::FrameWidth,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isFrameWidthDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::FrameWidth);
  }

  double WindowPropertyFrameAndDivider_Impl::frameOutsideProjection() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::FrameOutsideProjection,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isFrameOutsideProjectionDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::FrameOutsideProjection);
  }

  double WindowPropertyFrameAndDivider_Impl::frameInsideProjection() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::FrameInsideProjection,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isFrameInsideProjectionDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::FrameInsideProjection);
  }

  boost::optional<double> WindowPropertyFrameAndDivider_Impl::frameConductance() const {
    return getDouble(OS_WindowProperty_FrameAndDividerFields::FrameConductance,true);
  }

  double WindowPropertyFrameAndDivider_Impl::ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::RatioofFrameEdgeGlassConductancetoCenterOfGlassConductance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::RatioofFrameEdgeGlassConductancetoCenterOfGlassConductance);
  }

  double WindowPropertyFrameAndDivider_Impl::frameSolarAbsorptance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::FrameSolarAbsorptance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isFrameSolarAbsorptanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::FrameSolarAbsorptance);
  }

  double WindowPropertyFrameAndDivider_Impl::frameVisibleAbsorptance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::FrameVisibleAbsorptance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isFrameVisibleAbsorptanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::FrameVisibleAbsorptance);
  }

  double WindowPropertyFrameAndDivider_Impl::frameThermalHemisphericalEmissivity() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::FrameThermalHemisphericalEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isFrameThermalHemisphericalEmissivityDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::FrameThermalHemisphericalEmissivity);
  }

  std::string WindowPropertyFrameAndDivider_Impl::dividerType() const {
    boost::optional<std::string> value = getString(OS_WindowProperty_FrameAndDividerFields::DividerType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isDividerTypeDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::DividerType);
  }

  double WindowPropertyFrameAndDivider_Impl::dividerWidth() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::DividerWidth,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isDividerWidthDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::DividerWidth);
  }

  double WindowPropertyFrameAndDivider_Impl::numberOfHorizontalDividers() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::NumberofHorizontalDividers,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isNumberOfHorizontalDividersDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::NumberofHorizontalDividers);
  }

  double WindowPropertyFrameAndDivider_Impl::numberOfVerticalDividers() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::NumberofVerticalDividers,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isNumberOfVerticalDividersDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::NumberofVerticalDividers);
  }

  double WindowPropertyFrameAndDivider_Impl::dividerOutsideProjection() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::DividerOutsideProjection,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isDividerOutsideProjectionDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::DividerOutsideProjection);
  }

  double WindowPropertyFrameAndDivider_Impl::dividerInsideProjection() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::DividerInsideProjection,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isDividerInsideProjectionDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::DividerInsideProjection);
  }

  double WindowPropertyFrameAndDivider_Impl::dividerConductance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::DividerConductance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isDividerConductanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::DividerConductance);
  }

  double WindowPropertyFrameAndDivider_Impl::ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::RatioofDividerEdgeGlassConductancetoCenterOfGlassConductance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::RatioofDividerEdgeGlassConductancetoCenterOfGlassConductance);
  }

  double WindowPropertyFrameAndDivider_Impl::dividerSolarAbsorptance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::DividerSolarAbsorptance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isDividerSolarAbsorptanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::DividerSolarAbsorptance);
  }

  double WindowPropertyFrameAndDivider_Impl::dividerVisibleAbsorptance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::DividerVisibleAbsorptance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isDividerVisibleAbsorptanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::DividerVisibleAbsorptance);
  }

  double WindowPropertyFrameAndDivider_Impl::dividerThermalHemisphericalEmissivity() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::DividerThermalHemisphericalEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isDividerThermalHemisphericalEmissivityDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::DividerThermalHemisphericalEmissivity);
  }

  double WindowPropertyFrameAndDivider_Impl::outsideRevealDepth() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::OutsideRevealDepth,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isOutsideRevealDepthDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::OutsideRevealDepth);
  }

  double WindowPropertyFrameAndDivider_Impl::outsideRevealSolarAbsorptance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::OutsideRevealSolarAbsorptance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isOutsideRevealSolarAbsorptanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::OutsideRevealSolarAbsorptance);
  }

  double WindowPropertyFrameAndDivider_Impl::insideSillDepth() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::InsideSillDepth,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isInsideSillDepthDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::InsideSillDepth);
  }

  double WindowPropertyFrameAndDivider_Impl::insideSillSolarAbsorptance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::InsideSillSolarAbsorptance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isInsideSillSolarAbsorptanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::InsideSillSolarAbsorptance);
  }

  double WindowPropertyFrameAndDivider_Impl::insideRevealDepth() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::InsideRevealDepth,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isInsideRevealDepthDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::InsideRevealDepth);
  }

  double WindowPropertyFrameAndDivider_Impl::insideRevealSolarAbsorptance() const {
    boost::optional<double> value = getDouble(OS_WindowProperty_FrameAndDividerFields::InsideRevealSolarAbsorptance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WindowPropertyFrameAndDivider_Impl::isInsideRevealSolarAbsorptanceDefaulted() const {
    return isEmpty(OS_WindowProperty_FrameAndDividerFields::InsideRevealSolarAbsorptance);
  }

  bool WindowPropertyFrameAndDivider_Impl::setFrameWidth(double frameWidth) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::FrameWidth, frameWidth);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetFrameWidth() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::FrameWidth, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setFrameOutsideProjection(double frameOutsideProjection) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::FrameOutsideProjection, frameOutsideProjection);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetFrameOutsideProjection() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::FrameOutsideProjection, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setFrameInsideProjection(double frameInsideProjection) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::FrameInsideProjection, frameInsideProjection);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetFrameInsideProjection() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::FrameInsideProjection, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setFrameConductance(boost::optional<double> frameConductance) {
    bool result(false);
    if (frameConductance) {
      result = setDouble(OS_WindowProperty_FrameAndDividerFields::FrameConductance, frameConductance.get());
    }
    else {
      resetFrameConductance();
      result = true;
    }
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetFrameConductance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::FrameConductance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance(double ratioOfFrameEdgeGlassConductancetoCenterOfGlassConductance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::RatioofFrameEdgeGlassConductancetoCenterOfGlassConductance, ratioOfFrameEdgeGlassConductancetoCenterOfGlassConductance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::RatioofFrameEdgeGlassConductancetoCenterOfGlassConductance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setFrameSolarAbsorptance(double frameSolarAbsorptance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::FrameSolarAbsorptance, frameSolarAbsorptance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetFrameSolarAbsorptance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::FrameSolarAbsorptance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setFrameVisibleAbsorptance(double frameVisibleAbsorptance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::FrameVisibleAbsorptance, frameVisibleAbsorptance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetFrameVisibleAbsorptance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::FrameVisibleAbsorptance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setFrameThermalHemisphericalEmissivity(double frameThermalHemisphericalEmissivity) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::FrameThermalHemisphericalEmissivity, frameThermalHemisphericalEmissivity);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetFrameThermalHemisphericalEmissivity() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::FrameThermalHemisphericalEmissivity, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setDividerType(std::string dividerType) {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::DividerType, dividerType);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetDividerType() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::DividerType, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setDividerWidth(double dividerWidth) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::DividerWidth, dividerWidth);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetDividerWidth() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::DividerWidth, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setNumberOfHorizontalDividers(double numberofHorizontalDividers) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::NumberofHorizontalDividers, numberofHorizontalDividers);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetNumberOfHorizontalDividers() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::NumberofHorizontalDividers, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setNumberOfVerticalDividers(double numberofVerticalDividers) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::NumberofVerticalDividers, numberofVerticalDividers);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetNumberOfVerticalDividers() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::NumberofVerticalDividers, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setDividerOutsideProjection(double dividerOutsideProjection) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::DividerOutsideProjection, dividerOutsideProjection);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetDividerOutsideProjection() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::DividerOutsideProjection, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setDividerInsideProjection(double dividerInsideProjection) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::DividerInsideProjection, dividerInsideProjection);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetDividerInsideProjection() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::DividerInsideProjection, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setDividerConductance(double dividerConductance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::DividerConductance, dividerConductance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetDividerConductance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::DividerConductance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance(double ratioofDividerEdgeGlassConductancetoCenterOfGlassConductance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::RatioofDividerEdgeGlassConductancetoCenterOfGlassConductance, ratioofDividerEdgeGlassConductancetoCenterOfGlassConductance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::RatioofDividerEdgeGlassConductancetoCenterOfGlassConductance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setDividerSolarAbsorptance(double dividerSolarAbsorptance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::DividerSolarAbsorptance, dividerSolarAbsorptance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetDividerSolarAbsorptance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::DividerSolarAbsorptance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setDividerVisibleAbsorptance(double dividerVisibleAbsorptance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::DividerVisibleAbsorptance, dividerVisibleAbsorptance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetDividerVisibleAbsorptance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::DividerVisibleAbsorptance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setDividerThermalHemisphericalEmissivity(double dividerThermalHemisphericalEmissivity) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::DividerThermalHemisphericalEmissivity, dividerThermalHemisphericalEmissivity);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetDividerThermalHemisphericalEmissivity() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::DividerThermalHemisphericalEmissivity, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setOutsideRevealDepth(double outsideRevealDepth) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::OutsideRevealDepth, outsideRevealDepth);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetOutsideRevealDepth() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::OutsideRevealDepth, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setOutsideRevealSolarAbsorptance(double outsideRevealSolarAbsorptance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::OutsideRevealSolarAbsorptance, outsideRevealSolarAbsorptance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetOutsideRevealSolarAbsorptance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::OutsideRevealSolarAbsorptance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setInsideSillDepth(double insideSillDepth) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::InsideSillDepth, insideSillDepth);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetInsideSillDepth() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::InsideSillDepth, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setInsideSillSolarAbsorptance(double insideSillSolarAbsorptance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::InsideSillSolarAbsorptance, insideSillSolarAbsorptance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetInsideSillSolarAbsorptance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::InsideSillSolarAbsorptance, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setInsideRevealDepth(double insideRevealDepth) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::InsideRevealDepth, insideRevealDepth);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetInsideRevealDepth() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::InsideRevealDepth, "");
    OS_ASSERT(result);
  }

  bool WindowPropertyFrameAndDivider_Impl::setInsideRevealSolarAbsorptance(double insideRevealSolarAbsorptance) {
    bool result = setDouble(OS_WindowProperty_FrameAndDividerFields::InsideRevealSolarAbsorptance, insideRevealSolarAbsorptance);
    return result;
  }

  void WindowPropertyFrameAndDivider_Impl::resetInsideRevealSolarAbsorptance() {
    bool result = setString(OS_WindowProperty_FrameAndDividerFields::InsideRevealSolarAbsorptance, "");
    OS_ASSERT(result);
  }

} // detail

WindowPropertyFrameAndDivider::WindowPropertyFrameAndDivider(const Model& model)
  : ResourceObject(WindowPropertyFrameAndDivider::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WindowPropertyFrameAndDivider_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType WindowPropertyFrameAndDivider::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WindowProperty_FrameAndDivider);
}

std::vector<std::string> WindowPropertyFrameAndDivider::dividerTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WindowProperty_FrameAndDividerFields::DividerType);
}

double WindowPropertyFrameAndDivider::frameWidth() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->frameWidth();
}

bool WindowPropertyFrameAndDivider::isFrameWidthDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isFrameWidthDefaulted();
}

double WindowPropertyFrameAndDivider::frameOutsideProjection() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->frameOutsideProjection();
}

bool WindowPropertyFrameAndDivider::isFrameOutsideProjectionDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isFrameOutsideProjectionDefaulted();
}

double WindowPropertyFrameAndDivider::frameInsideProjection() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->frameInsideProjection();
}

bool WindowPropertyFrameAndDivider::isFrameInsideProjectionDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isFrameInsideProjectionDefaulted();
}

boost::optional<double> WindowPropertyFrameAndDivider::frameConductance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->frameConductance();
}

double WindowPropertyFrameAndDivider::ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance();
}

bool WindowPropertyFrameAndDivider::isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted();
}

double WindowPropertyFrameAndDivider::frameSolarAbsorptance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->frameSolarAbsorptance();
}

bool WindowPropertyFrameAndDivider::isFrameSolarAbsorptanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isFrameSolarAbsorptanceDefaulted();
}

double WindowPropertyFrameAndDivider::frameVisibleAbsorptance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->frameVisibleAbsorptance();
}

bool WindowPropertyFrameAndDivider::isFrameVisibleAbsorptanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isFrameVisibleAbsorptanceDefaulted();
}

double WindowPropertyFrameAndDivider::frameThermalHemisphericalEmissivity() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->frameThermalHemisphericalEmissivity();
}

bool WindowPropertyFrameAndDivider::isFrameThermalHemisphericalEmissivityDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isFrameThermalHemisphericalEmissivityDefaulted();
}

std::string WindowPropertyFrameAndDivider::dividerType() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->dividerType();
}

bool WindowPropertyFrameAndDivider::isDividerTypeDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isDividerTypeDefaulted();
}

double WindowPropertyFrameAndDivider::dividerWidth() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->dividerWidth();
}

bool WindowPropertyFrameAndDivider::isDividerWidthDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isDividerWidthDefaulted();
}

double WindowPropertyFrameAndDivider::numberOfHorizontalDividers() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->numberOfHorizontalDividers();
}

bool WindowPropertyFrameAndDivider::isNumberOfHorizontalDividersDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isNumberOfHorizontalDividersDefaulted();
}

double WindowPropertyFrameAndDivider::numberOfVerticalDividers() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->numberOfVerticalDividers();
}

bool WindowPropertyFrameAndDivider::isNumberOfVerticalDividersDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isNumberOfVerticalDividersDefaulted();
}

double WindowPropertyFrameAndDivider::dividerOutsideProjection() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->dividerOutsideProjection();
}

bool WindowPropertyFrameAndDivider::isDividerOutsideProjectionDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isDividerOutsideProjectionDefaulted();
}

double WindowPropertyFrameAndDivider::dividerInsideProjection() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->dividerInsideProjection();
}

bool WindowPropertyFrameAndDivider::isDividerInsideProjectionDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isDividerInsideProjectionDefaulted();
}

double WindowPropertyFrameAndDivider::dividerConductance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->dividerConductance();
}

bool WindowPropertyFrameAndDivider::isDividerConductanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isDividerConductanceDefaulted();
}

double WindowPropertyFrameAndDivider::ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance();
}

bool WindowPropertyFrameAndDivider::isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted();
}

double WindowPropertyFrameAndDivider::dividerSolarAbsorptance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->dividerSolarAbsorptance();
}

bool WindowPropertyFrameAndDivider::isDividerSolarAbsorptanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isDividerSolarAbsorptanceDefaulted();
}

double WindowPropertyFrameAndDivider::dividerVisibleAbsorptance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->dividerVisibleAbsorptance();
}

bool WindowPropertyFrameAndDivider::isDividerVisibleAbsorptanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isDividerVisibleAbsorptanceDefaulted();
}

double WindowPropertyFrameAndDivider::dividerThermalHemisphericalEmissivity() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->dividerThermalHemisphericalEmissivity();
}

bool WindowPropertyFrameAndDivider::isDividerThermalHemisphericalEmissivityDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isDividerThermalHemisphericalEmissivityDefaulted();
}

double WindowPropertyFrameAndDivider::outsideRevealDepth() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->outsideRevealDepth();
}

bool WindowPropertyFrameAndDivider::isOutsideRevealDepthDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isOutsideRevealDepthDefaulted();
}

double WindowPropertyFrameAndDivider::outsideRevealSolarAbsorptance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->outsideRevealSolarAbsorptance();
}

bool WindowPropertyFrameAndDivider::isOutsideRevealSolarAbsorptanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isOutsideRevealSolarAbsorptanceDefaulted();
}

double WindowPropertyFrameAndDivider::insideSillDepth() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->insideSillDepth();
}

bool WindowPropertyFrameAndDivider::isInsideSillDepthDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isInsideSillDepthDefaulted();
}

double WindowPropertyFrameAndDivider::insideSillSolarAbsorptance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->insideSillSolarAbsorptance();
}

bool WindowPropertyFrameAndDivider::isInsideSillSolarAbsorptanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isInsideSillSolarAbsorptanceDefaulted();
}

double WindowPropertyFrameAndDivider::insideRevealDepth() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->insideRevealDepth();
}

bool WindowPropertyFrameAndDivider::isInsideRevealDepthDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isInsideRevealDepthDefaulted();
}

double WindowPropertyFrameAndDivider::insideRevealSolarAbsorptance() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->insideRevealSolarAbsorptance();
}

bool WindowPropertyFrameAndDivider::isInsideRevealSolarAbsorptanceDefaulted() const {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->isInsideRevealSolarAbsorptanceDefaulted();
}

bool WindowPropertyFrameAndDivider::setFrameWidth(double frameWidth) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setFrameWidth(frameWidth);
}

void WindowPropertyFrameAndDivider::resetFrameWidth() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetFrameWidth();
}

bool WindowPropertyFrameAndDivider::setFrameOutsideProjection(double frameOutsideProjection) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setFrameOutsideProjection(frameOutsideProjection);
}

void WindowPropertyFrameAndDivider::resetFrameOutsideProjection() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetFrameOutsideProjection();
}

bool WindowPropertyFrameAndDivider::setFrameInsideProjection(double frameInsideProjection) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setFrameInsideProjection(frameInsideProjection);
}

void WindowPropertyFrameAndDivider::resetFrameInsideProjection() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetFrameInsideProjection();
}

bool WindowPropertyFrameAndDivider::setFrameConductance(double frameConductance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setFrameConductance(frameConductance);
}

void WindowPropertyFrameAndDivider::resetFrameConductance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetFrameConductance();
}

bool WindowPropertyFrameAndDivider::setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance(double ratioOfFrameEdgeGlassConductancetoCenterOfGlassConductance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance(ratioOfFrameEdgeGlassConductancetoCenterOfGlassConductance);
}

void WindowPropertyFrameAndDivider::resetRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance();
}

bool WindowPropertyFrameAndDivider::setFrameSolarAbsorptance(double frameSolarAbsorptance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setFrameSolarAbsorptance(frameSolarAbsorptance);
}

void WindowPropertyFrameAndDivider::resetFrameSolarAbsorptance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetFrameSolarAbsorptance();
}

bool WindowPropertyFrameAndDivider::setFrameVisibleAbsorptance(double frameVisibleAbsorptance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setFrameVisibleAbsorptance(frameVisibleAbsorptance);
}

void WindowPropertyFrameAndDivider::resetFrameVisibleAbsorptance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetFrameVisibleAbsorptance();
}

bool WindowPropertyFrameAndDivider::setFrameThermalHemisphericalEmissivity(double frameThermalHemisphericalEmissivity) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setFrameThermalHemisphericalEmissivity(frameThermalHemisphericalEmissivity);
}

void WindowPropertyFrameAndDivider::resetFrameThermalHemisphericalEmissivity() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetFrameThermalHemisphericalEmissivity();
}

bool WindowPropertyFrameAndDivider::setDividerType(std::string dividerType) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setDividerType(dividerType);
}

void WindowPropertyFrameAndDivider::resetDividerType() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetDividerType();
}

bool WindowPropertyFrameAndDivider::setDividerWidth(double dividerWidth) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setDividerWidth(dividerWidth);
}

void WindowPropertyFrameAndDivider::resetDividerWidth() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetDividerWidth();
}

bool WindowPropertyFrameAndDivider::setNumberOfHorizontalDividers(double numberofHorizontalDividers) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setNumberOfHorizontalDividers(numberofHorizontalDividers);
}

void WindowPropertyFrameAndDivider::resetNumberOfHorizontalDividers() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetNumberOfHorizontalDividers();
}

bool WindowPropertyFrameAndDivider::setNumberOfVerticalDividers(double numberofVerticalDividers) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setNumberOfVerticalDividers(numberofVerticalDividers);
}

void WindowPropertyFrameAndDivider::resetNumberOfVerticalDividers() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetNumberOfVerticalDividers();
}

bool WindowPropertyFrameAndDivider::setDividerOutsideProjection(double dividerOutsideProjection) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setDividerOutsideProjection(dividerOutsideProjection);
}

void WindowPropertyFrameAndDivider::resetDividerOutsideProjection() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetDividerOutsideProjection();
}

bool WindowPropertyFrameAndDivider::setDividerInsideProjection(double dividerInsideProjection) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setDividerInsideProjection(dividerInsideProjection);
}

void WindowPropertyFrameAndDivider::resetDividerInsideProjection() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetDividerInsideProjection();
}

bool WindowPropertyFrameAndDivider::setDividerConductance(double dividerConductance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setDividerConductance(dividerConductance);
}

void WindowPropertyFrameAndDivider::resetDividerConductance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetDividerConductance();
}

bool WindowPropertyFrameAndDivider::setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance(double ratioofDividerEdgeGlassConductancetoCenterOfGlassConductance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance(ratioofDividerEdgeGlassConductancetoCenterOfGlassConductance);
}

void WindowPropertyFrameAndDivider::resetRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance();
}

bool WindowPropertyFrameAndDivider::setDividerSolarAbsorptance(double dividerSolarAbsorptance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setDividerSolarAbsorptance(dividerSolarAbsorptance);
}

void WindowPropertyFrameAndDivider::resetDividerSolarAbsorptance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetDividerSolarAbsorptance();
}

bool WindowPropertyFrameAndDivider::setDividerVisibleAbsorptance(double dividerVisibleAbsorptance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setDividerVisibleAbsorptance(dividerVisibleAbsorptance);
}

void WindowPropertyFrameAndDivider::resetDividerVisibleAbsorptance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetDividerVisibleAbsorptance();
}

bool WindowPropertyFrameAndDivider::setDividerThermalHemisphericalEmissivity(double dividerThermalHemisphericalEmissivity) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setDividerThermalHemisphericalEmissivity(dividerThermalHemisphericalEmissivity);
}

void WindowPropertyFrameAndDivider::resetDividerThermalHemisphericalEmissivity() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetDividerThermalHemisphericalEmissivity();
}

bool WindowPropertyFrameAndDivider::setOutsideRevealDepth(double outsideRevealDepth) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setOutsideRevealDepth(outsideRevealDepth);
}

void WindowPropertyFrameAndDivider::resetOutsideRevealDepth() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetOutsideRevealDepth();
}

bool WindowPropertyFrameAndDivider::setOutsideRevealSolarAbsorptance(double outsideRevealSolarAbsorptance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setOutsideRevealSolarAbsorptance(outsideRevealSolarAbsorptance);
}

void WindowPropertyFrameAndDivider::resetOutsideRevealSolarAbsorptance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetOutsideRevealSolarAbsorptance();
}

bool WindowPropertyFrameAndDivider::setInsideSillDepth(double insideSillDepth) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setInsideSillDepth(insideSillDepth);
}

void WindowPropertyFrameAndDivider::resetInsideSillDepth() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetInsideSillDepth();
}

bool WindowPropertyFrameAndDivider::setInsideSillSolarAbsorptance(double insideSillSolarAbsorptance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setInsideSillSolarAbsorptance(insideSillSolarAbsorptance);
}

void WindowPropertyFrameAndDivider::resetInsideSillSolarAbsorptance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetInsideSillSolarAbsorptance();
}

bool WindowPropertyFrameAndDivider::setInsideRevealDepth(double insideRevealDepth) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setInsideRevealDepth(insideRevealDepth);
}

void WindowPropertyFrameAndDivider::resetInsideRevealDepth() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetInsideRevealDepth();
}

bool WindowPropertyFrameAndDivider::setInsideRevealSolarAbsorptance(double insideRevealSolarAbsorptance) {
  return getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->setInsideRevealSolarAbsorptance(insideRevealSolarAbsorptance);
}

void WindowPropertyFrameAndDivider::resetInsideRevealSolarAbsorptance() {
  getImpl<detail::WindowPropertyFrameAndDivider_Impl>()->resetInsideRevealSolarAbsorptance();
}

/// @cond
WindowPropertyFrameAndDivider::WindowPropertyFrameAndDivider(std::shared_ptr<detail::WindowPropertyFrameAndDivider_Impl> impl)
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

