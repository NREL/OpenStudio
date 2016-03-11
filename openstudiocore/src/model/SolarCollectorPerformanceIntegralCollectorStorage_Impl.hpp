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

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEINTEGRALCOLLECTORSTORAGE_IMPL_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEINTEGRALCOLLECTORSTORAGE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** SolarCollectorPerformanceIntegralCollectorStorage_Impl is a ModelObject_Impl that is the implementation class for SolarCollectorPerformanceIntegralCollectorStorage.*/
  class MODEL_API SolarCollectorPerformanceIntegralCollectorStorage_Impl : public ModelObject_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    SolarCollectorPerformanceIntegralCollectorStorage_Impl(const IdfObject& idfObject,
                                                           Model_Impl* model,
                                                           bool keepHandle);

    SolarCollectorPerformanceIntegralCollectorStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle);

    SolarCollectorPerformanceIntegralCollectorStorage_Impl(const SolarCollectorPerformanceIntegralCollectorStorage_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle);

    virtual ~SolarCollectorPerformanceIntegralCollectorStorage_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
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

    bool setICSCollectorType(std::string iCSCollectorType);

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
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorPerformanceIntegralCollectorStorage");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORPERFORMANCEINTEGRALCOLLECTORSTORAGE_IMPL_HPP

