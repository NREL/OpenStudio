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

#ifndef MODEL_COILCOOLINGCOOLEDBEAM_IMPL_HPP
#define MODEL_COILCOOLINGCOOLEDBEAM_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

namespace openstudio {
namespace model {

namespace detail {

  /** CoilCoolingCooledBeam_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingCooledBeam.*/
  class MODEL_API CoilCoolingCooledBeam_Impl : public StraightComponent_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingCooledBeam_Impl(const IdfObject& idfObject,
                               Model_Impl* model,
                               bool keepHandle);

    CoilCoolingCooledBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    CoilCoolingCooledBeam_Impl(const CoilCoolingCooledBeam_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    virtual ~CoilCoolingCooledBeam_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const; 

    virtual IddObjectType iddObjectType() const; 

    virtual unsigned inletPort();

    virtual unsigned outletPort();
    
    virtual boost::optional<StraightComponent> containingStraightComponent() const; 

    //@}
    /** @name Getters */
    //@{

    double coilSurfaceAreaperCoilLength() const; 

    bool isCoilSurfaceAreaperCoilLengthDefaulted() const; 

    double modelParametera() const; 

    bool isModelParameteraDefaulted() const; 

    double modelParametern1() const; 

    bool isModelParametern1Defaulted() const;

    double modelParametern2() const;

    bool isModelParametern2Defaulted() const;

    double modelParametern3() const;

    bool isModelParametern3Defaulted() const;

    double modelParametera0() const;

    bool isModelParametera0Defaulted() const;

    double modelParameterK1() const;

    bool isModelParameterK1Defaulted() const;

    double modelParametern() const;

    bool isModelParameternDefaulted() const;

    double leavingPipeInsideDiameter() const;

    bool isLeavingPipeInsideDiameterDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCoilSurfaceAreaperCoilLength(double coilSurfaceAreaperCoilLength); 

    void resetCoilSurfaceAreaperCoilLength(); 

    bool setModelParametera(double modelParametera); 

    void resetModelParametera(); 

    bool setModelParametern1(double modelParametern1);

    void resetModelParametern1();

    bool setModelParametern2(double modelParametern2);

    void resetModelParametern2();

    bool setModelParametern3(double modelParametern3);

    void resetModelParametern3();

    bool setModelParametera0(double modelParametera0);

    void resetModelParametera0();

    bool setModelParameterK1(double modelParameterK1);

    void resetModelParameterK1();

    bool setModelParametern(double modelParametern);

    void resetModelParametern();

    bool setLeavingPipeInsideDiameter(double leavingPipeInsideDiameter);

    void resetLeavingPipeInsideDiameter();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingCooledBeam");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGCOOLEDBEAM_IMPL_HPP

