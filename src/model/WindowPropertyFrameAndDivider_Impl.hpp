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

#ifndef MODEL_WINDOWPROPERTYFRAMEANDDIVIDER_IMPL_HPP
#define MODEL_WINDOWPROPERTYFRAMEANDDIVIDER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** WindowPropertyFrameAndDivider_Impl is a ResourceObject_Impl that is the implementation class for WindowPropertyFrameAndDivider.*/
    class MODEL_API WindowPropertyFrameAndDivider_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      WindowPropertyFrameAndDivider_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WindowPropertyFrameAndDivider_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WindowPropertyFrameAndDivider_Impl(const WindowPropertyFrameAndDivider_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~WindowPropertyFrameAndDivider_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double frameWidth() const;

      bool isFrameWidthDefaulted() const;

      double frameOutsideProjection() const;

      bool isFrameOutsideProjectionDefaulted() const;

      double frameInsideProjection() const;

      bool isFrameInsideProjectionDefaulted() const;

      double frameConductance() const;

      bool isFrameConductanceDefaulted() const;

      double ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance() const;

      bool isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const;

      double frameSolarAbsorptance() const;

      bool isFrameSolarAbsorptanceDefaulted() const;

      double frameVisibleAbsorptance() const;

      bool isFrameVisibleAbsorptanceDefaulted() const;

      double frameThermalHemisphericalEmissivity() const;

      bool isFrameThermalHemisphericalEmissivityDefaulted() const;

      std::string dividerType() const;

      bool isDividerTypeDefaulted() const;

      double dividerWidth() const;

      bool isDividerWidthDefaulted() const;

      int numberOfHorizontalDividers() const;

      bool isNumberOfHorizontalDividersDefaulted() const;

      int numberOfVerticalDividers() const;

      bool isNumberOfVerticalDividersDefaulted() const;

      double dividerOutsideProjection() const;

      bool isDividerOutsideProjectionDefaulted() const;

      double dividerInsideProjection() const;

      bool isDividerInsideProjectionDefaulted() const;

      double dividerConductance() const;

      bool isDividerConductanceDefaulted() const;

      double ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance() const;

      bool isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const;

      double dividerSolarAbsorptance() const;

      bool isDividerSolarAbsorptanceDefaulted() const;

      double dividerVisibleAbsorptance() const;

      bool isDividerVisibleAbsorptanceDefaulted() const;

      double dividerThermalHemisphericalEmissivity() const;

      bool isDividerThermalHemisphericalEmissivityDefaulted() const;

      double outsideRevealDepth() const;

      bool isOutsideRevealDepthDefaulted() const;

      double outsideRevealSolarAbsorptance() const;

      bool isOutsideRevealSolarAbsorptanceDefaulted() const;

      double insideSillDepth() const;

      bool isInsideSillDepthDefaulted() const;

      double insideSillSolarAbsorptance() const;

      bool isInsideSillSolarAbsorptanceDefaulted() const;

      double insideRevealDepth() const;

      bool isInsideRevealDepthDefaulted() const;

      double insideRevealSolarAbsorptance() const;

      bool isInsideRevealSolarAbsorptanceDefaulted() const;

      std::string nfrcProductTypeforAssemblyCalculations() const;

      bool isNFRCProductTypeforAssemblyCalculationsDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setFrameWidth(double frameWidth);

      void resetFrameWidth();

      bool setFrameOutsideProjection(double frameOutsideProjection);

      void resetFrameOutsideProjection();

      bool setFrameInsideProjection(double frameInsideProjection);

      void resetFrameInsideProjection();

      bool setFrameConductance(boost::optional<double> frameConductance);

      void resetFrameConductance();

      bool setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance(double ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance);

      void resetRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance();

      bool setFrameSolarAbsorptance(double frameSolarAbsorptance);

      void resetFrameSolarAbsorptance();

      bool setFrameVisibleAbsorptance(double frameVisibleAbsorptance);

      void resetFrameVisibleAbsorptance();

      bool setFrameThermalHemisphericalEmissivity(double frameThermalHemisphericalEmissivity);

      void resetFrameThermalHemisphericalEmissivity();

      bool setDividerType(const std::string& dividerType);

      void resetDividerType();

      bool setDividerWidth(double dividerWidth);

      void resetDividerWidth();

      bool setNumberOfHorizontalDividers(int numberOfHorizontalDividers);

      void resetNumberOfHorizontalDividers();

      bool setNumberOfVerticalDividers(int numberOfVerticalDividers);

      void resetNumberOfVerticalDividers();

      bool setDividerOutsideProjection(double dividerOutsideProjection);

      void resetDividerOutsideProjection();

      bool setDividerInsideProjection(double dividerInsideProjection);

      void resetDividerInsideProjection();

      bool setDividerConductance(double dividerConductance);

      void resetDividerConductance();

      bool setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance(double ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance);

      void resetRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance();

      bool setDividerSolarAbsorptance(double dividerSolarAbsorptance);

      void resetDividerSolarAbsorptance();

      bool setDividerVisibleAbsorptance(double dividerVisibleAbsorptance);

      void resetDividerVisibleAbsorptance();

      bool setDividerThermalHemisphericalEmissivity(double dividerThermalHemisphericalEmissivity);

      void resetDividerThermalHemisphericalEmissivity();

      bool setOutsideRevealDepth(double outsideRevealDepth);

      void resetOutsideRevealDepth();

      bool setOutsideRevealSolarAbsorptance(double outsideRevealSolarAbsorptance);

      void resetOutsideRevealSolarAbsorptance();

      bool setInsideSillDepth(double insideSillDepth);

      void resetInsideSillDepth();

      bool setInsideSillSolarAbsorptance(double insideSillSolarAbsorptance);

      void resetInsideSillSolarAbsorptance();

      bool setInsideRevealDepth(double insideRevealDepth);

      void resetInsideRevealDepth();

      bool setInsideRevealSolarAbsorptance(double insideRevealSolarAbsorptance);

      void resetInsideRevealSolarAbsorptance();

      bool setNFRCProductTypeforAssemblyCalculations(const std::string& nfrcProductTypeforAssemblyCalculations);

      void resetNFRCProductTypeforAssemblyCalculations();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.WindowPropertyFrameAndDivider");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WINDOWPROPERTYFRAMEANDDIVIDER_IMPL_HPP
