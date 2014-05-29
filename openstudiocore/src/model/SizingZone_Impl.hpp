/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_SIZINGZONE_IMPL_HPP
#define MODEL_SIZINGZONE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class ThermalZone;

namespace detail {

  /** SizingZone_Impl is a ModelObject_Impl that is the implementation class for SizingZone.*/
  class MODEL_API SizingZone_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(double zoneCoolingDesignSupplyAirTemperature READ zoneCoolingDesignSupplyAirTemperature WRITE setZoneCoolingDesignSupplyAirTemperature);
    Q_PROPERTY(openstudio::Quantity zoneCoolingDesignSupplyAirTemperature_SI READ zoneCoolingDesignSupplyAirTemperature_SI WRITE setZoneCoolingDesignSupplyAirTemperature);
    Q_PROPERTY(openstudio::Quantity zoneCoolingDesignSupplyAirTemperature_IP READ zoneCoolingDesignSupplyAirTemperature_IP WRITE setZoneCoolingDesignSupplyAirTemperature);

    Q_PROPERTY(double zoneHeatingDesignSupplyAirTemperature READ zoneHeatingDesignSupplyAirTemperature WRITE setZoneHeatingDesignSupplyAirTemperature);
    Q_PROPERTY(openstudio::Quantity zoneHeatingDesignSupplyAirTemperature_SI READ zoneHeatingDesignSupplyAirTemperature_SI WRITE setZoneHeatingDesignSupplyAirTemperature);
    Q_PROPERTY(openstudio::Quantity zoneHeatingDesignSupplyAirTemperature_IP READ zoneHeatingDesignSupplyAirTemperature_IP WRITE setZoneHeatingDesignSupplyAirTemperature);

    Q_PROPERTY(double zoneCoolingDesignSupplyAirHumidityRatio READ zoneCoolingDesignSupplyAirHumidityRatio WRITE setZoneCoolingDesignSupplyAirHumidityRatio);
    Q_PROPERTY(openstudio::Quantity zoneCoolingDesignSupplyAirHumidityRatio_SI READ zoneCoolingDesignSupplyAirHumidityRatio_SI WRITE setZoneCoolingDesignSupplyAirHumidityRatio);
    Q_PROPERTY(openstudio::Quantity zoneCoolingDesignSupplyAirHumidityRatio_IP READ zoneCoolingDesignSupplyAirHumidityRatio_IP WRITE setZoneCoolingDesignSupplyAirHumidityRatio);

    Q_PROPERTY(double zoneHeatingDesignSupplyAirHumidityRatio READ zoneHeatingDesignSupplyAirHumidityRatio WRITE setZoneHeatingDesignSupplyAirHumidityRatio);
    Q_PROPERTY(openstudio::Quantity zoneHeatingDesignSupplyAirHumidityRatio_SI READ zoneHeatingDesignSupplyAirHumidityRatio_SI WRITE setZoneHeatingDesignSupplyAirHumidityRatio);
    Q_PROPERTY(openstudio::Quantity zoneHeatingDesignSupplyAirHumidityRatio_IP READ zoneHeatingDesignSupplyAirHumidityRatio_IP WRITE setZoneHeatingDesignSupplyAirHumidityRatio);

    Q_PROPERTY(boost::optional<double> zoneHeatingSizingFactor READ zoneHeatingSizingFactor WRITE setZoneHeatingSizingFactor RESET resetZoneHeatingSizingFactor);
    Q_PROPERTY(openstudio::OSOptionalQuantity zoneHeatingSizingFactor_SI READ zoneHeatingSizingFactor_SI WRITE setZoneHeatingSizingFactor RESET resetZoneHeatingSizingFactor);
    Q_PROPERTY(openstudio::OSOptionalQuantity zoneHeatingSizingFactor_IP READ zoneHeatingSizingFactor_IP WRITE setZoneHeatingSizingFactor RESET resetZoneHeatingSizingFactor);

    Q_PROPERTY(boost::optional<double> zoneCoolingSizingFactor READ zoneCoolingSizingFactor WRITE setZoneCoolingSizingFactor RESET resetZoneCoolingSizingFactor);
    Q_PROPERTY(openstudio::OSOptionalQuantity zoneCoolingSizingFactor_SI READ zoneCoolingSizingFactor_SI WRITE setZoneCoolingSizingFactor RESET resetZoneCoolingSizingFactor);
    Q_PROPERTY(openstudio::OSOptionalQuantity zoneCoolingSizingFactor_IP READ zoneCoolingSizingFactor_IP WRITE setZoneCoolingSizingFactor RESET resetZoneCoolingSizingFactor);

