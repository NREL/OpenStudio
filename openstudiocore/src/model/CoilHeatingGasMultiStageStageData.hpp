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

#ifndef MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_HPP
#define MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class CoilHeatingGasMultiStageStageData_Impl;

} // detail

/** CoilHeatingGasMultiStageStageData is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:Gas:MultiStage:StageData'. */
class MODEL_API CoilHeatingGasMultiStageStageData : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingGasMultiStageStageData(const Model& model);

  virtual ~CoilHeatingGasMultiStageStageData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double gasBurnerEfficiency() const;

  boost::optional<double> nominalCapacity() const;

  bool isNominalCapacityAutosized() const;

  double parasiticElectricLoad() const;

  //@}
  /** @name Setters */
  //@{

  bool setGasBurnerEfficiency(double GasBurnerEfficiency);

  bool setNominalCapacity(double StageNominalCapacity);

  void autosizeNominalCapacity();

  void setParasiticElectricLoad(double StageParasiticElectricLoad);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingGasMultiStageStageData_Impl ImplType;

  explicit CoilHeatingGasMultiStageStageData(std::shared_ptr<detail::CoilHeatingGasMultiStageStageData_Impl> impl);

  friend class detail::CoilHeatingGasMultiStageStageData_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingGasMultiStageStageData");
};

/** \relates CoilHeatingGasMultiStageStageData*/
typedef boost::optional<CoilHeatingGasMultiStageStageData> OptionalCoilHeatingGasMultiStageStageData;

/** \relates CoilHeatingGasMultiStageStageData*/
typedef std::vector<CoilHeatingGasMultiStageStageData> CoilHeatingGasMultiStageStageDataVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_HPP

