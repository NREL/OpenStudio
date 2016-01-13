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

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../Material.hpp"
#include "../Material_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../StandardsInformationMaterial.hpp"
#include "../StandardsInformationMaterial_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/idf/IdfFile.hpp"

#include <utilities/idd/OS_Construction_FieldEnums.hxx>
#include <utilities/idd/OS_Material_FieldEnums.hxx>
#include <utilities/idd/OS_Material_AirGap_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Material)
{
  Model model;

  StandardOpaqueMaterial exterior(model);

  MaterialVector materials = model.getModelObjects<Material>();
  ASSERT_EQ(static_cast<unsigned>(1), materials.size());

  StandardsInformationMaterialVector materialInformations = model.getModelObjects<StandardsInformationMaterial>();
  ASSERT_EQ(static_cast<unsigned>(0), materialInformations.size());

  {
    Model testModel;

    exterior.clone(testModel);

    materials = model.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = model.getModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(0), materialInformations.size());

    materials = testModel.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = testModel.getModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(0), materialInformations.size());
  }

  StandardsInformationMaterial info = exterior.standardsInformation();

  {
    Model testModel;

    exterior.clone(testModel);


    materials = model.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = model.getModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(1), materialInformations.size());

    materials = testModel.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = testModel.getModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(1), materialInformations.size());
  }

  {
    Model testModel;

    info.clone(testModel);

    materials = model.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = model.getModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(1), materialInformations.size());

    materials = testModel.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(0), materials.size());

    materialInformations = testModel.getModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(1), materialInformations.size());
  }

  StandardOpaqueMaterial exteriorClone = exterior.clone().cast<StandardOpaqueMaterial>();

  materials = model.getModelObjects<Material>();
  ASSERT_EQ(static_cast<unsigned>(2), materials.size());

  materialInformations = model.getModelObjects<StandardsInformationMaterial>();
  ASSERT_EQ(static_cast<unsigned>(2), materialInformations.size());

}