    Q_PROPERTY(std::string coolingDesignAirFlowMethod READ coolingDesignAirFlowMethod WRITE setCoolingDesignAirFlowMethod RESET resetCoolingDesignAirFlowMethod);
    Q_PROPERTY(bool isCoolingDesignAirFlowMethodDefaulted READ isCoolingDesignAirFlowMethodDefaulted);
    Q_PROPERTY(std::vector<std::string> coolingDesignAirFlowMethodValues READ coolingDesignAirFlowMethodValues);

    Q_PROPERTY(double coolingDesignAirFlowRate READ coolingDesignAirFlowRate WRITE setCoolingDesignAirFlowRate RESET resetCoolingDesignAirFlowRate);
    Q_PROPERTY(openstudio::Quantity coolingDesignAirFlowRate_SI READ coolingDesignAirFlowRate_SI WRITE setCoolingDesignAirFlowRate RESET resetCoolingDesignAirFlowRate);
    Q_PROPERTY(openstudio::Quantity coolingDesignAirFlowRate_IP READ coolingDesignAirFlowRate_IP WRITE setCoolingDesignAirFlowRate RESET resetCoolingDesignAirFlowRate);
    Q_PROPERTY(bool isCoolingDesignAirFlowRateDefaulted READ isCoolingDesignAirFlowRateDefaulted);

    Q_PROPERTY(double coolingMinimumAirFlowperZoneFloorArea READ coolingMinimumAirFlowperZoneFloorArea WRITE setCoolingMinimumAirFlowperZoneFloorArea RESET resetCoolingMinimumAirFlowperZoneFloorArea);
    Q_PROPERTY(openstudio::Quantity coolingMinimumAirFlowperZoneFloorArea_SI READ coolingMinimumAirFlowperZoneFloorArea_SI WRITE setCoolingMinimumAirFlowperZoneFloorArea RESET resetCoolingMinimumAirFlowperZoneFloorArea);
    Q_PROPERTY(openstudio::Quantity coolingMinimumAirFlowperZoneFloorArea_IP READ coolingMinimumAirFlowperZoneFloorArea_IP WRITE setCoolingMinimumAirFlowperZoneFloorArea RESET resetCoolingMinimumAirFlowperZoneFloorArea);
    Q_PROPERTY(bool isCoolingMinimumAirFlowperZoneFloorAreaDefaulted READ isCoolingMinimumAirFlowperZoneFloorAreaDefaulted);

    Q_PROPERTY(double coolingMinimumAirFlow READ coolingMinimumAirFlow WRITE setCoolingMinimumAirFlow RESET resetCoolingMinimumAirFlow);
    Q_PROPERTY(openstudio::Quantity coolingMinimumAirFlow_SI READ coolingMinimumAirFlow_SI WRITE setCoolingMinimumAirFlow RESET resetCoolingMinimumAirFlow);
    Q_PROPERTY(openstudio::Quantity coolingMinimumAirFlow_IP READ coolingMinimumAirFlow_IP WRITE setCoolingMinimumAirFlow RESET resetCoolingMinimumAirFlow);
    Q_PROPERTY(bool isCoolingMinimumAirFlowDefaulted READ isCoolingMinimumAirFlowDefaulted);

    Q_PROPERTY(double coolingMinimumAirFlowFraction READ coolingMinimumAirFlowFraction WRITE setCoolingMinimumAirFlowFraction RESET resetCoolingMinimumAirFlowFraction);
    Q_PROPERTY(openstudio::Quantity coolingMinimumAirFlowFraction_SI READ coolingMinimumAirFlowFraction_SI WRITE setCoolingMinimumAirFlowFraction RESET resetCoolingMinimumAirFlowFraction);
    Q_PROPERTY(openstudio::Quantity coolingMinimumAirFlowFraction_IP READ coolingMinimumAirFlowFraction_IP WRITE setCoolingMinimumAirFlowFraction RESET resetCoolingMinimumAirFlowFraction);
    Q_PROPERTY(bool isCoolingMinimumAirFlowFractionDefaulted READ isCoolingMinimumAirFlowFractionDefaulted);

