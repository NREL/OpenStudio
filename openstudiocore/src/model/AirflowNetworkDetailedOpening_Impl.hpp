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

#ifndef MODEL_AIRFLOWNETWORKDETAILEDOPENING_IMPL_HPP
#define MODEL_AIRFLOWNETWORKDETAILEDOPENING_IMPL_HPP

#include <model/ModelAPI.hpp>

#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

class DetailedOpeningFactorData;

namespace detail {

/** AirflowNetworkDetailedOpening_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkDetailedOpening.*/
class MODEL_API AirflowNetworkDetailedOpening_Impl : public AirflowNetworkComponent_Impl
{
public:
  /** @name Constructors and Destructors */
  //@{

  AirflowNetworkDetailedOpening_Impl(const IdfObject& idfObject,
    Model_Impl* model,
    bool keepHandle);

  AirflowNetworkDetailedOpening_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
    Model_Impl* model,
    bool keepHandle);

  AirflowNetworkDetailedOpening_Impl(const AirflowNetworkDetailedOpening_Impl& other,
    Model_Impl* model,
    bool keepHandle);

  virtual ~AirflowNetworkDetailedOpening_Impl() {}

  //@}
  /** @name Virtual Methods */
  //@{

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  //@}
  /** @name Getters */
  //@{

  double airMassFlowCoefficientWhenOpeningisClosed() const;

  double airMassFlowExponentWhenOpeningisClosed() const;

  bool isAirMassFlowExponentWhenOpeningisClosedDefaulted() const;

  std::string typeofRectangularLargeVerticalOpening() const;

  bool isTypeofRectangularLargeVerticalOpeningDefaulted() const;

  double extraCrackLengthorHeightofPivotingAxis() const;

  bool isExtraCrackLengthorHeightofPivotingAxisDefaulted() const;

  std::vector<DetailedOpeningFactorData> openingFactors() const;

  //@}
  /** @name Setters */
  //@{

  bool setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed);

  bool setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed);

  void resetAirMassFlowExponentWhenOpeningisClosed();

  bool setTypeofRectangularLargeVerticalOpening(std::string typeofRectangularLargeVerticalOpening);

  void resetTypeofRectangularLargeVerticalOpening();

  bool setExtraCrackLengthorHeightofPivotingAxis(double extraCrackLengthorHeightofPivotingAxis);

  void resetExtraCrackLengthorHeightofPivotingAxis();

  bool setOpeningFactors(std::vector<DetailedOpeningFactorData>& factors);

  //@}
protected:
private:
  REGISTER_LOGGER("openstudio.model.AirflowNetworkDetailedOpening");
};

} // detail

} // model
} // openstudio

#endif // MODEL_AIRFLOWNETWORKDETAILEDOPENING_IMPL_HPP

