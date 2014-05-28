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

#ifndef MODEL_SIZINGPARAMETERS_HPP
#define MODEL_SIZINGPARAMETERS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class SizingParameters_Impl;

} // detail

/** SizingParameters is a ModelObject that wraps the OpenStudio IDD object 'OS:Sizing:Parameters'. */
class MODEL_API SizingParameters : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  virtual ~SizingParameters() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  double heatingSizingFactor() const;

  Quantity getHeatingSizingFactor(bool returnIP=false) const;

  bool isHeatingSizingFactorDefaulted() const;

  double coolingSizingFactor() const;

  Quantity getCoolingSizingFactor(bool returnIP=false) const;

  bool isCoolingSizingFactorDefaulted() const;

  boost::optional<int> timestepsinAveragingWindow() const;

  //@}
  /** @name Setters */
  //@{

  bool setHeatingSizingFactor(double heatingSizingFactor);

  bool setHeatingSizingFactor(const Quantity& heatingSizingFactor);

  void resetHeatingSizingFactor();

  bool setCoolingSizingFactor(double coolingSizingFactor);

  bool setCoolingSizingFactor(const Quantity& coolingSizingFactor);

  void resetCoolingSizingFactor();

  bool setTimestepsinAveragingWindow(int timestepsinAveragingWindow);

  void resetTimestepsinAveragingWindow();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SizingParameters_Impl ImplType;

  explicit SizingParameters(std::shared_ptr<detail::SizingParameters_Impl> impl);

  friend class detail::SizingParameters_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit SizingParameters(Model& model);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SizingParameters");
};

/** \relates SizingParameters*/
typedef boost::optional<SizingParameters> OptionalSizingParameters;

/** \relates SizingParameters*/
typedef std::vector<SizingParameters> SizingParametersVector;

} // model
} // openstudio

#endif // MODEL_SIZINGPARAMETERS_HPP

