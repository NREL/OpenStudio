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

#ifndef MODEL_COILCOOLINGDXVARIABLESPEEDSPEEDDATA_IMPL_HPP
#define MODEL_COILCOOLINGDXVARIABLESPEEDSPEEDDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  /** CoilCoolingDXVariableSpeedSpeedData_Impl is a ParentObject_Impl that is the implementation class for CoilCoolingDXVariableSpeedSpeedData.*/
  class MODEL_API CoilCoolingDXVariableSpeedSpeedData_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingDXVariableSpeedSpeedData_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    CoilCoolingDXVariableSpeedSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    CoilCoolingDXVariableSpeedSpeedData_Impl(const CoilCoolingDXVariableSpeedSpeedData_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~CoilCoolingDXVariableSpeedSpeedData_Impl() {}

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

    double referenceUnitGrossRatedTotalCoolingCapacity() const;

    double referenceUnitGrossRatedSensibleHeatRatio() const;

    double referenceUnitGrossRatedCoolingCOP() const;

    double referenceUnitRatedAirFlowRate() const;

    boost::optional<double> referenceUnitRatedCondenserAirFlowRate() const;

    boost::optional<double> referenceUnitRatedPadEffectivenessofEvapPrecooling() const;

    Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionofAirFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity);

    bool setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio);

    bool setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP);

    bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

    bool setReferenceUnitRatedCondenserAirFlowRate(boost::optional<double> referenceUnitRatedCondenserAirFlowRate);

    void resetReferenceUnitRatedCondenserAirFlowRate();

    bool setReferenceUnitRatedPadEffectivenessofEvapPrecooling(boost::optional<double> referenceUnitRatedPadEffectivenessofEvapPrecooling);

    void resetReferenceUnitRatedPadEffectivenessofEvapPrecooling();

    bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableSpeedSpeedData");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Curve> optionalTotalCoolingCapacityFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXVARIABLESPEEDSPEEDDATA_IMPL_HPP

