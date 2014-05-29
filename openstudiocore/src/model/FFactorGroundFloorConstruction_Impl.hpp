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

#ifndef MODEL_FFACTORGROUNDFLOORCONSTRUCTION_IMPL_HPP
#define MODEL_FFACTORGROUNDFLOORCONSTRUCTION_IMPL_HPP 

#include "ConstructionBase_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {
namespace detail {

  class MODEL_API FFactorGroundFloorConstruction_Impl : public ConstructionBase_Impl 
  {
    Q_OBJECT;

    Q_PROPERTY(double fFactor READ fFactor WRITE setFFactor);
    Q_PROPERTY(openstudio::Quantity fFactor_SI READ fFactor_SI WRITE setFFactor);
    Q_PROPERTY(openstudio::Quantity fFactor_IP READ fFactor_IP WRITE setFFactor);    

    Q_PROPERTY(double area READ area WRITE setArea);
    Q_PROPERTY(openstudio::Quantity area_SI READ area_SI WRITE setArea);
    Q_PROPERTY(openstudio::Quantity area_IP READ area_IP WRITE setArea);    

    Q_PROPERTY(double perimeterExposed READ perimeterExposed WRITE setPerimeterExposed);
    Q_PROPERTY(openstudio::Quantity perimeterExposed_SI READ perimeterExposed_SI WRITE setPerimeterExposed);
    Q_PROPERTY(openstudio::Quantity perimeterExposed_IP READ perimeterExposed_IP WRITE setPerimeterExposed);    

  public:

    // constructor
    FFactorGroundFloorConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    FFactorGroundFloorConstruction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                        Model_Impl* model, 
                                        bool keepHandle);

    FFactorGroundFloorConstruction_Impl(const FFactorGroundFloorConstruction_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    // virtual destructor
    virtual ~FFactorGroundFloorConstruction_Impl(){}

    /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Queries */
    //@{

    virtual bool isOpaque() const;

    virtual bool isFenestration() const;

    virtual bool isSolarDiffusing() const;

    virtual bool isModelPartition() const;

    virtual int renderingColorIndex() const;

    /// Returns fFactor.
    virtual double fFactor() const;
    virtual Quantity getFFactor(bool returnIP = false) const;
    virtual bool setFFactor(double fFactor);
    virtual bool setFFactor(const Quantity& fFactor);

    /// Returns area.
    virtual double area() const;
    virtual Quantity getArea(bool returnIP = false) const;
    virtual bool setArea(double area);
    virtual bool setArea(const Quantity& area);

    /// Returns perimeterExposed.
    virtual double perimeterExposed() const;
    virtual Quantity getPerimeterExposed(bool returnIP = false) const;
    virtual bool setPerimeterExposed(double perimeterExposed);
    virtual bool setPerimeterExposed(const Quantity& perimeterExposed);

    //@}

  private:
    openstudio::Quantity fFactor_SI() const;
    openstudio::Quantity fFactor_IP() const;

    openstudio::Quantity area_SI() const;
    openstudio::Quantity area_IP() const;

    openstudio::Quantity perimeterExposed_SI() const;
    openstudio::Quantity perimeterExposed_IP() const;

    REGISTER_LOGGER("openstudio.model.FFactorGroundFloorConstruction");
  };

} // detail
} // model
} // openstudio

#endif // MODEL_FFACTORGROUNDFLOORCONSTRUCTION_IMPL_HPP
