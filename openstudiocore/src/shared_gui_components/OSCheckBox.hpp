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

#ifndef SHAREDGUICOMPONENTS_OSCHECKBOX_HPP
#define SHAREDGUICOMPONENTS_OSCHECKBOX_HPP

#include "FieldMethodTypedefs.hpp"

#include "../model/Model.hpp"

#include <QCheckBox>
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include <QPushButton>

namespace openstudio {

  class OSCheckBox3 : public QCheckBox, public Nano::Observer {
    Q_OBJECT

  public:

    OSCheckBox3(QWidget * parent = nullptr);

    virtual ~OSCheckBox3();

    void bind(model::ModelObject & modelObject,
      BoolGetter get,
      boost::optional<BoolSetter> set = boost::none,
      boost::optional<NoFailAction> reset = boost::none,
      boost::optional<BasicQuery> isDefaulted = boost::none);

    void bind(model::ModelObject & modelObject,
      BoolGetter get,
      boost::optional<BoolSetterBoolReturn> set = boost::none,
      boost::optional<NoFailAction> reset = boost::none,
      boost::optional<BasicQuery> isDefaulted = boost::none);

    void unbind();

    private slots:

    void onToggled(bool checked);

    void onModelObjectChange();

    void onModelObjectRemove(const Handle& handle);

  private:
    boost::optional<model::ModelObject> m_modelObject;
    boost::optional<BoolGetter> m_get;
    boost::optional<BoolSetter> m_set;
    boost::optional<BoolSetterBoolReturn> m_setBoolReturn;
    boost::optional<NoFailAction> m_reset;
    boost::optional<BasicQuery> m_isDefaulted;
  };

  class OSCheckBox2 : public QPushButton, public Nano::Observer {
  Q_OBJECT

 public:

  OSCheckBox2(QWidget * parent = nullptr);

  virtual ~OSCheckBox2() {}

  void bind(model::ModelObject & modelObject,
            BoolGetter get,
            boost::optional<BoolSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none);

  void unbind();

 private slots:

  void onToggled(bool checked);

  void onModelObjectChange();

  void onModelObjectRemove(const Handle& handle);

 private:
  boost::optional<model::ModelObject> m_modelObject;
  boost::optional<BoolGetter> m_get;
  boost::optional<BoolSetter> m_set;
  boost::optional<NoFailAction> m_reset;
  boost::optional<BasicQuery> m_isDefaulted;
};

// class OSCheckBox : public QPushButton, public Nano::Observer {
//   Q_OBJECT

//  public:

//   OSCheckBox(QWidget * parent = nullptr);

//   virtual ~OSCheckBox() {}

//   void bind(model::ModelObject & modelObject, const char * property);

//   void unbind();

//  private slots:

//   void onToggled(bool checked);

//   void onModelObjectChange();

//   void onModelObjectRemove(const Handle& handle);

//  private:

//   boost::optional<model::ModelObject> m_modelObject;

//   std::string m_property;
// };

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSCHECKBOX_HPP

