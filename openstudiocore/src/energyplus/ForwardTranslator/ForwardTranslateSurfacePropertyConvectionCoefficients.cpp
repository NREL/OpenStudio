/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SurfacePropertyConvectionCoefficients.hpp"
#include "../../model/SurfacePropertyConvectionCoefficients_Impl.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/SurfaceProperty_ConvectionCoefficients_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSurfacePropertyConvectionCoefficients(model::SurfacePropertyConvectionCoefficients & modelObject)
{
  boost::optional<ModelObject> convectionCoefficientSurface = modelObject.surfaceAsModelObject();
  boost::optional<std::string> convectionCoefficient1Location = modelObject.convectionCoefficient1Location();
  boost::optional<std::string> convectionCoefficient1Type = modelObject.convectionCoefficient1Type();
  boost::optional<double> convectionCoefficient1 = modelObject.convectionCoefficient1();
  boost::optional<Schedule> convectionCoefficient1Schedule = modelObject.convectionCoefficient1Schedule();
  boost::optional<std::string> convectionCoefficient2Location = modelObject.convectionCoefficient2Location();
  boost::optional<std::string> convectionCoefficient2Type = modelObject.convectionCoefficient2Type();
  boost::optional<double> convectionCoefficient2 = modelObject.convectionCoefficient2();
  boost::optional<Schedule> convectionCoefficient2Schedule = modelObject.convectionCoefficient2Schedule();

  if (!(convectionCoefficientSurface && convectionCoefficient1Location && convectionCoefficient1Type)){
    LOG(Error, "SurfacePropertyConvectionCoefficients '" << modelObject.name().get() << "' missing required fields, will not be translated");
    return boost::none;
  }

  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SurfaceProperty_ConvectionCoefficients,
                                                       modelObject);

  if (convectionCoefficientSurface) {
    boost::optional<IdfObject> surface = translateAndMapModelObject(*convectionCoefficientSurface);
    if (surface && surface->name()) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::SurfaceName, surface->name().get());
    }
  }

  if (convectionCoefficient1Location){
    idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location, *convectionCoefficient1Location);
  }

  if (convectionCoefficient1Type){
    idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type, *convectionCoefficient1Type);
  }

  if (convectionCoefficient1){
    idfObject.setDouble(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1, *convectionCoefficient1);
  }

  if (convectionCoefficient1Schedule){
    boost::optional<IdfObject> sch = translateAndMapModelObject(*convectionCoefficient1Schedule);
    if (sch && sch->name()){
      idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1ScheduleName, sch->name().get());
    }
  }

  if (convectionCoefficient2Location){
    idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location, *convectionCoefficient2Location);
  }

  if (convectionCoefficient2Type){
    idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type, *convectionCoefficient2Type);
  }

  if (convectionCoefficient2){
    idfObject.setDouble(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2, *convectionCoefficient2);
  }

  if (convectionCoefficient2Schedule){
    boost::optional<IdfObject> sch = translateAndMapModelObject(*convectionCoefficient2Schedule);
    if (sch && sch->name()){
      idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2ScheduleName, sch->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

