/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "IdfFixture.hpp"
#include "../IdfObject.hpp"
#include "../IdfObject_Impl.hpp"
#include "../IdfExtensibleGroup.hpp"
#include "../IdfRegex.hpp"
#include <memory>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../idd/IddRegex.hpp"
#include "../../idd/Comments.hpp"
#include "../../core/Optional.hpp"

#include "../../units/QuantityFactory.hpp"
#include "../../units/QuantityConverter.hpp"
#include "../../units/OSOptionalQuantity.hpp"

#include <utilities/idd/OS_Building_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EquipmentList_FieldEnums.hxx>

#include <resources.hxx>

#include <boost/lexical_cast.hpp>

#include <limits>
#include <type_traits>
#include <sstream>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(IdfFixture, IdfObject_ConstructDefaultsFromIddObjectType) {

  // loop through each IddObject, create a default object, and print to a file.
  // examine, then partition into "good" and candidate for refactor (us or E+)
  openstudio::filesystem::ofstream outFile("defaultObjects.idf");
  ASSERT_TRUE(outFile ? true : false);

  IddObjectVector iddObjects = IddFactory::instance().objects();
  for (const IddObject& iddObject : iddObjects) {
    IdfObject defaultObject(iddObject.type());
    defaultObject.print(outFile);
  }

  outFile.close();
}

TEST_F(IdfFixture, IdfObject_ConstructFromText_Comments) {
  std::string singleLineComment = "! Single line comment";
  std::string multiLineComment = "! Multi" + idfRegex::newLinestring() + "! line " + idfRegex::newLinestring() + "!comment";

  // single line comment above object
  std::string text = singleLineComment + idfRegex::newLinestring() + iddRegex::commentOnlyObjectName() + ";";
  IdfObject object = IdfObject::load(text).get();
  EXPECT_TRUE(object.iddObject().type() == IddObjectType::CommentOnly);
  EXPECT_TRUE(object.isValid(StrictnessLevel::Final));
  EXPECT_EQ(singleLineComment, object.comment());

  // single line comment after object
  text = iddRegex::commentOnlyObjectName() + ";" + singleLineComment;
  object = IdfObject::load(text).get();
  EXPECT_TRUE(object.iddObject().type() == IddObjectType::CommentOnly);
  EXPECT_TRUE(object.isValid(StrictnessLevel::Final));
  EXPECT_EQ(singleLineComment, object.comment());

  // single line comment below object
  text = iddRegex::commentOnlyObjectName() + ";" + idfRegex::newLinestring() + singleLineComment;
  object = IdfObject::load(text).get();
  EXPECT_TRUE(object.iddObject().type() == IddObjectType::CommentOnly);
  EXPECT_TRUE(object.isValid(StrictnessLevel::Final));
  EXPECT_EQ(singleLineComment, object.comment());

  // multi line comment above object
  text = multiLineComment + idfRegex::newLinestring() + iddRegex::commentOnlyObjectName() + ";";
  object = IdfObject::load(text).get();
  EXPECT_TRUE(object.iddObject().type() == IddObjectType::CommentOnly);
  EXPECT_TRUE(object.isValid(StrictnessLevel::Final));
  EXPECT_EQ(multiLineComment, object.comment());

  // multi line comment after object
  text = iddRegex::commentOnlyObjectName() + ";" + multiLineComment;
  object = IdfObject::load(text).get();
  EXPECT_TRUE(object.iddObject().type() == IddObjectType::CommentOnly);
  EXPECT_TRUE(object.isValid(StrictnessLevel::Final));
  EXPECT_EQ(multiLineComment, object.comment());

  // multi line comment below object
  text = iddRegex::commentOnlyObjectName() + ";" + idfRegex::newLinestring() + multiLineComment;
  object = IdfObject::load(text).get();
  EXPECT_TRUE(object.iddObject().type() == IddObjectType::CommentOnly);
  EXPECT_TRUE(object.isValid(StrictnessLevel::Final));
  EXPECT_EQ(multiLineComment, object.comment());
}

TEST_F(IdfFixture, IdfObject_ConstructFromText_MultifieldLines) {
  std::string text = "Building,Building, !- Name\n\
                      30.0,              !- North Axis (deg) \n\
                      City,              !- Terrain \n\
                      0.04,0.4,          !- Load and temperature convergence values\n\
                      FullExterior,      !- Solar Distribution \n\
                      25;                !- Maximum Number of Warmup Days";
  OptionalIdfObject oObj = IdfObject::load(text);
  ASSERT_TRUE(oObj);
  IdfObject building = *oObj;
  EXPECT_EQ(8u, building.numFields());
  ASSERT_TRUE(building.fieldComment(5, false));
  EXPECT_TRUE(building.fieldComment(5, false).get().empty());  // parser should strip !- comments
  ASSERT_TRUE(building.fieldComment(5, true));
  EXPECT_FALSE(building.fieldComment(5, true).get().empty());

  text = "Site:GroundTemperature:BuildingSurface,20.03,20.03,20.13,20.30,20.43,20.52,20.62,20.77,20.78,20.55,20.44,20.20;";
  oObj = IdfObject::load(text);
  ASSERT_TRUE(oObj);
  IdfObject groundTemperature = *oObj;
  EXPECT_EQ(static_cast<unsigned>(12), groundTemperature.numFields());
}

