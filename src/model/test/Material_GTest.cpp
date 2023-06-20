/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

TEST_F(ModelFixture, Material) {
  Model model;

  StandardOpaqueMaterial exterior(model);

  MaterialVector materials = model.getModelObjects<Material>();
  ASSERT_EQ(static_cast<unsigned>(1), materials.size());

  StandardsInformationMaterialVector materialInformations = model.getConcreteModelObjects<StandardsInformationMaterial>();
  ASSERT_EQ(static_cast<unsigned>(0), materialInformations.size());

  {
    Model testModel;

    exterior.clone(testModel);

    materials = model.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = model.getConcreteModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(0), materialInformations.size());

    materials = testModel.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = testModel.getConcreteModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(0), materialInformations.size());
  }

  StandardsInformationMaterial info = exterior.standardsInformation();

  {
    Model testModel;

    exterior.clone(testModel);

    materials = model.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = model.getConcreteModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(1), materialInformations.size());

    materials = testModel.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = testModel.getConcreteModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(1), materialInformations.size());
  }

  {
    Model testModel;

    info.clone(testModel);

    materials = model.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(1), materials.size());

    materialInformations = model.getConcreteModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(1), materialInformations.size());

    materials = testModel.getModelObjects<Material>();
    ASSERT_EQ(static_cast<unsigned>(0), materials.size());

    materialInformations = testModel.getConcreteModelObjects<StandardsInformationMaterial>();
    ASSERT_EQ(static_cast<unsigned>(1), materialInformations.size());
  }

  auto exteriorClone = exterior.clone().cast<StandardOpaqueMaterial>();

  materials = model.getModelObjects<Material>();
  ASSERT_EQ(static_cast<unsigned>(2), materials.size());

  materialInformations = model.getConcreteModelObjects<StandardsInformationMaterial>();
  ASSERT_EQ(static_cast<unsigned>(2), materialInformations.size());
}