    Q_PROPERTY(std::string heatingDesignAirFlowMethod READ heatingDesignAirFlowMethod WRITE setHeatingDesignAirFlowMethod RESET resetHeatingDesignAirFlowMethod);
    Q_PROPERTY(bool isHeatingDesignAirFlowMethodDefaulted READ isHeatingDesignAirFlowMethodDefaulted);
    Q_PROPERTY(std::vector<std::string> heatingDesignAirFlowMethodValues READ heatingDesignAirFlowMethodValues);

    Q_PROPERTY(double heatingDesignAirFlowRate READ heatingDesignAirFlowRate WRITE setHeatingDesignAirFlowRate RESET resetHeatingDesignAirFlowRate);
    Q_PROPERTY(openstudio::Quantity heatingDesignAirFlowRate_SI READ heatingDesignAirFlowRate_SI WRITE setHeatingDesignAirFlowRate RESET resetHeatingDesignAirFlowRate);
    Q_PROPERTY(openstudio::Quantity heatingDesignAirFlowRate_IP READ heatingDesignAirFlowRate_IP WRITE setHeatingDesignAirFlowRate RESET resetHeatingDesignAirFlowRate);
    Q_PROPERTY(bool isHeatingDesignAirFlowRateDefaulted READ isHeatingDesignAirFlowRateDefaulted);

    Q_PROPERTY(double heatingMaximumAirFlowperZoneFloorArea READ heatingMaximumAirFlowperZoneFloorArea WRITE setHeatingMaximumAirFlowperZoneFloorArea RESET resetHeatingMaximumAirFlowperZoneFloorArea);
    Q_PROPERTY(openstudio::Quantity heatingMaximumAirFlowperZoneFloorArea_SI READ heatingMaximumAirFlowperZoneFloorArea_SI WRITE setHeatingMaximumAirFlowperZoneFloorArea RESET resetHeatingMaximumAirFlowperZoneFloorArea);
    Q_PROPERTY(openstudio::Quantity heatingMaximumAirFlowperZoneFloorArea_IP READ heatingMaximumAirFlowperZoneFloorArea_IP WRITE setHeatingMaximumAirFlowperZoneFloorArea RESET resetHeatingMaximumAirFlowperZoneFloorArea);
    Q_PROPERTY(bool isHeatingMaximumAirFlowperZoneFloorAreaDefaulted READ isHeatingMaximumAirFlowperZoneFloorAreaDefaulted);

    Q_PROPERTY(double heatingMaximumAirFlow READ heatingMaximumAirFlow WRITE setHeatingMaximumAirFlow RESET resetHeatingMaximumAirFlow);
    Q_PROPERTY(openstudio::Quantity heatingMaximumAirFlow_SI READ heatingMaximumAirFlow_SI WRITE setHeatingMaximumAirFlow RESET resetHeatingMaximumAirFlow);
    Q_PROPERTY(openstudio::Quantity heatingMaximumAirFlow_IP READ heatingMaximumAirFlow_IP WRITE setHeatingMaximumAirFlow RESET resetHeatingMaximumAirFlow);
    Q_PROPERTY(bool isHeatingMaximumAirFlowDefaulted READ isHeatingMaximumAirFlowDefaulted);

    Q_PROPERTY(double heatingMaximumAirFlowFraction READ heatingMaximumAirFlowFraction WRITE setHeatingMaximumAirFlowFraction RESET resetHeatingMaximumAirFlowFraction);
    Q_PROPERTY(openstudio::Quantity heatingMaximumAirFlowFraction_SI READ heatingMaximumAirFlowFraction_SI WRITE setHeatingMaximumAirFlowFraction RESET resetHeatingMaximumAirFlowFraction);
    Q_PROPERTY(openstudio::Quantity heatingMaximumAirFlowFraction_IP READ heatingMaximumAirFlowFraction_IP WRITE setHeatingMaximumAirFlowFraction RESET resetHeatingMaximumAirFlowFraction);
    Q_PROPERTY(bool isHeatingMaximumAirFlowFractionDefaulted READ isHeatingMaximumAirFlowFractionDefaulted);