TEST_F(IdfFixture, IdfObject_CopyConstructor) {
  std::string text = "Building,                !- Building \n\
                      Building,                !- Name \n\
                      30.,                     !- North Axis {deg} \n\
                      City,                    !- Terrain \n\
                      0.04,                    !- Loads Convergence Tolerance Value \n\
                      0.4,                     !- Temperature Convergence Tolerance Value {deltaC} \n\
                      FullExterior,            !- Solar Distribution \n\
                      25;                      !- Maximum Number of Warmup Days";

  // make an idf object
  OptionalIdfObject oObj = IdfObject::load(text);
  ASSERT_TRUE(oObj);
  IdfObject building = *oObj;
  EXPECT_TRUE(building.iddObject().type() == IddObjectType::Building);
  EXPECT_TRUE(building.isValid(StrictnessLevel::Final));
  ASSERT_TRUE(building.name());
  EXPECT_EQ("Building", *(building.name()));

  // copy idf object
  IdfObject building2(building);
  EXPECT_TRUE(building2.isValid(StrictnessLevel::Final));
  ASSERT_TRUE(building2.name());
  EXPECT_EQ("Building", *(building2.name()));

  // change building2's name
  building2.setString(0, "New Building");
  EXPECT_TRUE(building2.isValid(StrictnessLevel::Final));
  ASSERT_TRUE(building2.name());
  EXPECT_EQ("New Building", *(building2.name()));

  // also changed first building's name
  EXPECT_TRUE(building.isValid(StrictnessLevel::Final));
  ASSERT_TRUE(building.name());
  EXPECT_EQ("New Building", *(building.name()));
}

TEST_F(IdfFixture, IdfObject_CommentGettersAndSetters) {
  // DEFAULT OBJECT COMMENTS
  IdfObject object(IddObjectType::Zone);
  // exist? if so, are comments according to regex?
  std::string str = object.comment();
  if (!str.empty()) {
    EXPECT_EQ(str, makeComment(str));
  }
  unsigned n = object.numFields();
  for (unsigned i = 0; i < n; ++i) {
    OptionalString oStr = object.fieldComment(i);
    if (oStr && !oStr->empty()) {
      EXPECT_EQ(*oStr, makeComment(*oStr));
    }
  }

  // comment setter works (already comments, and needs comment character prepended)
  str = "! New object comment";
  object.setComment(str);
  EXPECT_EQ(str, object.comment());
  str = "\n\nThis is my object.\n";
  object.setComment(str);
  str = "\n\n! This is my object.\n";
  EXPECT_EQ(str, object.comment());

  // field comment setter works for valid indices
  for (int i = n - 1; i >= 0; --i) {
    std::stringstream ss;
    ss << "Field " << i;
    str = ss.str();
    ss.str("");
    EXPECT_TRUE(object.setFieldComment(i, str));
    ss << "! " << str;
    EXPECT_EQ(ss.str(), object.fieldComment(i).get());
  }

  // field comment setter returns false, does not crash for invalid indices
  EXPECT_FALSE(object.setFieldComment(n + 3, str));

  // TEXT OBJECT COMMENTS
  std::stringstream text;
  text << "  Branch," << '\n'
       << "    Central Chiller Branch," << '\n'
       << "    0,                       !- Maximum Flow Rate {m3/s}" << '\n'
       << "    ," << '\n'
       << "    Chiller:Electric, ! i think we have an electric chiller" << '\n'
       << "    Central Chiller," << '\n'
       << "    Central Chiller Inlet Node," << '\n'
       << "    Central Chiller Outlet Node," << '\n'
       << "    Active;";
  OptionalIdfObject oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  object = *oObj;
  // field comments setter properly extends field comments vector as needed
  OptionalString optStr = object.fieldComment(0);
  ASSERT_TRUE(optStr);
  EXPECT_EQ("", *optStr);
  optStr = object.fieldComment(1);
  ASSERT_TRUE(optStr);
  EXPECT_EQ("", *optStr);  // auto-generated comments are stripped
  optStr = object.fieldComment(3);
  ASSERT_TRUE(optStr);
  EXPECT_EQ("! i think we have an electric chiller", *optStr);
  EXPECT_TRUE(object.setFieldComment(5, "my comment"));
  optStr = object.fieldComment(5);
  ASSERT_TRUE(optStr);
  EXPECT_EQ("! my comment", *optStr);
  optStr = object.fieldComment(4);
  ASSERT_TRUE(optStr);
  EXPECT_EQ("", *optStr);
  optStr = object.fieldComment(6);
  ASSERT_TRUE(optStr);
  EXPECT_TRUE(optStr->empty());

  // field comments setter returns false, does not crash if exceed number of fields.
  EXPECT_FALSE(object.setFieldComment(10, "hi"));
}

