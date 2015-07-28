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

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEINTEGRALCOLLECTORSTORAGE_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEINTEGRALCOLLECTORSTORAGE_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {

namespace model {

namespace detail {

  class SolarCollectorPerformanceIntegralCollectorStorage_Impl;

} // detail

/** SolarCollectorPerformanceIntegralCollectorStorage is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:IntegralCollectorStorage'. */
class MODEL_API SolarCollectorPerformanceIntegralCollectorStorage : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SolarCollectorPerformanceIntegralCollectorStorage(const Model& model);

  virtual ~SolarCollectorPerformanceIntegralCollectorStorage() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> iCSCollectorTypeValues();

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

  bool setGrossArea(double grossArea);

  void resetGrossArea();

  bool setCollectorWaterVolume(double collectorWaterVolume);

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
  /// @cond
  typedef detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl ImplType;

  explicit SolarCollectorPerformanceIntegralCollectorStorage(std::shared_ptr<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl> impl);

  friend class detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl;
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

