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

} // detail

/** SolarCollectorPerformanceIntegralCollectorStorage is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:IntegralCollectorStorage'. */
class MODEL_API SolarCollectorPerformanceIntegralCollectorStorage : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~SolarCollectorPerformanceIntegralCollectorStorage() {}

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
  /// @cond
  typedef detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl ImplType;

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
typedef boost::optional<SolarCollectorPerformanceIntegralCollectorStorage> OptionalSolarCollectorPerformanceIntegralCollectorStorage;

/** \relates SolarCollectorPerformanceIntegralCollectorStorage*/
typedef std::vector<SolarCollectorPerformanceIntegralCollectorStorage> SolarCollectorPerformanceIntegralCollectorStorageVector;

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORPERFORMANCEINTEGRALCOLLECTORSTORAGE_HPP