TEST_F(IdfFixture, IdfObject_DefaultFieldComments) {
  // OBJECT WITH NO FIELD COMMENTS
  std::stringstream text;
  text << "  Schedule:Day:Interval," << '\n'
       << "    A Schedule," << '\n'
       << "    Any Number," << '\n'
       << "    ," << '\n'
       << "    09:00," << '\n'
       << "    0," << '\n'
       << "    22:00," << '\n'
       << "    1," << '\n'
       << "    24:00," << '\n'
       << "    0;";
  OptionalIdfObject oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  IdfObject object = *oObj;
  // non-extensible fields
  OptionalString fc = object.fieldComment(0, true);
  OptionalIddField iddField = object.iddObject().getField(0);
  ASSERT_TRUE(fc);
  ASSERT_TRUE(iddField);
  EXPECT_EQ(makeIdfEditorComment(iddField->name()), *fc);
  fc = object.fieldComment(0);
  ASSERT_TRUE(fc);
  EXPECT_TRUE(fc->empty());

  fc = object.fieldComment(2, true);
  iddField = object.iddObject().getField(2);
  ASSERT_TRUE(fc);
  ASSERT_TRUE(iddField);
  EXPECT_EQ(makeIdfEditorComment(iddField->name()), *fc);
  fc = object.fieldComment(2);
  ASSERT_TRUE(fc);
  EXPECT_TRUE(fc->empty());

  // extensible fields
  fc = object.fieldComment(6, true);
  iddField = object.iddObject().getField(6);
  ASSERT_TRUE(fc);
  ASSERT_TRUE(iddField);
  EXPECT_EQ(makeIdfEditorComment(iddField->name()) + " 2", *fc);
  fc = object.fieldComment(6);
  ASSERT_TRUE(fc);
  EXPECT_TRUE(fc->empty());

  // non-existant fields
  fc = object.fieldComment(14, true);
  EXPECT_FALSE(fc);

  // OBJECT WITH SOME FIELD COMMENTS
  text.str("");
  text << "  Schedule:Day:Interval," << '\n'
       << "    A Schedule," << '\n'
       << "    Any Number," << '\n'
       << "    ," << '\n'
       << "    09:00, ! opening time" << '\n'
       << "    0," << '\n'
       << "    22:00, ! closing time" << '\n'
       << "    1," << '\n'
       << "    24:00," << '\n'
       << "    0;";
  oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  object = *oObj;
  // returns set values
  fc = object.fieldComment(3, true);
  ASSERT_TRUE(fc);
  EXPECT_EQ("! opening time", *fc);
  fc = object.fieldComment(5, true);
  ASSERT_TRUE(fc);
  EXPECT_EQ("! closing time", *fc);

  // returns default for fields behind fields with set values
  fc = object.fieldComment(4, true);
  iddField = object.iddObject().getField(4);
  ASSERT_TRUE(fc);
  ASSERT_TRUE(iddField);
  EXPECT_EQ(makeIdfEditorComment(iddField->name()) + " 1", *fc);
  fc = object.fieldComment(6);
  ASSERT_TRUE(fc);
  EXPECT_TRUE(fc->empty());

  // returns default for fields past fields with set values
  fc = object.fieldComment(6, true);
  iddField = object.iddObject().getField(6);
  ASSERT_TRUE(fc);
  ASSERT_TRUE(iddField);
  EXPECT_EQ(makeIdfEditorComment(iddField->name()) + " 2", *fc);
  fc = object.fieldComment(6);
  ASSERT_TRUE(fc);
  EXPECT_TRUE(fc->empty());
}

