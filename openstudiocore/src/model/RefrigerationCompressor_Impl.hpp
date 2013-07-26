/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ModelAPI.hpp>
#include <model/ModelObject_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class BicubicCurves;

namespace detail {

  /** RefrigerationCompressor_Impl is a ModelObject_Impl that is the implementation class for RefrigerationCompressor.*/
  class MODEL_API RefrigerationCompressor_Impl : public ModelObject_Impl {
    Q_OBJECT;
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

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: BicubicCurves.
    BicubicCurves refrigerationCompressorPowerCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BicubicCurves.
    BicubicCurves refrigerationCompressorCapacityCurve() const;

    boost::optional<double> ratedSuperheat() const;

    boost::optional<double> ratedReturnGasTemperature() const;

    boost::optional<double> ratedLiquidTemperature() const;

    boost::optional<double> ratedSubcooling() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    std::string modeofOperation() const;

    bool isModeofOperationDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: BicubicCurves.
    boost::optional<BicubicCurves> transcriticalCompressorPowerCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BicubicCurves.
    boost::optional<BicubicCurves> transcriticalCompressorCapacityCurve() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: BicubicCurves.
    bool setRefrigerationCompressorPowerCurve(const BicubicCurves& bicubicCurves);

    // TODO: Check argument type. From object lists, some candidates are: BicubicCurves.
    bool setRefrigerationCompressorCapacityCurve(const BicubicCurves& bicubicCurves);

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

    bool setModeofOperation(std::string modeofOperation);

    void resetModeofOperation();

    // TODO: Check argument type. From object lists, some candidates are: BicubicCurves.
    bool setTranscriticalCompressorPowerCurve(const boost::optional<BicubicCurves>& bicubicCurves);

    void resetTranscriticalCompressorPowerCurve();

    // TODO: Check argument type. From object lists, some candidates are: BicubicCurves.
    bool setTranscriticalCompressorCapacityCurve(const boost::optional<BicubicCurves>& bicubicCurves);

    void resetTranscriticalCompressorCapacityCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCompressor");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<BicubicCurves> optionalRefrigerationCompressorPowerCurve() const;
    boost::optional<BicubicCurves> optionalRefrigerationCompressorCapacityCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCOMPRESSOR_IMPL_HPP