    Q_PROPERTY(double designZoneAirDistributionEffectivenessinCoolingMode READ designZoneAirDistributionEffectivenessinCoolingMode WRITE setDesignZoneAirDistributionEffectivenessinCoolingMode RESET resetDesignZoneAirDistributionEffectivenessinCoolingMode);
    Q_PROPERTY(openstudio::Quantity designZoneAirDistributionEffectivenessinCoolingMode_SI READ designZoneAirDistributionEffectivenessinCoolingMode_SI WRITE setDesignZoneAirDistributionEffectivenessinCoolingMode RESET resetDesignZoneAirDistributionEffectivenessinCoolingMode);
    Q_PROPERTY(openstudio::Quantity designZoneAirDistributionEffectivenessinCoolingMode_IP READ designZoneAirDistributionEffectivenessinCoolingMode_IP WRITE setDesignZoneAirDistributionEffectivenessinCoolingMode RESET resetDesignZoneAirDistributionEffectivenessinCoolingMode);
    Q_PROPERTY(bool isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted READ isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted);

    Q_PROPERTY(double designZoneAirDistributionEffectivenessinHeatingMode READ designZoneAirDistributionEffectivenessinHeatingMode WRITE setDesignZoneAirDistributionEffectivenessinHeatingMode RESET resetDesignZoneAirDistributionEffectivenessinHeatingMode);
    Q_PROPERTY(openstudio::Quantity designZoneAirDistributionEffectivenessinHeatingMode_SI READ designZoneAirDistributionEffectivenessinHeatingMode_SI WRITE setDesignZoneAirDistributionEffectivenessinHeatingMode RESET resetDesignZoneAirDistributionEffectivenessinHeatingMode);
    Q_PROPERTY(openstudio::Quantity designZoneAirDistributionEffectivenessinHeatingMode_IP READ designZoneAirDistributionEffectivenessinHeatingMode_IP WRITE setDesignZoneAirDistributionEffectivenessinHeatingMode RESET resetDesignZoneAirDistributionEffectivenessinHeatingMode);
    Q_PROPERTY(bool isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted READ isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> thermalZone READ thermalZoneAsModelObject WRITE setThermalZoneAsModelObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    SizingZone_Impl(const IdfObject& idfObject,
                    Model_Impl* model,
                    bool keepHandle);

    SizingZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    SizingZone_Impl(const SizingZone_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    virtual ~SizingZone_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: ThermalZone.
    ThermalZone thermalZone() const;

    double zoneCoolingDesignSupplyAirTemperature() const;

    Quantity getZoneCoolingDesignSupplyAirTemperature(bool returnIP=false) const;

    double zoneHeatingDesignSupplyAirTemperature() const;

    Quantity getZoneHeatingDesignSupplyAirTemperature(bool returnIP=false) const;

    double zoneCoolingDesignSupplyAirHumidityRatio() const;

    Quantity getZoneCoolingDesignSupplyAirHumidityRatio(bool returnIP=false) const;

    double zoneHeatingDesignSupplyAirHumidityRatio() const;

    Quantity getZoneHeatingDesignSupplyAirHumidityRatio(bool returnIP=false) const;

    boost::optional<double> zoneHeatingSizingFactor() const;

    OSOptionalQuantity getZoneHeatingSizingFactor(bool returnIP=false) const;

    boost::optional<double> zoneCoolingSizingFactor() const;

    OSOptionalQuantity getZoneCoolingSizingFactor(bool returnIP=false) const;

    std::string coolingDesignAirFlowMethod() const;

    bool isCoolingDesignAirFlowMethodDefaulted() const;

    double coolingDesignAirFlowRate() const;

    Quantity getCoolingDesignAirFlowRate(bool returnIP=false) const;

    bool isCoolingDesignAirFlowRateDefaulted() const;

    double coolingMinimumAirFlowperZoneFloorArea() const;

    Quantity getCoolingMinimumAirFlowperZoneFloorArea(bool returnIP=false) const;

    bool isCoolingMinimumAirFlowperZoneFloorAreaDefaulted() const;

    double coolingMinimumAirFlow() const;

    Quantity getCoolingMinimumAirFlow(bool returnIP=false) const;

    bool isCoolingMinimumAirFlowDefaulted() const;

    double coolingMinimumAirFlowFraction() const;

    Quantity getCoolingMinimumAirFlowFraction(bool returnIP=false) const;

    bool isCoolingMinimumAirFlowFractionDefaulted() const;

    std::string heatingDesignAirFlowMethod() const;

    bool isHeatingDesignAirFlowMethodDefaulted() const;

    double heatingDesignAirFlowRate() const;

    Quantity getHeatingDesignAirFlowRate(bool returnIP=false) const;

    bool isHeatingDesignAirFlowRateDefaulted() const;

    double heatingMaximumAirFlowperZoneFloorArea() const;

    Quantity getHeatingMaximumAirFlowperZoneFloorArea(bool returnIP=false) const;

    bool isHeatingMaximumAirFlowperZoneFloorAreaDefaulted() const;

    double heatingMaximumAirFlow() const;

    Quantity getHeatingMaximumAirFlow(bool returnIP=false) const;

    bool isHeatingMaximumAirFlowDefaulted() const;

    double heatingMaximumAirFlowFraction() const;

    Quantity getHeatingMaximumAirFlowFraction(bool returnIP=false) const;

    bool isHeatingMaximumAirFlowFractionDefaulted() const;

    double designZoneAirDistributionEffectivenessinCoolingMode() const;

    Quantity getDesignZoneAirDistributionEffectivenessinCoolingMode(bool returnIP=false) const;

    bool isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

    double designZoneAirDistributionEffectivenessinHeatingMode() const;

    Quantity getDesignZoneAirDistributionEffectivenessinHeatingMode(bool returnIP=false) const;

    bool isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
    bool setThermalZone(const ThermalZone& thermalZone);

    void setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature);

    bool setZoneCoolingDesignSupplyAirTemperature(const Quantity& zoneCoolingDesignSupplyAirTemperature);

    void setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature);

