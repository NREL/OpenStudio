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

#ifndef OPENSTUDIO_BUILDINGUNIT_HPP
#define OPENSTUDIO_BUILDINGUNIT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class Space;
class RenderingColor;

namespace detail {

    class BuildingUnit_Impl;

} // detail

class MODEL_API BuildingUnit : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit BuildingUnit(const Model& model);

  virtual ~BuildingUnit() {}

  //@}

  /** @name Static Methods */
  //@{

  // List the possible values for building unit type
  static std::vector<std::string> buildingUnitTypeValues();

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // Gets the rendering color
  boost::optional<RenderingColor> renderingColor() const;

  // Gets the building unit type
  std::string buildingUnitType() const;

  std::vector<Space> spaces() const;

  std::vector<std::string> featureNames() const;

  boost::optional<std::string> getFeatureDataType(const std::string& name) const;

  boost::optional<std::string> getFeatureAsString(const std::string& name) const;

  boost::optional<double> getFeatureAsDouble(const std::string& name) const;

  boost::optional<int> getFeatureAsInteger(const std::string& name) const;

  boost::optional<bool> getFeatureAsBoolean(const std::string& name) const;

  std::vector<std::string> suggestedFeatures() const;

  double floorArea() const;

  //@}
  /** @name Setters */
  //@{

  bool setRenderingColor(const RenderingColor& renderingColor);

  void resetRenderingColor();

  bool setBuildingUnitType(const std::string& buildingUnitType);

  void resetBuildingUnitType();

  bool setFeature(const std::string& name, const std::string& value);

  bool setFeature(const std::string& name, const char* value);

  bool setFeature(const std::string& name, double value);

  bool setFeature(const std::string& name, int value);

  bool setFeature(const std::string& name, bool value);

  bool resetFeature(const std::string& name);

  //@}

 protected:
  /// @cond

  typedef detail::BuildingUnit_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit BuildingUnit(std::shared_ptr<detail::BuildingUnit_Impl> impl);

  /// @endcond

 private:
  REGISTER_LOGGER("openstudio.model.BuildingUnit")

};

} // model

} // openstudio

#endif //OPENSTUDIO_BUILDINGUNIT_HPP
