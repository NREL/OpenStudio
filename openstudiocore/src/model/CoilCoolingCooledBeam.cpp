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

#include <model/CoilCoolingCooledBeam.hpp>
#include <model/CoilCoolingCooledBeam_Impl.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <utilities/units/Unit.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Cooling_CooledBeam_FieldEnums.hxx>
#include <utilities/units/Unit.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingCooledBeam_Impl::CoilCoolingCooledBeam_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == CoilCoolingCooledBeam::iddObjectType());
  }

  CoilCoolingCooledBeam_Impl::CoilCoolingCooledBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == CoilCoolingCooledBeam::iddObjectType());
  }

  CoilCoolingCooledBeam_Impl::CoilCoolingCooledBeam_Impl(const CoilCoolingCooledBeam_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}
  

  const std::vector<std::string>& CoilCoolingCooledBeam_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
    }
    return result;
  }

  IddObjectType CoilCoolingCooledBeam_Impl::iddObjectType() const 
  {
    return CoilCoolingCooledBeam::iddObjectType();
  }
  
  unsigned CoilCoolingCooledBeam_Impl::inletPort()
		{
				return OS_Coil_Cooling_CooledBeamFields::ChilledWaterInletNodeName;
		}

  unsigned CoilCoolingCooledBeam_Impl::outletPort()
		{
				return OS_Coil_Cooling_CooledBeamFields::ChilledWaterOutletNodeName;
		}
		
		boost::optional<StraightComponent> CoilCoolingCooledBeam_Impl::containingStraightComponent() const
  {
    // this coil can only be found in a AirTerminalSingleDuctConstantVolumeCooledBeam
    // check all AirTerminalSingleDuctConstantVolumeCooledBeam in the model, seeing if this coil 
    // is inside of one of them.  Return the one it is inside of
    // declare a vector to hold all of the AirTerminalSingleDuctConstantVolumeCooledBeam

    std::vector<AirTerminalSingleDuctConstantVolumeCooledBeam> airTerminalSingleDuctConstantVolumeCooledBeam;

    airTerminalSingleDuctConstantVolumeCooledBeam = this->model().getModelObjects<AirTerminalSingleDuctConstantVolumeCooledBeam>();

    for( std::vector<AirTerminalSingleDuctConstantVolumeCooledBeam>::iterator it = airTerminalSingleDuctConstantVolumeCooledBeam.begin();
    it < airTerminalSingleDuctConstantVolumeCooledBeam.end();
    it++ )
    {
      if( boost::optional<HVACComponent> coil = it-> coilCoolingCooledBeam() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }
    return boost::none;
  }
  
  double CoilCoolingCooledBeam_Impl::coilSurfaceAreaperCoilLength() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::CoilSurfaceAreaperCoilLength,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isCoilSurfaceAreaperCoilLengthDefaulted() const 
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::CoilSurfaceAreaperCoilLength);
  }

  double CoilCoolingCooledBeam_Impl::modelParametera() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametera,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParameteraDefaulted() const 
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametera);
  }

  double CoilCoolingCooledBeam_Impl::modelParametern1() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern1,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParametern1Defaulted() const 
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametern1);
  }

  double CoilCoolingCooledBeam_Impl::modelParametern2() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern2,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParametern2Defaulted() const 
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametern2);
  }

  double CoilCoolingCooledBeam_Impl::modelParametern3() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern3,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParametern3Defaulted() const 
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametern3);
  }

  double CoilCoolingCooledBeam_Impl::modelParametera0() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametera0,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParametera0Defaulted() const 
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametera0);
  }

  double CoilCoolingCooledBeam_Impl::modelParameterK1() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParameterK1,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParameterK1Defaulted() const 
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParameterK1);
  }

  double CoilCoolingCooledBeam_Impl::modelParametern() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParameternDefaulted() const 
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametern);
  }

  double CoilCoolingCooledBeam_Impl::leavingPipeInsideDiameter() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::LeavingPipeInsideDiameter,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isLeavingPipeInsideDiameterDefaulted() const 
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::LeavingPipeInsideDiameter);
  }

  bool CoilCoolingCooledBeam_Impl::setCoilSurfaceAreaperCoilLength(double coilSurfaceAreaperCoilLength) 
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::CoilSurfaceAreaperCoilLength, coilSurfaceAreaperCoilLength);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetCoilSurfaceAreaperCoilLength() 
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::CoilSurfaceAreaperCoilLength, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametera(double modelParametera) 
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametera, modelParametera);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametera() 
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametera, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametern1(double modelParametern1) 
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern1, modelParametern1);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametern1() {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametern1, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametern2(double modelParametern2) 
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern2, modelParametern2);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametern2() 
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametern2, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametern3(double modelParametern3) 
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern3, modelParametern3);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametern3() 
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametern3, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametera0(double modelParametera0) 
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametera0, modelParametera0);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametera0() 
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametera0, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParameterK1(double modelParameterK1) 
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParameterK1, modelParameterK1);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParameterK1() 
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParameterK1, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametern(double modelParametern) 
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern, modelParametern);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametern() 
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametern, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setLeavingPipeInsideDiameter(double leavingPipeInsideDiameter) 
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::LeavingPipeInsideDiameter, leavingPipeInsideDiameter);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetLeavingPipeInsideDiameter() 
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::LeavingPipeInsideDiameter, "");
    BOOST_ASSERT(result);
  }
    
} // detail

