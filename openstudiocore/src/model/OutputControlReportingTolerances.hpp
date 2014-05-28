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

#ifndef MODEL_OUTPUTCONTROLREPORTINGTOLERANCES_HPP
#define MODEL_OUTPUTCONTROLREPORTINGTOLERANCES_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class OutputControlReportingTolerances_Impl;

} // detail

/** OutputControlReportingTolerances is a ModelObject that wraps the OpenStudio IDD object 'OS:OutputControl:ReportingTolerances'. */
class MODEL_API OutputControlReportingTolerances : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~OutputControlReportingTolerances() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double toleranceforTimeHeatingSetpointNotMet() const;

  Quantity getToleranceforTimeHeatingSetpointNotMet(bool returnIP=false) const;

  bool isToleranceforTimeHeatingSetpointNotMetDefaulted() const;

  double toleranceforTimeCoolingSetpointNotMet() const;

  Quantity getToleranceforTimeCoolingSetpointNotMet(bool returnIP=false) const;

  bool isToleranceforTimeCoolingSetpointNotMetDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setToleranceforTimeHeatingSetpointNotMet(double toleranceforTimeHeatingSetpointNotMet);

  bool setToleranceforTimeHeatingSetpointNotMet(const Quantity& toleranceforTimeHeatingSetpointNotMet);

  void resetToleranceforTimeHeatingSetpointNotMet();

  bool setToleranceforTimeCoolingSetpointNotMet(double toleranceforTimeCoolingSetpointNotMet);

  bool setToleranceforTimeCoolingSetpointNotMet(const Quantity& toleranceforTimeCoolingSetpointNotMet);

  void resetToleranceforTimeCoolingSetpointNotMet();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::OutputControlReportingTolerances_Impl ImplType;

  explicit OutputControlReportingTolerances(std::shared_ptr<detail::OutputControlReportingTolerances_Impl> impl);

  friend class detail::OutputControlReportingTolerances_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit OutputControlReportingTolerances(Model& model);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.OutputControlReportingTolerances");
};

/** \relates OutputControlReportingTolerances*/
typedef boost::optional<OutputControlReportingTolerances> OptionalOutputControlReportingTolerances;

/** \relates OutputControlReportingTolerances*/
typedef std::vector<OutputControlReportingTolerances> OutputControlReportingTolerancesVector;

} // model
} // openstudio

#endif // MODEL_OUTPUTCONTROLREPORTINGTOLERANCES_HPP