    bool setZoneHeatingDesignSupplyAirTemperature(const Quantity& zoneHeatingDesignSupplyAirTemperature);

    bool setZoneCoolingDesignSupplyAirHumidityRatio(double zoneCoolingDesignSupplyAirHumidityRatio);

    bool setZoneCoolingDesignSupplyAirHumidityRatio(const Quantity& zoneCoolingDesignSupplyAirHumidityRatio);

    bool setZoneHeatingDesignSupplyAirHumidityRatio(double zoneHeatingDesignSupplyAirHumidityRatio);

    bool setZoneHeatingDesignSupplyAirHumidityRatio(const Quantity& zoneHeatingDesignSupplyAirHumidityRatio);

    bool setZoneHeatingSizingFactor(boost::optional<double> zoneHeatingSizingFactor);

    bool setZoneHeatingSizingFactor(const OSOptionalQuantity& zoneHeatingSizingFactor);

    void resetZoneHeatingSizingFactor();

    bool setZoneCoolingSizingFactor(boost::optional<double> zoneCoolingSizingFactor);

    bool setZoneCoolingSizingFactor(const OSOptionalQuantity& zoneCoolingSizingFactor);

    void resetZoneCoolingSizingFactor();

    bool setCoolingDesignAirFlowMethod(std::string coolingDesignAirFlowMethod);

    void resetCoolingDesignAirFlowMethod();

