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

#ifndef MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_IMPL_HPP
#define MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_IMPL_HPP 

#include "ConstructionBase_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {
namespace detail {

  class MODEL_API CFactorUndergroundWallConstruction_Impl : public ConstructionBase_Impl {
    Q_OBJECT;

    Q_PROPERTY(double cFactor READ cFactor WRITE setCFactor);
    Q_PROPERTY(openstudio::Quantity cFactor_SI READ cFactor_SI WRITE setCFactor);
    Q_PROPERTY(openstudio::Quantity cFactor_IP READ cFactor_IP WRITE setCFactor);    

    Q_PROPERTY(double height READ height WRITE setHeight);
    Q_PROPERTY(openstudio::Quantity height_SI READ height_SI WRITE setHeight);
    Q_PROPERTY(openstudio::Quantity height_IP READ height_IP WRITE setHeight);    

   public:

    // constructor
    CFactorUndergroundWallConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    CFactorUndergroundWallConstruction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                            Model_Impl* model, 
                                            bool keepHandle);

    CFactorUndergroundWallConstruction_Impl(const CFactorUndergroundWallConstruction_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    // virtual destructor
    virtual ~CFactorUndergroundWallConstruction_Impl(){}

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

    /// Returns cFactor.
    virtual double cFactor() const;
    virtual Quantity getCFactor(bool returnIP = false) const;
    virtual bool setCFactor(double cFactor);
    virtual bool setCFactor(const Quantity& cFactor);

    /// Returns height.
    virtual double height() const;
    virtual Quantity getHeight(bool returnIP = false) const;
    virtual bool setHeight(double height);
    virtual bool setHeight(const Quantity& height);

    //@}

  private:
    openstudio::Quantity cFactor_SI() const;
    openstudio::Quantity cFactor_IP() const;
    
    openstudio::Quantity height_SI() const;
    openstudio::Quantity height_IP() const;

    REGISTER_LOGGER("openstudio.model.CFactorUndergroundWallConstruction");
  };

} // detail
} // model
} // openstudio

#endif // MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_IMPL_HPP
