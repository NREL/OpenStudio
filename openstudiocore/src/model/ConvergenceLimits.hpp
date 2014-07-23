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

#ifndef MODEL_CONVERGENCELIMITS_HPP
#define MODEL_CONVERGENCELIMITS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ConvergenceLimits_Impl;

} // detail

/** ConvergenceLimits is a ModelObject that wraps the OpenStudio IDD object 'OS:ConvergenceLimits'. */
class MODEL_API ConvergenceLimits : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ConvergenceLimits() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<int> minimumSystemTimestep() const;

  int maximumHVACIterations() const;

  bool isMaximumHVACIterationsDefaulted() const;

  int minimumPlantIterations() const;

  bool isMinimumPlantIterationsDefaulted() const;

  int maximumPlantIterations() const;

  bool isMaximumPlantIterationsDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setMinimumSystemTimestep(int minimumSystemTimestep);

  void resetMinimumSystemTimestep();

  bool setMaximumHVACIterations(int maximumHVACIterations);

  void resetMaximumHVACIterations();

  bool setMinimumPlantIterations(int minimumPlantIterations);

  void resetMinimumPlantIterations();

  bool setMaximumPlantIterations(int maximumPlantIterations);

  void resetMaximumPlantIterations();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ConvergenceLimits_Impl ImplType;

  explicit ConvergenceLimits(std::shared_ptr<detail::ConvergenceLimits_Impl> impl);

  friend class detail::ConvergenceLimits_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit ConvergenceLimits(Model& model);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ConvergenceLimits");
};

/** \relates ConvergenceLimits */
typedef boost::optional<ConvergenceLimits> OptionalConvergenceLimits;

/** \relates ConvergenceLimits*/
typedef std::vector<ConvergenceLimits> ConvergenceLimitsVector;

} // model
} // openstudio

#endif // MODEL_CONVERGENCELIMITS_HPP