CoilCoolingCooledBeam::CoilCoolingCooledBeam(const Model& model)
  : StraightComponent(CoilCoolingCooledBeam::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::CoilCoolingCooledBeam_Impl>());
}



IddObjectType CoilCoolingCooledBeam::iddObjectType() 
{
  return IddObjectType(IddObjectType::OS_Coil_Cooling_CooledBeam);
}

double CoilCoolingCooledBeam::coilSurfaceAreaperCoilLength() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->coilSurfaceAreaperCoilLength();
}

bool CoilCoolingCooledBeam::isCoilSurfaceAreaperCoilLengthDefaulted() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->isCoilSurfaceAreaperCoilLengthDefaulted();
}

double CoilCoolingCooledBeam::modelParametera() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->modelParametera();
}

bool CoilCoolingCooledBeam::isModelParameteraDefaulted() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->isModelParameteraDefaulted();
}

double CoilCoolingCooledBeam::modelParametern1() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->modelParametern1();
}

bool CoilCoolingCooledBeam::isModelParametern1Defaulted() const {
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->isModelParametern1Defaulted();
}

double CoilCoolingCooledBeam::modelParametern2() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->modelParametern2();
}

bool CoilCoolingCooledBeam::isModelParametern2Defaulted() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->isModelParametern2Defaulted();
}

double CoilCoolingCooledBeam::modelParametern3() const {
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->modelParametern3();
}

bool CoilCoolingCooledBeam::isModelParametern3Defaulted() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->isModelParametern3Defaulted();
}

double CoilCoolingCooledBeam::modelParametera0() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->modelParametera0();
}

bool CoilCoolingCooledBeam::isModelParametera0Defaulted() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->isModelParametera0Defaulted();
}

double CoilCoolingCooledBeam::modelParameterK1() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->modelParameterK1();
}

bool CoilCoolingCooledBeam::isModelParameterK1Defaulted() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->isModelParameterK1Defaulted();
}

double CoilCoolingCooledBeam::modelParametern() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->modelParametern();
}

bool CoilCoolingCooledBeam::isModelParameternDefaulted() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->isModelParameternDefaulted();
}

double CoilCoolingCooledBeam::leavingPipeInsideDiameter() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->leavingPipeInsideDiameter();
}

bool CoilCoolingCooledBeam::isLeavingPipeInsideDiameterDefaulted() const 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->isLeavingPipeInsideDiameterDefaulted();
}

bool CoilCoolingCooledBeam::setCoilSurfaceAreaperCoilLength(double coilSurfaceAreaperCoilLength) 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->setCoilSurfaceAreaperCoilLength(coilSurfaceAreaperCoilLength);
}

void CoilCoolingCooledBeam::resetCoilSurfaceAreaperCoilLength() 
{
  getImpl<detail::CoilCoolingCooledBeam_Impl>()->resetCoilSurfaceAreaperCoilLength();
}

bool CoilCoolingCooledBeam::setModelParametera(double modelParametera) 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->setModelParametera(modelParametera);
}

void CoilCoolingCooledBeam::resetModelParametera() 
{
  getImpl<detail::CoilCoolingCooledBeam_Impl>()->resetModelParametera();
}

bool CoilCoolingCooledBeam::setModelParametern1(double modelParametern1) 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->setModelParametern1(modelParametern1);
}

void CoilCoolingCooledBeam::resetModelParametern1() 
{
  getImpl<detail::CoilCoolingCooledBeam_Impl>()->resetModelParametern1();
}

bool CoilCoolingCooledBeam::setModelParametern2(double modelParametern2) {
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->setModelParametern2(modelParametern2);
}

void CoilCoolingCooledBeam::resetModelParametern2() 
{
  getImpl<detail::CoilCoolingCooledBeam_Impl>()->resetModelParametern2();
}

bool CoilCoolingCooledBeam::setModelParametern3(double modelParametern3) 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->setModelParametern3(modelParametern3);
}

void CoilCoolingCooledBeam::resetModelParametern3() 
{
  getImpl<detail::CoilCoolingCooledBeam_Impl>()->resetModelParametern3();
}

bool CoilCoolingCooledBeam::setModelParametera0(double modelParametera0) 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->setModelParametera0(modelParametera0);
}

void CoilCoolingCooledBeam::resetModelParametera0() 
{
  getImpl<detail::CoilCoolingCooledBeam_Impl>()->resetModelParametera0();
}

bool CoilCoolingCooledBeam::setModelParameterK1(double modelParameterK1) 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->setModelParameterK1(modelParameterK1);
}

void CoilCoolingCooledBeam::resetModelParameterK1() 
{
  getImpl<detail::CoilCoolingCooledBeam_Impl>()->resetModelParameterK1();
}

bool CoilCoolingCooledBeam::setModelParametern(double modelParametern) 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->setModelParametern(modelParametern);
}

void CoilCoolingCooledBeam::resetModelParametern() 
{
  getImpl<detail::CoilCoolingCooledBeam_Impl>()->resetModelParametern();
}

bool CoilCoolingCooledBeam::setLeavingPipeInsideDiameter(double leavingPipeInsideDiameter) 
{
  return getImpl<detail::CoilCoolingCooledBeam_Impl>()->setLeavingPipeInsideDiameter(leavingPipeInsideDiameter);
}

void CoilCoolingCooledBeam::resetLeavingPipeInsideDiameter() 
{
  getImpl<detail::CoilCoolingCooledBeam_Impl>()->resetLeavingPipeInsideDiameter();
}

/// @cond
CoilCoolingCooledBeam::CoilCoolingCooledBeam(boost::shared_ptr<detail::CoilCoolingCooledBeam_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

