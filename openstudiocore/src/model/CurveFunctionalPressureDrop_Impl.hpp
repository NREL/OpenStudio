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

#ifndef MODEL_CURVEFUNCTIONALPRESSUREDROP_IMPL_HPP
#define MODEL_CURVEFUNCTIONALPRESSUREDROP_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveFunctionalPressureDrop_Impl is a Curve_Impl that is the implementation class for CurveFunctionalPressureDrop.*/
  class MODEL_API CurveFunctionalPressureDrop_Impl : public Curve_Impl {
    Q_OBJECT;
    Q_PROPERTY(double diameter READ diameter WRITE setDiameter);
    Q_PROPERTY(boost::optional<double> minorLossCoefficient READ minorLossCoefficient WRITE setMinorLossCoefficient RESET resetMinorLossCoefficient);
    Q_PROPERTY(boost::optional<double> length READ length WRITE setLength RESET resetLength);
    Q_PROPERTY(boost::optional<double> roughness READ roughness WRITE setRoughness RESET resetRoughness);
    Q_PROPERTY(boost::optional<double> fixedFrictionFactor READ fixedFrictionFactor WRITE setFixedFrictionFactor RESET resetFixedFrictionFactor);
   public:

    /** @name Constructors and Destructors */
    //@{

    CurveFunctionalPressureDrop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CurveFunctionalPressureDrop_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    CurveFunctionalPressureDrop_Impl(const CurveFunctionalPressureDrop_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    virtual ~CurveFunctionalPressureDrop_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual int numVariables() const;

    virtual double evaluate(const std::vector<double>& x) const;

    //@}
    /** @name Getters */
    //@{

    double diameter() const;

    boost::optional<double> minorLossCoefficient() const;

    boost::optional<double> length() const;

    boost::optional<double> roughness() const;

    boost::optional<double> fixedFrictionFactor() const;

    //@}
    /** @name Setters */
    //@{

    void setDiameter(double diameter);

    void setMinorLossCoefficient(boost::optional<double> minorLossCoefficient);

    void resetMinorLossCoefficient();

    void setLength(boost::optional<double> length);

    void resetLength();

    void setRoughness(boost::optional<double> roughness);

    void resetRoughness();

    void setFixedFrictionFactor(boost::optional<double> fixedFrictionFactor);

    void resetFixedFrictionFactor();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CurveFunctionalPressureDrop");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVEFUNCTIONALPRESSUREDROP_IMPL_HPP

