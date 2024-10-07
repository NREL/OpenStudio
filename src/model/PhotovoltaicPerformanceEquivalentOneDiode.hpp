/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PHOTOVOLTAICPERFORMANCEEQUIVALENTONEDIODE_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCEEQUIVALENTONEDIODE_HPP

#include "ModelAPI.hpp"
#include "PhotovoltaicPerformance.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class PhotovoltaicPerformanceEquivalentOneDiode_Impl;

  }  // namespace detail

  /** PhotovoltaicPerformanceEquivalentOneDiode is a PhotovoltaicPerformance that wraps the OpenStudio IDD object 'OS:PhotovoltaicPerformance:EquivalentOne-Diode'. */
  class MODEL_API PhotovoltaicPerformanceEquivalentOneDiode : public PhotovoltaicPerformance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PhotovoltaicPerformanceEquivalentOneDiode(const Model& model);

    virtual ~PhotovoltaicPerformanceEquivalentOneDiode() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PhotovoltaicPerformanceEquivalentOneDiode(const PhotovoltaicPerformanceEquivalentOneDiode& other) = default;
    PhotovoltaicPerformanceEquivalentOneDiode(PhotovoltaicPerformanceEquivalentOneDiode&& other) = default;
    PhotovoltaicPerformanceEquivalentOneDiode& operator=(const PhotovoltaicPerformanceEquivalentOneDiode&) = default;
    PhotovoltaicPerformanceEquivalentOneDiode& operator=(PhotovoltaicPerformanceEquivalentOneDiode&&) = default;

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

    bool setCelltype(const std::string& celltype);

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

    bool setTemperatureCoefficientofShortCircuitCurrent(double temperatureCoefficientofShortCircuitCurrent);

    void resetTemperatureCoefficientofShortCircuitCurrent();

    bool setTemperatureCoefficientofOpenCircuitVoltage(double temperatureCoefficientofOpenCircuitVoltage);

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
    using ImplType = detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl;

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
  using OptionalPhotovoltaicPerformanceEquivalentOneDiode = boost::optional<PhotovoltaicPerformanceEquivalentOneDiode>;

  /** \relates PhotovoltaicPerformanceEquivalentOneDiode*/
  using PhotovoltaicPerformanceEquivalentOneDiodeVector = std::vector<PhotovoltaicPerformanceEquivalentOneDiode>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PHOTOVOLTAICPERFORMANCEEQUIVALENTONEDIODE_HPP
