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

#include "QMetaTypes.hpp"

namespace openstudio {
namespace detail{

  // Note JM 2018-12-19: `Q_DECLARE_METATYPE` is enough to use a type inside a QVariant, but qRegisterMetaType is needed to use the type in
  // *queued* signals/slots and to dynamically create objects of these types at runtime

  int __iddobjectype_type = qRegisterMetaType<openstudio::IddObjectType>("openstudio::IddObjectType");
  int __iddfiletype_type = qRegisterMetaType<openstudio::IddFileType>("openstudio::IddFileType");

  int __ositemid_type = qRegisterMetaType<OSItemId>("OSItemId");
  int __ositemid_vector_type = qRegisterMetaType<std::vector<OSItemId> >("std::vector<OSItemId>");

  // qRegisterMetaType<openstudio::model::ModelObject>("openstudio::model::ModelObject"); // No default constructor!
  // qRegisterMetaType<boost::optional<openstudio::model::ModelObject> >("boost::optional<openstudio::model::ModelObject>");
  // qRegisterMetaType<std::vector<openstudio::model::ModelObject> >("std::vector<openstudio::model::ModelObject>" );

  int __uuid_type = qRegisterMetaType<openstudio::UUID>("openstudio::UUID");

  int __string_type = qRegisterMetaType<std::string>("std::string");
  int __string_vector_type = qRegisterMetaType<std::vector<std::string> >("std::vector<std::string>");

  int __optional_double_type = qRegisterMetaType<boost::optional<double> >("boost::optional<double>");
  int __optional_unsigned_type = qRegisterMetaType<boost::optional<unsigned> >("boost::optional<unsigned>");
  int __optional_int_type = qRegisterMetaType<boost::optional<int> >("boost::optional<int>");
  int __optional_string_type = qRegisterMetaType<boost::optional<std::string> >("boost::optional<std::string>");

  // qRegisterMetaType<openstudio::Attribute>("openstudio::Attribute");
  int __attribute_optional_type__ = qRegisterMetaType<boost::optional<openstudio::Attribute> >("boost::optional<openstudio::Attribute>");
  int __atribute_vector_type = qRegisterMetaType<std::vector<openstudio::Attribute> >("std::vector<openstudio::Attribute>");

  int __quantity_type = qRegisterMetaType<openstudio::Quantity>("openstudio::Quantity");
  int __optionalquantity_type = qRegisterMetaType<openstudio::OSOptionalQuantity>("openstudio::OSOptionalQuantity");

  int __workspaceobject_type = qRegisterMetaType<std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> >();

} // detail
} // openstudio
