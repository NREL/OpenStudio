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

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEINTEGRALCOLLECTORSTORAGE_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEINTEGRALCOLLECTORSTORAGE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class SolarCollectorIntegralCollectorStorage;

  namespace detail {

    class SolarCollectorIntegralCollectorStorage_Impl;
    class SolarCollectorPerformanceIntegralCollectorStorage_Impl;

  }  // namespace detail

  /** SolarCollectorPerformanceIntegralCollectorStorage is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:IntegralCollectorStorage'. */
  class MODEL_API SolarCollectorPerformanceIntegralCollectorStorage : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~SolarCollectorPerformanceIntegralCollectorStorage() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorPerformanceIntegralCollectorStorage(const SolarCollectorPerformanceIntegralCollectorStorage& other) = default;
    SolarCollectorPerformanceIntegralCollectorStorage(SolarCollectorPerformanceIntegralCollectorStorage&& other) = default;
    SolarCollectorPerformanceIntegralCollectorStorage& operator=(const SolarCollectorPerformanceIntegralCollectorStorage&) = default;
    SolarCollectorPerformanceIntegralCollectorStorage& operator=(SolarCollectorPerformanceIntegralCollectorStorage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> iCSCollectorTypeValues();

    /** @name Getters */
    //@{

    std::string iCSCollectorType() const;

    bool isICSCollectorTypeDefaulted() const;

    double grossArea() const;

    double collectorWaterVolume() const;

    double bottomHeatLossConductance() const;

    bool isBottomHeatLossConductanceDefaulted() const;

    double sideHeatLossConductance() const;

    bool isSideHeatLossConductanceDefaulted() const;

    double aspectRatio() const;

    bool isAspectRatioDefaulted() const;

    double collectorSideHeight() const;

    bool isCollectorSideHeightDefaulted() const;

    double thermalMassOfAbsorberPlate() const;

    bool isThermalMassOfAbsorberPlateDefaulted() const;

    int numberOfCovers() const;

    bool isNumberOfCoversDefaulted() const;

    double coverSpacing() const;

    bool isCoverSpacingDefaulted() const;

    double refractiveIndexOfOuterCover() const;

    bool isRefractiveIndexOfOuterCoverDefaulted() const;

    double extinctionCoefficientTimesThicknessOfOuterCover() const;

    bool isExtinctionCoefficientTimesThicknessOfOuterCoverDefaulted() const;

    double emissivityOfOuterCover() const;

    bool isEmissivityOfOuterCoverDefaulted() const;

    double refractiveIndexOfInnerCover() const;

    bool isRefractiveIndexOfInnerCoverDefaulted() const;

    double extinctionCoefficientTimesThicknessOfTheInnerCover() const;

    bool isExtinctionCoefficientTimesThicknessOfTheInnerCoverDefaulted() const;

    double emissivityOfInnerCover() const;

    bool isEmissivityOfInnerCoverDefaulted() const;

    double absorptanceOfAbsorberPlate() const;

    bool isAbsorptanceOfAbsorberPlateDefaulted() const;

    double emissivityOfAbsorberPlate() const;

    bool isEmissivityOfAbsorberPlateDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setICSCollectorType(const std::string& iCSCollectorType);

    void resetICSCollectorType();

    bool setGrossArea(double grossArea);

    //void resetGrossArea();

    bool setCollectorWaterVolume(double collectorWaterVolume);

    //void resetCollectorWaterVolume();

    bool setBottomHeatLossConductance(double bottomHeatLossConductance);

    void resetBottomHeatLossConductance();

    bool setSideHeatLossConductance(double sideHeatLossConductance);

    void resetSideHeatLossConductance();

    bool setAspectRatio(double aspectRatio);

    void resetAspectRatio();

    bool setCollectorSideHeight(double collectorSideHeight);

    void resetCollectorSideHeight();

    bool setThermalMassOfAbsorberPlate(double thermalMassOfAbsorberPlate);

    void resetThermalMassOfAbsorberPlate();

    bool setNumberOfCovers(int numberOfCovers);

    void resetNumberOfCovers();

    bool setCoverSpacing(double coverSpacing);

    void resetCoverSpacing();

    bool setRefractiveIndexOfOuterCover(double refractiveIndexOfOuterCover);

    void resetRefractiveIndexOfOuterCover();

    bool setExtinctionCoefficientTimesThicknessOfOuterCover(double extinctionCoefficientTimesThicknessOfOuterCover);

    void resetExtinctionCoefficientTimesThicknessOfOuterCover();

    bool setEmissivityOfOuterCover(double emissivityOfOuterCover);

    void resetEmissivityOfOuterCover();

    bool setRefractiveIndexOfInnerCover(double refractiveIndexOfInnerCover);

    void resetRefractiveIndexOfInnerCover();

    bool setExtinctionCoefficientTimesThicknessOfTheInnerCover(double extinctionCoefficientTimesThicknessOfTheInnerCover);

    void resetExtinctionCoefficientTimesThicknessOfTheInnerCover();

    bool setEmissivityOfInnerCover(double emissivityOfInnerCover);

    void resetEmissivityOfInnerCover();

    bool setAbsorptanceOfAbsorberPlate(double absorptanceOfAbsorberPlate);

    void resetAbsorptanceOfAbsorberPlate();

    bool setEmissivityOfAbsorberPlate(double emissivityOfAbsorberPlate);

    void resetEmissivityOfAbsorberPlate();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl;

    explicit SolarCollectorPerformanceIntegralCollectorStorage(const Model& model);

    explicit SolarCollectorPerformanceIntegralCollectorStorage(std::shared_ptr<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl> impl);

    friend class detail::SolarCollectorIntegralCollectorStorage_Impl;
    friend class detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl;
    friend class SolarCollectorIntegralCollectorStorage;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorPerformanceIntegralCollectorStorage");
  };

  /** \relates SolarCollectorPerformanceIntegralCollectorStorage*/
  using OptionalSolarCollectorPerformanceIntegralCollectorStorage = boost::optional<SolarCollectorPerformanceIntegralCollectorStorage>;

  /** \relates SolarCollectorPerformanceIntegralCollectorStorage*/
  using SolarCollectorPerformanceIntegralCollectorStorageVector = std::vector<SolarCollectorPerformanceIntegralCollectorStorage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORPERFORMANCEINTEGRALCOLLECTORSTORAGE_HPP
