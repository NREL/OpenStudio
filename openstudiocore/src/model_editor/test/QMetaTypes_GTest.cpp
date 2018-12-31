/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ModelEditorFixture.hpp"

#include <utilities/idd/IddEnums.hpp>
#include <utilities/idd/IddEnums.hxx>

#include "../QMetaTypes.hpp"
#include <QMetaType>
#include <QVariant>

// QModelIndex

using namespace openstudio;

TEST_F(ModelEditorFixture, IddObjectType_QVariant)
{
  IddObjectType type(IddObjectType::Zone);
  QVariant variant = QVariant::fromValue(type);
  EXPECT_EQ("openstudio::IddObjectType", std::string(variant.typeName()));
  ASSERT_TRUE(variant.canConvert<IddObjectType>());
  IddObjectType type2 = variant.value<IddObjectType>();
  EXPECT_EQ(type.value(), type2.value());
}


//Q_DECLARE_METATYPE(openstudio::IddFileType)
//Q_DECLARE_METATYPE(openstudio::IddObjectType)

//#include "../openstudio_lib/OSItem.hpp"
//Q_DECLARE_METATYPE(openstudio::OSItemId)
//Q_DECLARE_METATYPE(std::vector<openstudio::OSItemId>)

//// #include <model/ModelObject.hpp>
//// Note JM 2018-12-13: Was already commented out
//// Q_DECLARE_METATYPE(openstudio::model::ModelObject); // no default constructor
//// Q_DECLARE_METATYPE(boost::optional<openstudio::model::ModelObject>);
//// Q_DECLARE_METATYPE(std::vector<openstudio::model::ModelObject>);


//#include <utilities/core/UUID.hpp>
//Q_DECLARE_METATYPE(openstudio::UUID);
TEST_F(ModelEditorFixture, UUID_QVariant)
{
  Handle handle = createUUID();
  QVariant variant = QVariant::fromValue(handle);
  EXPECT_EQ("openstudio::UUID", std::string(variant.typeName()));
  ASSERT_TRUE(variant.canConvert<Handle>());
  Handle handle2 = variant.value<Handle>();
  EXPECT_EQ(handle, handle2);
}


//#include <string>
//Q_DECLARE_METATYPE(std::string);
//Q_DECLARE_METATYPE(std::vector<std::string>);


//#include <boost/optional.hpp>

//// Original comment: declare these types so we can use them as properties
//Q_DECLARE_METATYPE(boost::optional<double>);
//Q_DECLARE_METATYPE(boost::optional<unsigned>);
//Q_DECLARE_METATYPE(boost::optional<int>);
//Q_DECLARE_METATYPE(boost::optional<std::string>);

//#include <utilities/data/Attribute.hpp>
////Q_DECLARE_METATYPE(openstudio::Attribute);
//Q_DECLARE_METATYPE(boost::optional<openstudio::Attribute>);
//Q_DECLARE_METATYPE(std::vector<openstudio::Attribute>);

//#include <utilities/units/Quantity.hpp>
//Q_DECLARE_METATYPE(openstudio::Quantity);

//#include <utilities/units/OSOptionalQuantity.hpp>
//Q_DECLARE_METATYPE(openstudio::OSOptionalQuantity);

//#include <utilities/idf/Workspace_Impl.hpp>
//Q_DECLARE_METATYPE(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>)

