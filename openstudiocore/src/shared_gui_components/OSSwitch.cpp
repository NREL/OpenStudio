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

#include "OSSwitch.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/optional.hpp>
#include <QString>

namespace openstudio {

OSSwitch2::OSSwitch2( QWidget * parent )
  : QPushButton(parent)
{
  this->setAcceptDrops(false);
  setFlat(true);
  setFixedSize(63,21);

  makeOnOff();

  this->setCheckable(true);
}

OSSwitch2::~OSSwitch2()
{
}

void OSSwitch2::makeOnOff()
{
  setObjectName("OnOffSliderButton");
}

void OSSwitch2::makeTrueFalse()
{
  setObjectName("TrueFalseSliderButton");
}

void OSSwitch2::bind(model::ModelObject & modelObject,
                     BoolGetter get,
                     boost::optional<BoolSetter> set,
                     boost::optional<NoFailAction> reset,
                     boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;
  m_get = get;
  m_set = set;
  m_reset = reset;
  m_isDefaulted = isDefaulted;

  m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<OSSwitch2, &OSSwitch2::onModelObjectChange>(this);
  m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.connect<OSSwitch2, &OSSwitch2::onModelObjectRemove>(this);
  
  connect(this, &OSSwitch2::clicked, this, &OSSwitch2::onClicked);

  onModelObjectChange();
}

void OSSwitch2::unbind()
{
  if (m_modelObject){
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<OSSwitch2, &OSSwitch2::onModelObjectChange>(this);
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.disconnect<OSSwitch2, &OSSwitch2::onModelObjectRemove>(this);
    m_modelObject.reset();
    m_get.reset();
    m_set.reset();
    m_reset.reset();
    m_isDefaulted.reset();
  }
}

void OSSwitch2::onClicked(bool checked)
{
  if(m_modelObject && m_set) {
    if ((*m_get)() != checked) {
      (*m_set)(checked);
    }
  }
}

void OSSwitch2::onModelObjectChange()
{
  if( m_modelObject ) {
    if ((*m_get)() != this->isChecked()) {
      this->blockSignals(true);
      this->setChecked((*m_get)());
      this->blockSignals(false);
    }
  }
}

void OSSwitch2::onModelObjectRemove(const Handle& handle) {
  unbind();
}

// OSSwitch::OSSwitch( QWidget * parent )
//   : QPushButton(parent)
// {
//   this->setAcceptDrops(false);
//   setFlat(true);
//   setFixedSize(63,21);

//   setObjectName("OnOffSliderButton");

//   this->setCheckable(true);
// }

// void OSSwitch::bind(model::ModelObject & modelObject, const char * property)
// {
//   m_modelObject = modelObject;

//   m_property = property;

//   m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<OSSwitch, &OSSwitch::onModelObjectChange>(this);
//   m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.connect<OSSwitch, &OSSwitch::onModelObjectRemove>(this);

//   connect(this, &OSSwitch::clicked, this, &OSSwitch::onClicked);

//   onModelObjectChange();
// }

// void OSSwitch::unbind()
// {
//   if (m_modelObject){
//     m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<OSSwitch, &OSSwitch::onModelObjectChange>(this);
//     m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.disconnect<OSSwitch, &OSSwitch::onModelObjectRemove>(this);

//     m_modelObject.reset();
//     m_property = "";
//   }
// }

// void OSSwitch::onClicked(bool checked)
// {
//   if( m_modelObject )
//   {
//     m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(),checked);
//   }
// }

// void OSSwitch::onModelObjectChange()
// {
//   if( m_modelObject )
//   {
//     QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());

//     if( variant.canConvert<bool>() )
//     {
//       this->setChecked(variant.value<bool>());
//     }
//   }
// }

// void OSSwitch::onModelObjectRemove(const Handle& handle)
// {
//   m_modelObject.reset();
//   m_property = "";
// }

} // openstudio

