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

#ifndef MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP
#define MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  /** CoilHeatingDXMultiSpeedStageData_Impl is a ParentObject_Impl that is the implementation class for CoilHeatingDXMultiSpeedStageData.*/
  class MODEL_API CoilHeatingDXMultiSpeedStageData_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingDXMultiSpeedStageData_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingDXMultiSpeedStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingDXMultiSpeedStageData_Impl(const CoilHeatingDXMultiSpeedStageData_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~CoilHeatingDXMultiSpeedStageData_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<ModelObject> children() const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> grossRatedHeatingCapacity() const;

    bool isGrossRatedHeatingCapacityAutosized() const;

    double grossRatedHeatingCOP() const;

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    double ratedSupplyAirFanPowerPerVolumeFlowRate() const;

    Curve heatingCapacityFunctionofTemperatureCurve() const;

    Curve heatingCapacityFunctionofFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    double ratedWasteHeatFractionofPowerInput() const;

    Curve wasteHeatFunctionofTemperatureCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setGrossRatedHeatingCapacity(boost::optional<double> grossRatedHeatingCapacity);

    void autosizeGrossRatedHeatingCapacity();

    bool setGrossRatedHeatingCOP(double grossRatedHeatingCOP);

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    bool setRatedSupplyAirFanPowerPerVolumeFlowRate(double ratedSupplyAirFanPowerPerVolumeFlowRate);

    bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

    bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeedStageData");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Curve> optionalHeatingCapacityFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalHeatingCapacityFunctionofFlowFractionCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionofFlowFractionCurve() const;
    boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
    boost::optional<Curve> optionalWasteHeatFunctionofTemperatureCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP

