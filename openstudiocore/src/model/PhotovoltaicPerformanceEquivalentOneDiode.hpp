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

#ifndef MODEL_PHOTOVOLTAICPERFORMANCEEQUIVALENTONEDIODE_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCEEQUIVALENTONEDIODE_HPP

#include "ModelAPI.hpp"
#include "PhotovoltaicPerformance.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class PhotovoltaicPerformanceEquivalentOneDiode_Impl;

} // detail

/** PhotovoltaicPerformanceEquivalentOneDiode is a PhotovoltaicPerformance that wraps the OpenStudio IDD object 'OS:PhotovoltaicPerformance:EquivalentOne-Diode'. */
class MODEL_API PhotovoltaicPerformanceEquivalentOneDiode : public PhotovoltaicPerformance {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PhotovoltaicPerformanceEquivalentOneDiode(const Model& model);

  virtual ~PhotovoltaicPerformanceEquivalentOneDiode() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> celltypeValues();

  /** @name Getters */
  //@{

  std::string celltype() const;

  bool isCelltypeDefaulted() const;


  int numberOfCellsInSeries() const;


  bool isNumberOfCellsInSeriesDefaulted() const;

  double activeArea() const;

  bool isActiveAreaDefaulted() const;

  double transmittanceAbsorptanceProduct() const;

  bool isTransmittanceAbsorptanceProductDefaulted() const;

  double semiconductorBandgap() const;

  bool isSemiconductorBandgapDefaulted() const;

  double shuntResistance() const;

  bool isShuntResistanceDefaulted() const;

  double shortCircuitCurrent() const;

  bool isShortCircuitCurrentDefaulted() const;

  double openCircuitVoltage() const;

  bool isOpenCircuitVoltageDefaulted() const;

  double referenceTemperature() const;

  bool isReferenceTemperatureDefaulted() const;

  double referenceInsolation() const;

  bool isReferenceInsolationDefaulted() const;

  double moduleCurrentatMaximumPower() const;

  bool isModuleCurrentatMaximumPowerDefaulted() const;

  double moduleVoltageatMaximumPower() const;

  bool isModuleVoltageatMaximumPowerDefaulted() const;

  double temperatureCoefficientofShortCircuitCurrent() const;

  bool isTemperatureCoefficientofShortCircuitCurrentDefaulted() const;

  double temperatureCoefficientofOpenCircuitVoltage() const;

  bool isTemperatureCoefficientofOpenCircuitVoltageDefaulted() const;

  double nominalOperatingCellTemperatureTestAmbientTemperature() const;

  bool isNominalOperatingCellTemperatureTestAmbientTemperatureDefaulted() const;

  double nominalOperatingCellTemperatureTestCellTemperature() const;

  bool isNominalOperatingCellTemperatureTestCellTemperatureDefaulted() const;

  double nominalOperatingCellTemperatureTestInsolation() const;

  bool isNominalOperatingCellTemperatureTestInsolationDefaulted() const;

  double moduleHeatLossCoefficient() const;

  bool isModuleHeatLossCoefficientDefaulted() const;

  double totalHeatCapacity() const;

  bool isTotalHeatCapacityDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setCelltype(std::string celltype);

  void resetCelltype();


  bool setNumberofCellsinSeries(int numberOfCellsInSeries);


  void resetNumberofCellsinSeries();

  bool setActiveArea(double activeArea);

  void resetActiveArea();

  bool setTransmittanceAbsorptanceProduct(double transmittanceAbsorptanceProduct);

  void resetTransmittanceAbsorptanceProduct();

  bool setSemiconductorBandgap(double semiconductorBandgap);

  void resetSemiconductorBandgap();

  bool setShuntResistance(double shuntResistance);

  void resetShuntResistance();

  bool setShortCircuitCurrent(double shortCircuitCurrent);

  void resetShortCircuitCurrent();

  bool setOpenCircuitVoltage(double openCircuitVoltage);

  void resetOpenCircuitVoltage();

  bool setReferenceTemperature(double referenceTemperature);

  void resetReferenceTemperature();

  bool setReferenceInsolation(double referenceInsolation);

  void resetReferenceInsolation();

  bool setModuleCurrentatMaximumPower(double moduleCurrentatMaximumPower);

  void resetModuleCurrentatMaximumPower();

  bool setModuleVoltageatMaximumPower(double moduleVoltageatMaximumPower);

  void resetModuleVoltageatMaximumPower();

  void setTemperatureCoefficientofShortCircuitCurrent(double temperatureCoefficientofShortCircuitCurrent);

  void resetTemperatureCoefficientofShortCircuitCurrent();

  void setTemperatureCoefficientofOpenCircuitVoltage(double temperatureCoefficientofOpenCircuitVoltage);

  void resetTemperatureCoefficientofOpenCircuitVoltage();

  bool setNominalOperatingCellTemperatureTestAmbientTemperature(double nominalOperatingCellTemperatureTestAmbientTemperature);

  void resetNominalOperatingCellTemperatureTestAmbientTemperature();

  bool setNominalOperatingCellTemperatureTestCellTemperature(double nominalOperatingCellTemperatureTestCellTemperature);

  void resetNominalOperatingCellTemperatureTestCellTemperature();

  bool setNominalOperatingCellTemperatureTestInsolation(double nominalOperatingCellTemperatureTestInsolation);

  void resetNominalOperatingCellTemperatureTestInsolation();

  bool setModuleHeatLossCoefficient(double moduleHeatLossCoefficient);

  void resetModuleHeatLossCoefficient();

  bool setTotalHeatCapacity(double totalHeatCapacity);

  void resetTotalHeatCapacity();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl ImplType;

  explicit PhotovoltaicPerformanceEquivalentOneDiode(std::shared_ptr<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl> impl);

  friend class detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformanceEquivalentOneDiode");
};

/** \relates PhotovoltaicPerformanceEquivalentOneDiode*/
typedef boost::optional<PhotovoltaicPerformanceEquivalentOneDiode> OptionalPhotovoltaicPerformanceEquivalentOneDiode;

/** \relates PhotovoltaicPerformanceEquivalentOneDiode*/
typedef std::vector<PhotovoltaicPerformanceEquivalentOneDiode> PhotovoltaicPerformanceEquivalentOneDiodeVector;

} // model
} // openstudio

#endif // MODEL_PHOTOVOLTAICPERFORMANCEEQUIVALENTONEDIODE_HPP

