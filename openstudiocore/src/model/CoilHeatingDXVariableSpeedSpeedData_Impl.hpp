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

#ifndef MODEL_COILHEATINGDXVARIABLESPEEDSPEEDDATA_IMPL_HPP
#define MODEL_COILHEATINGDXVARIABLESPEEDSPEEDDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class CurveBiquadratic;
class CurveQuadratic;

namespace detail {

  /** CoilHeatingDXVariableSpeedSpeedData_Impl is a ParentObject_Impl that is the implementation class for CoilHeatingDXVariableSpeedSpeedData.*/
  class MODEL_API CoilHeatingDXVariableSpeedSpeedData_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingDXVariableSpeedSpeedData_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    CoilHeatingDXVariableSpeedSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    CoilHeatingDXVariableSpeedSpeedData_Impl(const CoilHeatingDXVariableSpeedSpeedData_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~CoilHeatingDXVariableSpeedSpeedData_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    double referenceUnitGrossRatedHeatingCapacity() const;

    double referenceUnitGrossRatedHeatingCOP() const;

    double referenceUnitRatedAirFlowRate() const;

    CurveBiquadratic heatingCapacityFunctionofTemperatureCurve() const;

    CurveQuadratic totalHeatingCapacityFunctionofAirFlowFractionCurve() const;

    CurveBiquadratic energyInputRatioFunctionofTemperatureCurve() const;

    CurveQuadratic energyInputRatioFunctionofAirFlowFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity);

    bool setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP);

    bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

    bool setHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    bool setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableSpeedSpeedData");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<CurveBiquadratic> optionalHeatingCapacityFunctionofTemperatureCurve() const;
    boost::optional<CurveQuadratic> optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve() const;
    boost::optional<CurveBiquadratic> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
    boost::optional<CurveQuadratic> optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXVARIABLESPEEDSPEEDDATA_IMPL_HPP