TEST_F(IdfFixture, IdfObject_NameGetterWithReturnDefaultOption) {
  // OBJECT WITH DEFAULT NAME
  std::stringstream text;
  text << "Building," << '\n' << "," << '\n' << "," << '\n' << "," << '\n' << "," << '\n' << "," << '\n' << "," << '\n' << ";";
  OptionalIdfObject oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  IdfObject object = *oObj;
  OptionalString name = object.name();
  ASSERT_TRUE(name);
  EXPECT_TRUE(name->empty());
  name = object.name(true);
  ASSERT_TRUE(name);
  EXPECT_EQ("NONE", *name);
  object.setName("MyBuilding");
  name = object.name();
  ASSERT_TRUE(name);
  EXPECT_EQ("MyBuilding", *name);
  OptionalString name2 = object.name(true);
  ASSERT_TRUE(name2);
  EXPECT_EQ(*name, *name2);
}

TEST_F(IdfFixture, IdfObject_IddObjectTypeInitialization) {
  IdfObject idfObject(IddObjectType::OS_Building);

  // get string should not return empty, initialized optional string
  EXPECT_FALSE(idfObject.getString(OS_BuildingFields::BuildingSectorType, false, true));
  EXPECT_FALSE(idfObject.getDouble(OS_BuildingFields::NorthAxis));
  EXPECT_FALSE(idfObject.getDouble(OS_BuildingFields::NominalFloortoFloorHeight));
}

