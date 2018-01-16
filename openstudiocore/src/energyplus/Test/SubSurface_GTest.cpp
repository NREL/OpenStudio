/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"

#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ReverseTranslator_GlassDoorToSubSurface) {

  std::string text = "\
  FenestrationSurface:Detailed,\n\
    Perimeter_ZN_1_wall_south_door,  !- Name\n\
    GlassDoor,               !- Surface Type\n\
    window_south,            !- Construction Name\n\
    Perimeter_ZN_1_wall_south,  !- Building Surface Name\n\
    ,                        !- Outside Boundary Condition Object\n\
    AutoCalculate,           !- View Factor to Ground\n\
    ,                        !- Shading Control Name\n\
    ,                        !- Frame and Divider Name\n\
    1.0000,                  !- Multiplier\n\
    4,                       !- Number of Vertices\n\
    12.930,0.0000,2.1340,  !- X,Y,Z ==> Vertex 1 {m}\n\
    12.930,0.0000,0.0000,  !- X,Y,Z ==> Vertex 2 {m}\n\
    14.760,0.0000,0.0000,  !- X,Y,Z ==> Vertex 3 {m}\n\
    14.760,0.0000,2.1340;  !- X,Y,Z ==> Vertex 4 {m}";

  IdfObject idfObject = IdfObject::load(text).get();
  Workspace ws(StrictnessLevel::Draft,IddFileType::EnergyPlus);
  OptionalWorkspaceObject owo = ws.addObject(idfObject);
  ASSERT_TRUE(owo);

  ReverseTranslator rt;
  Model model = rt.translateWorkspace(ws);
  SubSurfaceVector subSurfaces = model.getModelObjects<SubSurface>();
  ASSERT_EQ(1u,subSurfaces.size());
  EXPECT_EQ("GlassDoor",subSurfaces[0].subSurfaceType());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_SubSurface)
{
  Model model;
}