    bool setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate);

    bool setCoolingDesignAirFlowRate(const Quantity& coolingDesignAirFlowRate);

    void resetCoolingDesignAirFlowRate();

    bool setCoolingMinimumAirFlowperZoneFloorArea(double coolingMinimumAirFlowperZoneFloorArea);

    bool setCoolingMinimumAirFlowperZoneFloorArea(const Quantity& coolingMinimumAirFlowperZoneFloorArea);

    void resetCoolingMinimumAirFlowperZoneFloorArea();

    bool setCoolingMinimumAirFlow(double coolingMinimumAirFlow);

    bool setCoolingMinimumAirFlow(const Quantity& coolingMinimumAirFlow);

    void resetCoolingMinimumAirFlow();

    bool setCoolingMinimumAirFlowFraction(double coolingMinimumAirFlowFraction);

    bool setCoolingMinimumAirFlowFraction(const Quantity& coolingMinimumAirFlowFraction);

    void resetCoolingMinimumAirFlowFraction();

    bool setHeatingDesignAirFlowMethod(std::string heatingDesignAirFlowMethod);

    void resetHeatingDesignAirFlowMethod();

    bool setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate);

    bool setHeatingDesignAirFlowRate(const Quantity& heatingDesignAirFlowRate);

    void resetHeatingDesignAirFlowRate();

    bool setHeatingMaximumAirFlowperZoneFloorArea(double heatingMaximumAirFlowperZoneFloorArea);

    bool setHeatingMaximumAirFlowperZoneFloorArea(const Quantity& heatingMaximumAirFlowperZoneFloorArea);

    void resetHeatingMaximumAirFlowperZoneFloorArea();

    bool setHeatingMaximumAirFlow(double heatingMaximumAirFlow);

    bool setHeatingMaximumAirFlow(const Quantity& heatingMaximumAirFlow);

    void resetHeatingMaximumAirFlow();

    bool setHeatingMaximumAirFlowFraction(double heatingMaximumAirFlowFraction);

    bool setHeatingMaximumAirFlowFraction(const Quantity& heatingMaximumAirFlowFraction);

    void resetHeatingMaximumAirFlowFraction();

    bool setDesignZoneAirDistributionEffectivenessinCoolingMode(double designZoneAirDistributionEffectivenessinCoolingMode);

    bool setDesignZoneAirDistributionEffectivenessinCoolingMode(const Quantity& designZoneAirDistributionEffectivenessinCoolingMode);

    void resetDesignZoneAirDistributionEffectivenessinCoolingMode();

    bool setDesignZoneAirDistributionEffectivenessinHeatingMode(double designZoneAirDistributionEffectivenessinHeatingMode);

    bool setDesignZoneAirDistributionEffectivenessinHeatingMode(const Quantity& designZoneAirDistributionEffectivenessinHeatingMode);

    void resetDesignZoneAirDistributionEffectivenessinHeatingMode();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SizingZone");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<ThermalZone> optionalThermalZone() const;

    openstudio::Quantity zoneCoolingDesignSupplyAirTemperature_SI() const;
    openstudio::Quantity zoneCoolingDesignSupplyAirTemperature_IP() const;
    openstudio::Quantity zoneHeatingDesignSupplyAirTemperature_SI() const;
    openstudio::Quantity zoneHeatingDesignSupplyAirTemperature_IP() const;
    openstudio::Quantity zoneCoolingDesignSupplyAirHumidityRatio_SI() const;
    openstudio::Quantity zoneCoolingDesignSupplyAirHumidityRatio_IP() const;
    openstudio::Quantity zoneHeatingDesignSupplyAirHumidityRatio_SI() const;
    openstudio::Quantity zoneHeatingDesignSupplyAirHumidityRatio_IP() const;
    openstudio::OSOptionalQuantity zoneHeatingSizingFactor_SI() const;
    openstudio::OSOptionalQuantity zoneHeatingSizingFactor_IP() const;
    openstudio::OSOptionalQuantity zoneCoolingSizingFactor_SI() const;
    openstudio::OSOptionalQuantity zoneCoolingSizingFactor_IP() const;
    std::vector<std::string> coolingDesignAirFlowMethodValues() const;
    openstudio::Quantity coolingDesignAirFlowRate_SI() const;
    openstudio::Quantity coolingDesignAirFlowRate_IP() const;
    openstudio::Quantity coolingMinimumAirFlowperZoneFloorArea_SI() const;
    openstudio::Quantity coolingMinimumAirFlowperZoneFloorArea_IP() const;
    openstudio::Quantity coolingMinimumAirFlow_SI() const;
    openstudio::Quantity coolingMinimumAirFlow_IP() const;
    openstudio::Quantity coolingMinimumAirFlowFraction_SI() const;
    openstudio::Quantity coolingMinimumAirFlowFraction_IP() const;
    std::vector<std::string> heatingDesignAirFlowMethodValues() const;
    openstudio::Quantity heatingDesignAirFlowRate_SI() const;
    openstudio::Quantity heatingDesignAirFlowRate_IP() const;
    openstudio::Quantity heatingMaximumAirFlowperZoneFloorArea_SI() const;
    openstudio::Quantity heatingMaximumAirFlowperZoneFloorArea_IP() const;
    openstudio::Quantity heatingMaximumAirFlow_SI() const;
    openstudio::Quantity heatingMaximumAirFlow_IP() const;
    openstudio::Quantity heatingMaximumAirFlowFraction_SI() const;
    openstudio::Quantity heatingMaximumAirFlowFraction_IP() const;
    openstudio::Quantity designZoneAirDistributionEffectivenessinCoolingMode_SI() const;
    openstudio::Quantity designZoneAirDistributionEffectivenessinCoolingMode_IP() const;
    openstudio::Quantity designZoneAirDistributionEffectivenessinHeatingMode_SI() const;
    openstudio::Quantity designZoneAirDistributionEffectivenessinHeatingMode_IP() const;

    boost::optional<ModelObject> thermalZoneAsModelObject() const;

    bool setThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SIZINGZONE_IMPL_HPP

