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

#ifndef MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_IMPL_HPP
#define MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  class MODEL_API EvaporativeCoolerDirectResearchSpecial_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(double coolerEffectiveness READ coolerEffectiveness WRITE setCoolerEffectiveness);
    Q_PROPERTY(double driftLossFraction READ driftLossFraction WRITE setDriftLossFraction);
    Q_PROPERTY(double blowdownConcentrationRatio READ blowdownConcentrationRatio WRITE setBlowdownConcentrationRatio);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    EvaporativeCoolerDirectResearchSpecial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    EvaporativeCoolerDirectResearchSpecial_Impl(
        const openstudio::detail::WorkspaceObject_Impl& other,
        Model_Impl* model,
        bool keepHandle);

    EvaporativeCoolerDirectResearchSpecial_Impl(
        const EvaporativeCoolerDirectResearchSpecial_Impl& other,
        Model_Impl* model,
        bool keepHandle);

    virtual ~EvaporativeCoolerDirectResearchSpecial_Impl(){}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual std::vector<openstudio::IdfObject> remove() override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters and Setters */
    //@{

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    double coolerEffectiveness() const;

    void setCoolerEffectiveness( double value );

    boost::optional<double> recirculatingWaterPumpPowerConsumption() const;

    void setRecirculatingWaterPumpPowerConsumption( double value );

    void autosizeRecirculatingWaterPumpPowerConsumption();

    bool isRecirculatingWaterPumpPowerConsumptionAutosized() const;

    boost::optional<Node> sensorNode() const;

    void setSensorNode( const Node & node );

    double driftLossFraction() const;

    void setDriftLossFraction( double value );

    double blowdownConcentrationRatio() const;

    void setBlowdownConcentrationRatio( double value );

    boost::optional<Curve> effectivenessFlowRatioModifierCurve() const;

    bool setEffectivenessFlowRatioModifierCurve(const Curve& curve);

    void resetEffectivenessFlowRatioModifierCurve();

    double waterPumpPowerSizingFactor() const;

    void setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor);

    boost::optional<Curve> waterPumpPowerModifierCurve() const;

    bool setWaterPumpPowerModifierCurve(const Curve& curve);

    void resetWaterPumpPowerModifierCurve();

    boost::optional<double> primaryAirDesignFlowRate() const;

    void setPrimaryAirDesignFlowRate( double value );

    void autosizePrimaryAirDesignFlowRate();

    bool isPrimaryAirDesignFlowRateAutosized() const;

   private:

    REGISTER_LOGGER("openstudio.model.EvaporativeCoolerDirectResearchSpecial");

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_IMPL_HPP

