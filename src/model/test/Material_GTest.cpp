/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
