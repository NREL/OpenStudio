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

#ifndef MODEL_BUILDINGSTANDARDSINFORMATION_HPP
#define MODEL_BUILDINGSTANDARDSINFORMATION_HPP

#include <model/ModelObject.hpp>

namespace openstudio {
namespace model {

namespace detail {

  class BuildingStandardsInformation_Impl;

} // detail

/** BuildingStandardsInformation is a unique ModelObject that wraps OpenStudio IDD object 
 *  'OS:StandardsInformation:Building'. BuildingStandardsInformation provides a place to register 
 *  information about a building that building energy standards often need to know, but is 
 *  irrelevant to simulation. It contains user-specified values that are not generally checked 
 *  against input or output data. */
class MODEL_API BuildingStandardsInformation : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  /// ETH@20110615 This constructor should be protected, since this is a unique object.
  /** Create new, blank object. */
  explicit BuildingStandardsInformation(Model& model);

  virtual ~BuildingStandardsInformation() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  boost::optional<int> numberOfStories() const;

  boost::optional<int> numberOfAboveGroundStories() const;

  //@}
  /** @name Setters */
  //@{

  bool setNumberOfStories(boost::optional<int> value);

  bool setNumberOfAboveGroundStories(boost::optional<int> value);

  //@}
 protected:
  /// @cond
  typedef detail::BuildingStandardsInformation_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::BuildingStandardsInformation_Impl;

  explicit BuildingStandardsInformation(boost::shared_ptr<detail::BuildingStandardsInformation_Impl> impl);  
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.BuildingStandardsInformation");
};

/** \relates BuildingStandardsInformation */
typedef boost::optional<BuildingStandardsInformation> OptionalBuildingStandardsInformation;

} // model
} // openstudio

#endif // MODEL_BUILDINGSTANDARDSINFORMATION_HPP
