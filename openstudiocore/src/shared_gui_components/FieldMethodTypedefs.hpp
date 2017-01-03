/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef SHAREDGUICOMPONENTS_FIELDMETHODTYPEDEFS_HPP
#define SHAREDGUICOMPONENTS_FIELDMETHODTYPEDEFS_HPP

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

typedef std::function<bool ()> BoolGetter;
typedef std::function<double ()> DoubleGetter;
typedef std::function<int ()> IntGetter;
typedef std::function<std::string ()> StringGetter;
typedef std::function<unsigned ()> UnsignedGetter;
typedef std::function<model::ModelObject ()> ModelObjectGetter;

typedef std::function<boost::optional<double> ()> OptionalDoubleGetter;
typedef std::function<boost::optional<int> ()> OptionalIntGetter;
typedef std::function<boost::optional<std::string> ()> OptionalStringGetter;
typedef std::function<boost::optional<std::string> (bool)> OptionalStringGetterBoolArg;
typedef std::function<boost::optional<unsigned> ()> OptionalUnsignedGetter;
typedef std::function<boost::optional<model::ModelObject> ()> OptionalModelObjectGetter;

typedef std::function<void(bool)> BoolSetter;
typedef std::function<bool(bool)> BoolSetterBoolReturn;
typedef std::function<bool(double)> DoubleSetter;
typedef std::function<void (double)> DoubleSetterVoidReturn;
typedef std::function<bool (int)> IntSetter;
typedef std::function<bool(std::string)> StringSetter;
typedef std::function<void(std::string)> StringSetterVoidReturn;
typedef std::function<boost::optional<std::string>(const std::string &)> StringSetterOptionalStringReturn;
typedef std::function<bool (unsigned)> UnsignedSetter;
typedef std::function<bool (const model::ModelObject &)> ModelObjectSetter;

typedef std::function<void ()> NoFailAction;
typedef std::function<bool ()> BasicQuery;

}

#endif // SHAREDGUICOMPONENTS_FIELDMETHODTYPEDEFS_HPP
