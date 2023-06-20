/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/StandardGlazing.hpp"
#include "../../model/MaterialPropertyGlazingSpectralData.hpp"

#include <utilities/idd/WindowMaterial_Glazing_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_WindowMaterialGlazing_SpectralAverage) {
  Model model;

  StandardGlazing standardGlazing(model);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(model);

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::WindowMaterial_Glazing));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_windowMaterialGlazing(idfObjs[0]);

  EXPECT_EQ("SpectralAverage", idf_windowMaterialGlazing.getString(WindowMaterial_GlazingFields::OpticalDataType).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_WindowMaterialGlazing_Spectral) {
  Model model;

  StandardGlazing standardGlazing(model);
  MaterialPropertyGlazingSpectralData spectralData(model);
  spectralData.setName("Glazing Spectral Data");
  standardGlazing.setWindowGlassSpectralDataSet(spectralData);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(model);

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::WindowMaterial_Glazing));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_windowMaterialGlazing(idfObjs[0]);

  EXPECT_EQ("Spectral", idf_windowMaterialGlazing.getString(WindowMaterial_GlazingFields::OpticalDataType).get());
  EXPECT_TRUE(idf_windowMaterialGlazing.getString(WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName));
  EXPECT_EQ(spectralData.name().get(), idf_windowMaterialGlazing.getString(WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName).get());
}
