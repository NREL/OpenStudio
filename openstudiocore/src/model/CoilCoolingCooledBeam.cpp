/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "CoilCoolingCooledBeam.hpp"
#include "CoilCoolingCooledBeam_Impl.hpp"
#include "AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Coil_Cooling_CooledBeam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingCooledBeam_Impl::CoilCoolingCooledBeam_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingCooledBeam::iddObjectType());
  }

  CoilCoolingCooledBeam_Impl::CoilCoolingCooledBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingCooledBeam::iddObjectType());
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

    airTerminalSingleDuctConstantVolumeCooledBeam = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeCooledBeam>();

    for( const auto & elem : airTerminalSingleDuctConstantVolumeCooledBeam )
    {
      if( boost::optional<HVACComponent> coil = elem.coilCoolingCooledBeam() )
      {
        if( coil->handle() == this->handle() )
        {
          return elem;
        }
      }
    }
    return boost::none;
  }

  bool CoilCoolingCooledBeam_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->demandComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

  double CoilCoolingCooledBeam_Impl::coilSurfaceAreaperCoilLength() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::CoilSurfaceAreaperCoilLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isCoilSurfaceAreaperCoilLengthDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::CoilSurfaceAreaperCoilLength);
  }

  double CoilCoolingCooledBeam_Impl::modelParametera() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametera,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParameteraDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametera);
  }

  double CoilCoolingCooledBeam_Impl::modelParametern1() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern1,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParametern1Defaulted() const
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametern1);
  }

  double CoilCoolingCooledBeam_Impl::modelParametern2() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern2,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParametern2Defaulted() const
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametern2);
  }

  double CoilCoolingCooledBeam_Impl::modelParametern3() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern3,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParametern3Defaulted() const
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametern3);
  }

  double CoilCoolingCooledBeam_Impl::modelParametera0() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametera0,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParametera0Defaulted() const
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametera0);
  }

  double CoilCoolingCooledBeam_Impl::modelParameterK1() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParameterK1,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParameterK1Defaulted() const
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParameterK1);
  }

  double CoilCoolingCooledBeam_Impl::modelParametern() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingCooledBeam_Impl::isModelParameternDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_CooledBeamFields::ModelParametern);
  }

  double CoilCoolingCooledBeam_Impl::leavingPipeInsideDiameter() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_CooledBeamFields::LeavingPipeInsideDiameter,true);
    OS_ASSERT(value);
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
    OS_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametera(double modelParametera)
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametera, modelParametera);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametera()
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametera, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametern1(double modelParametern1)
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern1, modelParametern1);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametern1() {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametern1, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametern2(double modelParametern2)
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern2, modelParametern2);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametern2()
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametern2, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametern3(double modelParametern3)
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern3, modelParametern3);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametern3()
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametern3, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametera0(double modelParametera0)
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametera0, modelParametera0);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametera0()
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametera0, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParameterK1(double modelParameterK1)
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParameterK1, modelParameterK1);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParameterK1()
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParameterK1, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setModelParametern(double modelParametern)
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::ModelParametern, modelParametern);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetModelParametern()
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::ModelParametern, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingCooledBeam_Impl::setLeavingPipeInsideDiameter(double leavingPipeInsideDiameter)
  {
    bool result = setDouble(OS_Coil_Cooling_CooledBeamFields::LeavingPipeInsideDiameter, leavingPipeInsideDiameter);
    return result;
  }

  void CoilCoolingCooledBeam_Impl::resetLeavingPipeInsideDiameter()
  {
    bool result = setString(OS_Coil_Cooling_CooledBeamFields::LeavingPipeInsideDiameter, "");
    OS_ASSERT(result);
  }

} // detail

CoilCoolingCooledBeam::CoilCoolingCooledBeam(const Model& model)
  : StraightComponent(CoilCoolingCooledBeam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingCooledBeam_Impl>());
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
CoilCoolingCooledBeam::CoilCoolingCooledBeam(std::shared_ptr<detail::CoilCoolingCooledBeam_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