TEST_F(IdfFixture, IdfObject_StringFieldGetterWithReturnDefaultOption) {
  // NON-EXTENSIBLE OBJECT
  std::stringstream text;
  text << "Refrigeration:Condenser:AirCooled," << '\n'
       << "  MyCondenser," << '\n'
       << "  ," << '\n'
       << "  ," << '\n'  // default is 0.0
       << "  ," << '\n'  // default is "Fixed"
       << "  125.0;";    // default is 250.0
                         // default is 0.2
                         //
                         // default is "General"
                         // default is 0.0
                         // default is 0.0
                         // default is 0.0
  OptionalIdfObject oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  IdfObject object = *oObj;

  // returns set values
  OptionalString idfField = object.getString(0, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("MyCondenser", *idfField);
  idfField = object.getString(1, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("", *idfField);
  idfField = object.getString(4, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("125.0", *idfField);

  // returns default for fields behind fields with set values
  idfField = object.getString(2, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("0.0", *idfField);
  idfField = object.getString(3, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("Fixed", *idfField);

  // returns default for non-existent fields
  idfField = object.getString(6, true);
  EXPECT_FALSE(idfField);
  idfField = object.getString(7, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("General", *idfField);
  idfField = object.getString(8, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("0.0", *idfField);
  idfField = object.getString(10, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("0.0", *idfField);
  idfField = object.getString(11, true);
  EXPECT_FALSE(idfField);

  // EXTENSIBLE OBJECT
  text.str("");
  text << "DaylightingDevice:Tubular," << '\n'
       << "  MyTDD," << '\n'
       << "  MyDome," << '\n'
       << "  MyDiffuser," << '\n'
       << "  MyConstruction," << '\n'
       << "  1.0," << '\n'
       << "  2.0;";
  // \default 0.28
  // Transition Zone 1 Name
  // Transition Zone 1 Length
  // ... (extensible 2)
  oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  object = *oObj;
  EXPECT_EQ(7u, object.numFields());

  // returns set values
  idfField = object.getString(0, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("MyTDD", *idfField);
  idfField = object.getString(5, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("2.0", *idfField);

  // returns default for empty fields
  idfField = object.getString(6, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("0.28", *idfField);
  EXPECT_EQ(7u, object.numFields());
  idfField = object.getString(6);
  EXPECT_TRUE(idfField);

  StringVector newGroup;
  newGroup.push_back("MyFirstTransistionZone");
  newGroup.push_back("1.5");
  ASSERT_FALSE(object.pushExtensibleGroup(newGroup).empty());

  // returns default for fields behind fields with set values
  idfField = object.getString(6, true);
  ASSERT_TRUE(idfField);
  EXPECT_EQ("0.28", *idfField);
  idfField = object.getString(6);
  ASSERT_TRUE(idfField);
  EXPECT_TRUE(idfField->empty());

  // return evaluates to false for extensible fields that do not exist
  idfField = object.getString(10);
  EXPECT_FALSE(idfField);
}

TEST_F(IdfFixture, IdfObject_DoubleFieldGetterWithReturnDefaultOption) {
  std::stringstream text;
  text << "Refrigeration:Condenser:AirCooled," << '\n'
       << "  MyCondenser," << '\n'
       << "  ," << '\n'
       << "  ," << '\n'  // default is 0.0
       << "  ," << '\n'  // default is "Fixed"
       << "  125.0;";    // default is 250.0
                         // default is 0.2
                         //
                         // default is "General"
                         // default is 0.0
                         // default is 0.0
                         // default is 0.0
  OptionalIdfObject oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  IdfObject object = *oObj;

  // returns set values
  OptionalDouble dIdfField = object.getDouble(4, true);
  ASSERT_TRUE(dIdfField);
  EXPECT_NEAR(125.0, *dIdfField, tol);

  // is able to cast default value
  dIdfField = object.getDouble(8, true);
  ASSERT_TRUE(dIdfField);
  EXPECT_NEAR(0.0, *dIdfField, tol);
}

TEST_F(IdfFixture, IdfObject_UnsignedFieldGetterWithReturnDefaultOption) {
  std::stringstream text;
  text << "ZoneGroup," << '\n' << "  MyGroup," << '\n' << "  MyList;";
  // default 1
  OptionalIdfObject oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  IdfObject object = *oObj;

  // is able to cast default value
  OptionalUnsigned uIdfField = object.getUnsigned(2, true);
  ASSERT_TRUE(uIdfField);
  EXPECT_EQ(static_cast<unsigned>(1), *uIdfField);
  EXPECT_FALSE(object.getUnsigned(2));

  EXPECT_TRUE(object.setString(object.numFields(), "3"));

  // returns set values
  uIdfField = object.getUnsigned(2, true);
  ASSERT_TRUE(uIdfField);
  EXPECT_EQ(static_cast<unsigned>(3), *uIdfField);
  EXPECT_TRUE(object.getUnsigned(2));
}

TEST_F(IdfFixture, IdfObject_IntFieldGetterWithReturnDefaultOption) {
  std::stringstream text;
  text << "Building," << '\n'
       << "  Building," << '\n'
       << "  ," << '\n'  // default 0.0
       << "  ," << '\n'  // default Suburbs
       << "  ," << '\n'  // default 0.04
       << "  ," << '\n'  // default 0.4
       << "  ," << '\n'  // default FullExterior
       << "  ," << '\n'  // default 25
       << "  6;";        // default 25
  OptionalIdfObject oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  IdfObject object = *oObj;
  // is able to cast default value
  OptionalInt iIdfField = object.getInt(6, true);
  ASSERT_TRUE(iIdfField);
  EXPECT_EQ(25, *iIdfField);
  EXPECT_FALSE(object.getInt(5));

  // returns set values
  object.setInt(5, -3);
  iIdfField = object.getInt(5, true);
  ASSERT_TRUE(iIdfField);
  EXPECT_EQ(-3, *iIdfField);
  EXPECT_TRUE(object.getInt(5));
}

TEST_F(IdfFixture, IdfObject_FieldSettingWithHiddenPushes) {
  std::stringstream text;
  OptionalIdfObject oObj;
  bool result;

  // SHOULD NOT BE VALID
  text.str("");
  text << "Lights," << '\n'
       << "  MyLight," << '\n'
       << "  MyZone," << '\n'
       << "  MySchedule," << '\n'
       << "  Watts/Person," << '\n'
       << "  ," << '\n'
       << "  ," << '\n'
       << "  10.0," << '\n'
       << "  0.2," << '\n'
       << "  0.6," << '\n'
       << "  0.8;";
  oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  IdfObject object = *oObj;
  // impossible field index
  result = object.setString(20, "not a field");
  EXPECT_FALSE(result);
  EXPECT_EQ(static_cast<unsigned>(11), object.numFields());
  result = object.setUnsigned(20, 1);
  EXPECT_FALSE(result);
  EXPECT_EQ(static_cast<unsigned>(11), object.numFields());
}

TEST_F(IdfFixture, IdfObject_GetQuantity) {
  std::string text = "Building,                !- Building \n\
                      Building,                !- Name \n\
                      30.,                     !- North Axis {deg} \n\
                      City,                    !- Terrain \n\
                      0.04,                    !- Loads Convergence Tolerance Value \n\
                      0.4,                     !- Temperature Convergence Tolerance Value {deltaC} \n\
                      FullExterior,            !- Solar Distribution \n\
                      25;                      !- Maximum Number of Warmup Days";

  // make an idf object
  OptionalIdfObject oObj = IdfObject::load(text);
  ASSERT_TRUE(oObj);

  // Test get.
  OSOptionalQuantity ooq = oObj->getQuantity(4);
  ASSERT_TRUE(ooq.isSet());
  Quantity q = ooq.get();
  EXPECT_TRUE(q.value() == 0.4);
  EXPECT_TRUE(q.system() == UnitSystem::SI);
  EXPECT_TRUE(q.standardUnitsString() == "K");

  // Test set.
  OptionalQuantity oq = convert(q, UnitSystem(UnitSystem::IP));
  ASSERT_TRUE(oq);
  EXPECT_TRUE(oq->system() == UnitSystem::IP);
  EXPECT_DOUBLE_EQ(0.72, oq->value());
  oq->setValue(1.5);

  EXPECT_TRUE(oObj->setQuantity(4, *oq));
  ooq = oObj->getQuantity(4);
  ASSERT_TRUE(ooq.isSet());
  q = ooq.get();
  EXPECT_DOUBLE_EQ(0.83333333333333333, q.value());
  EXPECT_TRUE(q.system() == UnitSystem::SI);
  EXPECT_TRUE(q.standardUnitsString() == "K");
}

TEST_F(IdfFixture, IdfObject_GroupPushingAndPopping) {
  // NON-EXTENSIBLE OBJECT
  IdfObject object(IddObjectType::Lights);
  EXPECT_TRUE(object.pushExtensibleGroup().empty());

  // MINFIELDS INCLUDES AN EXTENSIBLE GROUP, BUT EXTENSIBLE GROUPS STILL INITIALIZED AS EMPTY
  object = IdfObject(IddObjectType::BuildingSurface_Detailed);
  EXPECT_EQ(static_cast<unsigned>(11), object.numFields());
  // push empty strings
  EXPECT_FALSE(object.pushExtensibleGroup().empty());
  EXPECT_EQ(static_cast<unsigned>(14), object.numFields());
  // push non-empty strings (correct number)
  StringVector values;
  values.push_back("2.1");
  values.push_back("100.0");
  values.push_back("0.0");
  EXPECT_FALSE(object.pushExtensibleGroup(values).empty());
  EXPECT_EQ(static_cast<unsigned>(17), object.numFields());
  // try to push incorrect number of non-empty strings
  values.pop_back();
  EXPECT_TRUE(object.pushExtensibleGroup(values).empty());
  EXPECT_EQ(static_cast<unsigned>(17), object.numFields());
  // pop until false
  StringVector result;
  result.push_back("Fake entry.");
  unsigned n = 17;
  while (!result.empty()) {
    result = object.popExtensibleGroup();
    if (!result.empty()) {
      n -= 3;
    }
    EXPECT_EQ(n, object.numFields());
  }
  EXPECT_EQ(static_cast<unsigned>(11), object.numFields());
}

TEST_F(IdfFixture, IdfObject_ScheduleFileWithUrl) {
  // testing that a funky url can be parsed
  std::string text = "Schedule:File, \n\
                      Web Schedule, !- Name \n\
                      , !- Schedule Type Limits Name \n\
                      http://bcl.development.nrel.gov/api?search='terms'@xxx.yyy, !- File Name \n\
                      1, !- Column Number \n\
                      0, !- Rows to Skip at Top \n\
                      8760, !- Number of Hours of Data \n\
                      Comma, !- Column Separator \n\
                      No, !- Interpolate to Timestep \n\
                      , !- Minutes per Item \n\
                      Yes; !- Adjust Schedule for Daylight Savings";

  // make an idf object
  OptionalIdfObject object = IdfObject::load(text);
  ASSERT_TRUE(object);
  ASSERT_EQ(static_cast<unsigned>(10), object->numFields());

  ASSERT_TRUE(object->getString(0));
  EXPECT_EQ("Web Schedule", object->getString(0).get());

  ASSERT_TRUE(object->getString(2));
  EXPECT_EQ("http://bcl.development.nrel.gov/api?search='terms'@xxx.yyy", object->getString(2).get());

  ASSERT_TRUE(object->getString(3));
  EXPECT_EQ("1", object->getString(3).get());

  ASSERT_TRUE(object->getString(6));
  EXPECT_EQ("Comma", object->getString(6).get());

  ASSERT_TRUE(object->getString(7));
  EXPECT_EQ("No", object->getString(7).get());

  ASSERT_TRUE(object->getString(9));
  EXPECT_EQ("Yes", object->getString(9).get());
}

TEST_F(IdfFixture, DoubleDisplayedAsString) {
  double value = 0.05;

  // boost::lexical_cast
  auto str = boost::lexical_cast<std::string>(value);
  // EXPECT_EQ("0.05",str); // behavior is platform dependent
  auto roundTripValue = boost::lexical_cast<double>(str);
  EXPECT_DOUBLE_EQ(value, roundTripValue);

  // std::stringstream
  std::stringstream ss;
  ss << std::setprecision(std::numeric_limits<double>::digits10) << value;
  EXPECT_EQ("0.05", ss.str());
  ss >> roundTripValue;
  EXPECT_DOUBLE_EQ(value, roundTripValue);
}

TEST_F(IdfFixture, IdfObject_SetDouble_NaN_and_Inf) {

  // try with an IdfObject
  // IdfObject does not prevent Infinity and NaN
  IdfObject object(IddObjectType::OS_People_Definition);

  // Set Number of People
  // Check for nan
  EXPECT_TRUE(object.setDouble(3, std::numeric_limits<double>::quiet_NaN()));
  ASSERT_TRUE(object.getDouble(3));
  EXPECT_TRUE(std::isnan(object.getDouble(3).get()));

  // Infinity
  EXPECT_TRUE(object.setDouble(3, std::numeric_limits<double>::infinity()));
  ASSERT_TRUE(object.getDouble(3));
  EXPECT_TRUE(std::isinf(object.getDouble(3).get()));
  EXPECT_TRUE(object.getDouble(3).get() > 100);

  EXPECT_TRUE(object.setDouble(3, -std::numeric_limits<double>::infinity()));
  ASSERT_TRUE(object.getDouble(3));
  EXPECT_TRUE(std::isinf(object.getDouble(3).get()));
  EXPECT_TRUE(object.getDouble(3).get() < -100);

  // try with an IdfExtensibleGroup (Hour, Minute, Value)
  IdfObject object2(IddObjectType::OS_Schedule_Day);
  IdfExtensibleGroup eg = object2.pushExtensibleGroup();
  // set the value field
  // Check for nan
  EXPECT_TRUE(eg.setDouble(2, std::numeric_limits<double>::quiet_NaN()));
  ASSERT_TRUE(eg.getDouble(2));
  EXPECT_TRUE(std::isnan(eg.getDouble(2).get()));

  // Infinity
  EXPECT_TRUE(eg.setDouble(2, std::numeric_limits<double>::infinity()));
  ASSERT_TRUE(eg.getDouble(2));
  EXPECT_TRUE(std::isinf(eg.getDouble(2).get()));
  EXPECT_TRUE(eg.getDouble(2).get() > 100);

  EXPECT_TRUE(eg.setDouble(2, -std::numeric_limits<double>::infinity()));
  ASSERT_TRUE(eg.getDouble(2));
  EXPECT_TRUE(std::isinf(eg.getDouble(2).get()));
  EXPECT_TRUE(eg.getDouble(2).get() < -100);

  // new extensible group
  std::vector<std::string> group{"1", "2"};
  group.push_back(toString(std::numeric_limits<double>::quiet_NaN()));
  EXPECT_EQ(1u, object2.numExtensibleGroups());
  EXPECT_FALSE(object2.pushExtensibleGroup(group).empty());
  EXPECT_EQ(2u, object2.numExtensibleGroups());

  group.clear();
  group = {"1", "2", toString(std::numeric_limits<double>::infinity())};
  EXPECT_EQ(2u, object2.numExtensibleGroups());
  EXPECT_FALSE(object2.pushExtensibleGroup(group).empty());
  EXPECT_EQ(3u, object2.numExtensibleGroups());

  group.clear();
  group = {"1", "2", toString(3.0)};
  EXPECT_EQ(3u, object2.numExtensibleGroups());
  EXPECT_FALSE(object2.pushExtensibleGroup(group).empty());
  EXPECT_EQ(4u, object2.numExtensibleGroups());
}

TEST_F(IdfFixture, IdfObject_ExtensibleGroup_Failure_4268_WholeExtensibleFields) {

  // Test for #4268 - a case that works
  // This includes the two (empty) fields at the end, so that the extensible group is fully provided (it's whole)
  std::string idf_text = R"(
    ZoneHVAC:EquipmentList,
      Zone1Equipment,                         !- Name
      SequentialLoad,                         !- Load Distribution Scheme
      ZoneHVAC:AirDistributionUnit,           !- Zone Equipment Object Type 1
      Zone1DirectAir ADU,                     !- Zone Equipment Name 1
      1,                                      !- Zone Equipment Cooling Sequence 1
      1,                                      !- Zone Equipment Heating or No-Load Sequence 1
      ,                                       !- Zone Equipment Sequential Cooling Fraction Schedule Name 1
      ,                                       !- Zone Equipment Sequential Heating Fraction Schedule Name 1
      ZoneHVAC:WaterToAirHeatPump,            !- Zone Equipment Object Type 2
      Zone1WTAHP,                             !- Zone Equipment Name 2
      2,                                      !- Zone Equipment Cooling Sequence 2
      2,                                      !- Zone Equipment Heating or No-Load Sequence 2
      ,                                       !- Zone Equipment Sequential Cooling Fraction Schedule Name 2
      ;                                       !- Zone Equipment Sequential Heating Fraction Schedule Name 2
  )";

  IdfObject obj = IdfObject::load(idf_text).get();

  unsigned nNonExtensible = obj.iddObject().numFields();
  unsigned groupSize = obj.iddObject().properties().numExtensible;
  EXPECT_EQ(nNonExtensible, obj.numNonextensibleFields());
  EXPECT_EQ(nNonExtensible + groupSize * 2, obj.numFields());
  ASSERT_NO_THROW(obj.numExtensibleGroups());
  EXPECT_EQ(2, obj.numExtensibleGroups());
}

TEST_F(IdfFixture, IdfObject_ExtensibleGroup_Failure_4268_TruncatedExtensibleFields) {

  // Test for #4268 - ASHRAE9012016_OutPatientHealthCare_Denver.idf OS_ASSERT issue
  // Now we truncate the end of the extensible group. This is the case in many E+ example files
  // (ASHRAE9012016_OutPatientHealthCare_Denver.idf for eg)

  std::string idf_text = R"(
    ZoneHVAC:EquipmentList,
      Zone1Equipment,                         !- Name
      SequentialLoad,                         !- Load Distribution Scheme
      ZoneHVAC:AirDistributionUnit,           !- Zone Equipment Object Type 1
      Zone1DirectAir ADU,                     !- Zone Equipment Name 1
      1,                                      !- Zone Equipment Cooling Sequence 1
      1,                                      !- Zone Equipment Heating or No-Load Sequence 1
      ,                                       !- Zone Equipment Sequential Cooling Fraction Schedule Name 1
      ,                                       !- Zone Equipment Sequential Heating Fraction Schedule Name 1
      ZoneHVAC:WaterToAirHeatPump,            !- Zone Equipment Object Type 2
      Zone1WTAHP,                             !- Zone Equipment Name 2
      2,                                      !- Zone Equipment Cooling Sequence 2
      2;                                      !- Zone Equipment Heating or No-Load Sequence 2
  )";

  IdfObject obj = IdfObject::load(idf_text).get();

  unsigned nNonExtensible = obj.iddObject().numFields();
  unsigned groupSize = obj.iddObject().properties().numExtensible;
  EXPECT_EQ(nNonExtensible, obj.numNonextensibleFields());
  EXPECT_EQ(nNonExtensible + groupSize * 2, obj.numFields());
  ASSERT_NO_THROW(obj.numExtensibleGroups());
  EXPECT_EQ(2, obj.numExtensibleGroups());
}

TEST_F(IdfFixture, IdfObject_SpecialMembers) {

  static_assert(!std::is_trivial<IdfObject>{});
  static_assert(!std::is_pod<IdfObject>{});

  // checks if a type has a default constructor
  static_assert(!std::is_default_constructible<IdfObject>{});
  static_assert(!std::is_trivially_default_constructible<IdfObject>{});
  static_assert(!std::is_nothrow_default_constructible<IdfObject>{});

  // checks if a type has a copy constructor
  static_assert(std::is_copy_constructible<IdfObject>{});
  static_assert(!std::is_trivially_copy_constructible<IdfObject>{});
  static_assert(!std::is_nothrow_copy_constructible<IdfObject>{});

  // checks if a type can be constructed from an rvalue reference
  // Note: Types without a move constructor, but with a copy constructor that accepts const T& arguments, satisfy std::is_move_constructible.
  // Move constructors are usually noexcept, since otherwise they are unusable in any code that provides strong exception guarantee.
  static_assert(std::is_move_constructible<IdfObject>{});
  static_assert(!std::is_trivially_move_constructible<std::shared_ptr<openstudio::detail::IdfObject_Impl>>{});
  static_assert(!std::is_trivially_move_constructible<IdfObject>{});
  static_assert(std::is_nothrow_move_constructible<IdfObject>{});

  // checks if a type has a copy assignment operator
  static_assert(std::is_copy_assignable<IdfObject>{});
  static_assert(!std::is_trivially_copy_assignable<IdfObject>{});
  static_assert(!std::is_nothrow_copy_assignable<IdfObject>{});  // We didn't specify noexcept on the user defined one

  // checks if a type has a move assignment operator
  static_assert(std::is_move_assignable<IdfObject>{});
  static_assert(!std::is_trivially_move_assignable<std::shared_ptr<openstudio::detail::IdfObject_Impl>>{});
  static_assert(!std::is_trivially_move_assignable<IdfObject>{});
  static_assert(std::is_nothrow_move_assignable<IdfObject>{});

  // checks if a type has a non-deleted destructor
  static_assert(std::is_destructible<IdfObject>{});
  static_assert(!std::is_trivially_destructible<std::shared_ptr<openstudio::detail::IdfObject_Impl>>{});
  static_assert(!std::is_trivially_destructible<IdfObject>{});
  static_assert(std::is_nothrow_destructible<IdfObject>{});

  // checks if a type has a virtual destructor
  static_assert(std::has_virtual_destructor<IdfObject>{});

  // checks if objects of a type can be swapped with objects of same or different type
  static_assert(std::is_swappable<IdfObject>{});
  static_assert(std::is_nothrow_swappable<IdfObject>{});
}
