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

#ifndef MODEL_SIZINGPLANT_HPP
#define MODEL_SIZINGPLANT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class PlantLoop;

namespace detail {

  class SizingPlant_Impl;

} // detail

/** SizingPlant is a ModelObject that wraps the OpenStudio IDD object 'OS:Sizing:Plant'. */
class MODEL_API SizingPlant : public ModelObject {
  public:

  explicit SizingPlant(const Model& model, const PlantLoop & plantLoop);

  virtual ~SizingPlant() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> loopTypeValues();


  PlantLoop plantLoop() const;

  bool setPlantLoop(const PlantLoop& plantLoop);


  std::string loopType() const;

  bool setLoopType(std::string loopType);


  double designLoopExitTemperature() const;

  Quantity getDesignLoopExitTemperature(bool returnIP=false) const;

  void setDesignLoopExitTemperature(double designLoopExitTemperature);

  bool setDesignLoopExitTemperature(const Quantity& designLoopExitTemperature);


  double loopDesignTemperatureDifference() const;

  Quantity getLoopDesignTemperatureDifference(bool returnIP=false) const;

  bool setLoopDesignTemperatureDifference(double loopDesignTemperatureDifference);

  bool setLoopDesignTemperatureDifference(const Quantity& loopDesignTemperatureDifference);

  protected:

  /// @cond
  typedef detail::SizingPlant_Impl ImplType;

  explicit SizingPlant(std::shared_ptr<detail::SizingPlant_Impl> impl);

  friend class detail::SizingPlant_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.SizingPlant");
};

/** \relates SizingPlant*/
typedef boost::optional<SizingPlant> OptionalSizingPlant;

/** \relates SizingPlant*/
typedef std::vector<SizingPlant> SizingPlantVector;

} // model
} // openstudio

#endif // MODEL_SIZINGPLANT_HPP

