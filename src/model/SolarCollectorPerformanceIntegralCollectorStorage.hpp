/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~SolarCollectorPerformanceIntegralCollectorStorage() override = default;
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
