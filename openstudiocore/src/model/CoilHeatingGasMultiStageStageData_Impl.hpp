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

#ifndef MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_IMPL_HPP
#define MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CoilHeatingGasMultiStageStageData_Impl is a ModelObject_Impl that is the implementation class for CoilHeatingGasMultiStageStageData.*/
  class MODEL_API CoilHeatingGasMultiStageStageData_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingGasMultiStageStageData_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle);

    CoilHeatingGasMultiStageStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    CoilHeatingGasMultiStageStageData_Impl(const CoilHeatingGasMultiStageStageData_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    virtual ~CoilHeatingGasMultiStageStageData_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    double gasBurnerEfficiency() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    double parasiticElectricLoad() const;

    //@}
    /** @name Setters */
    //@{

    bool setGasBurnerEfficiency(double StageGasBurnerEfficiency);

    bool setNominalCapacity(boost::optional<double> StageNominalCapacity);

    void autosizeNominalCapacity();

    void setParasiticElectricLoad(double StageParasiticElectricLoad);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingGasMultiStageStageData");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_IMPL_HPP

