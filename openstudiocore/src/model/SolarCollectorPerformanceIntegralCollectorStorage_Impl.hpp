/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include <model/ModelAPI.hpp>
#include <model/ModelObject_Impl.hpp>

namespace openstudio {
namespace model {

namespace detail {

  /** SolarCollectorPerformanceIntegralCollectorStorage_Impl is a ModelObject_Impl that is the implementation class for SolarCollectorPerformanceIntegralCollectorStorage.*/
  class MODEL_API SolarCollectorPerformanceIntegralCollectorStorage_Impl : public ModelObject_Impl {
    Q_OBJECT;
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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string iCSCollectorType() const;

    bool isICSCollectorTypeDefaulted() const;

    boost::optional<double> grossArea() const;

    boost::optional<double> collectorWaterVolume() const;

    double bottomHeatLossConductance() const;

    bool isBottomHeatLossConductanceDefaulted() const;

    double sideHeatLossConductance() const;

    bool isSideHeatLossConductanceDefaulted() const;

    double aspectRatio() const;

    bool isAspectRatioDefaulted() const;

    double collectorSideHeight() const;

    bool isCollectorSideHeightDefaulted() const;

    double thermalMassofAbsorberPlate() const;

    bool isThermalMassofAbsorberPlateDefaulted() const;

    int numberofCovers() const;

    bool isNumberofCoversDefaulted() const;

    double coverSpacing() const;

    bool isCoverSpacingDefaulted() const;

    double refractiveIndexofOuterCover() const;

    bool isRefractiveIndexofOuterCoverDefaulted() const;

    double extinctionCoefficientTimesThicknessofOuterCover() const;

    bool isExtinctionCoefficientTimesThicknessofOuterCoverDefaulted() const;

    double emissivityofOuterCover() const;

    bool isEmissivityofOuterCoverDefaulted() const;

    double refractiveIndexofInnerCover() const;

    bool isRefractiveIndexofInnerCoverDefaulted() const;

    double extinctionCoefficientTimesThicknessoftheinnerCover() const;

    bool isExtinctionCoefficientTimesThicknessoftheinnerCoverDefaulted() const;

    double emissivityofInnerCover() const;

    bool isEmissivityofInnerCoverDefaulted() const;

    double absorptanceofAbsorberPlate() const;

    bool isAbsorptanceofAbsorberPlateDefaulted() const;

    double emissivityofAbsorberPlate() const;

    bool isEmissivityofAbsorberPlateDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setICSCollectorType(std::string iCSCollectorType);

    void resetICSCollectorType();

    bool setGrossArea(boost::optional<double> grossArea);

    void resetGrossArea();

    bool setCollectorWaterVolume(boost::optional<double> collectorWaterVolume);

    void resetCollectorWaterVolume();

    bool setBottomHeatLossConductance(double bottomHeatLossConductance);

    void resetBottomHeatLossConductance();

    bool setSideHeatLossConductance(double sideHeatLossConductance);

    void resetSideHeatLossConductance();

    bool setAspectRatio(double aspectRatio);

    void resetAspectRatio();

    bool setCollectorSideHeight(double collectorSideHeight);

    void resetCollectorSideHeight();

    bool setThermalMassofAbsorberPlate(double thermalMassofAbsorberPlate);

    void resetThermalMassofAbsorberPlate();

    bool setNumberofCovers(int numberofCovers);

    void resetNumberofCovers();

    bool setCoverSpacing(double coverSpacing);

    void resetCoverSpacing();

    bool setRefractiveIndexofOuterCover(double refractiveIndexofOuterCover);

    void resetRefractiveIndexofOuterCover();

    bool setExtinctionCoefficientTimesThicknessofOuterCover(double extinctionCoefficientTimesThicknessofOuterCover);

    void resetExtinctionCoefficientTimesThicknessofOuterCover();

    bool setEmissivityofOuterCover(double emissivityofOuterCover);

    void resetEmissivityofOuterCover();

    bool setRefractiveIndexofInnerCover(double refractiveIndexofInnerCover);

    void resetRefractiveIndexofInnerCover();

    bool setExtinctionCoefficientTimesThicknessoftheinnerCover(double extinctionCoefficientTimesThicknessoftheinnerCover);

    void resetExtinctionCoefficientTimesThicknessoftheinnerCover();

    bool setEmissivityofInnerCover(double emissivityofInnerCover);

    void resetEmissivityofInnerCover();

    bool setAbsorptanceofAbsorberPlate(double absorptanceofAbsorberPlate);

    void resetAbsorptanceofAbsorberPlate();

    bool setEmissivityofAbsorberPlate(double emissivityofAbsorberPlate);

    void resetEmissivityofAbsorberPlate();

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

