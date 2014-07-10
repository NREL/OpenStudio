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

#ifndef MODEL_REFRIGERATIONCOMPRESSOR_IMPL_HPP
#define MODEL_REFRIGERATIONCOMPRESSOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class CurveBicubic;

namespace detail {

  /** RefrigerationCompressor_Impl is a ParentObject_Impl that is the implementation class for RefrigerationCompressor.*/
  class MODEL_API RefrigerationCompressor_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationCompressor_Impl(const IdfObject& idfObject,
                                 Model_Impl* model,
                                 bool keepHandle);

    RefrigerationCompressor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    RefrigerationCompressor_Impl(const RefrigerationCompressor_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    virtual ~RefrigerationCompressor_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual std::vector<ModelObject> children() const;

    //@}
    /** @name Getters */
    //@{

    CurveBicubic refrigerationCompressorPowerCurve() const;

    CurveBicubic refrigerationCompressorCapacityCurve() const;

    boost::optional<double> ratedSuperheat() const;

    boost::optional<double> ratedReturnGasTemperature() const;

    boost::optional<double> ratedLiquidTemperature() const;

    boost::optional<double> ratedSubcooling() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    std::string modeofOperation() const;

    // bool isModeofOperationDefaulted() const;

    boost::optional<CurveBicubic> transcriticalCompressorPowerCurve() const;

    boost::optional<CurveBicubic> transcriticalCompressorCapacityCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setRefrigerationCompressorPowerCurve(const CurveBicubic& curveBicubic);

    bool setRefrigerationCompressorCapacityCurve(const CurveBicubic& curveBicubic);

    void setRatedSuperheat(boost::optional<double> ratedSuperheat);

    void resetRatedSuperheat();

    void setRatedReturnGasTemperature(boost::optional<double> ratedReturnGasTemperature);

    void resetRatedReturnGasTemperature();

    void setRatedLiquidTemperature(boost::optional<double> ratedLiquidTemperature);

    void resetRatedLiquidTemperature();

    void setRatedSubcooling(boost::optional<double> ratedSubcooling);

    void resetRatedSubcooling();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    // bool setModeofOperation(std::string modeofOperation);

    // void resetModeofOperation();

    bool setTranscriticalCompressorPowerCurve(const boost::optional<CurveBicubic>& curveBicubic);

    void resetTranscriticalCompressorPowerCurve();

    bool setTranscriticalCompressorCapacityCurve(const boost::optional<CurveBicubic>& curveBicubic);

    void resetTranscriticalCompressorCapacityCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCompressor");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<CurveBicubic> optionalRefrigerationCompressorPowerCurve() const;
    boost::optional<CurveBicubic> optionalRefrigerationCompressorCapacityCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCOMPRESSOR_IMPL_HPP

