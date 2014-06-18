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


