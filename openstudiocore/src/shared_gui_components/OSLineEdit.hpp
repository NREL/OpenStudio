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

#ifndef SHAREDGUICOMPONENTS_OSLINEEDIT_HPP
#define SHAREDGUICOMPONENTS_OSLINEEDIT_HPP

#include "FieldMethodTypedefs.hpp"

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include "../model/Model.hpp"

#include <QLineEdit>

#include <QTimer>

class QFocusEvent;
class QMouseEvent;

namespace openstudio {

class OSItem;

class OSLineEdit2 : public QLineEdit, public Nano::Observer {
  Q_OBJECT

public:

  OSLineEdit2(QWidget * parent = nullptr);

  virtual ~OSLineEdit2();

  void enableClickFocus() { this->m_hasClickFocus = true; }
  void setDeleteObject(bool deleteObject) { m_deleteObject = deleteObject; }
  bool hasData() { return !this->text().isEmpty(); }
  bool deleteObject() { return m_deleteObject; }

  void bind(model::ModelObject& modelObject,
            StringGetter get,
            boost::optional<StringSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none);

  void bind(model::ModelObject& modelObject, 
            OptionalStringGetter get,
            boost::optional<StringSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none);

  void bind(model::ModelObject& modelObject,
            OptionalStringGetterBoolArg get,
            boost::optional<StringSetterOptionalStringReturn> set,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none);

  void bind(model::ModelObject& modelObject,
    StringGetter get,
    boost::optional<StringSetterVoidReturn> set = boost::none,
    boost::optional<NoFailAction> reset = boost::none,
    boost::optional<BasicQuery> isDefaulted = boost::none);

  void unbind();

protected:

  void mouseReleaseEvent(QMouseEvent* event) override;

  virtual void focusInEvent(QFocusEvent * e) override;

  virtual void focusOutEvent(QFocusEvent * e) override;

signals:

  void itemClicked(OSItem* item);

  void objectRemoved(boost::optional<model::ParentObject> parent);

  void inFocus(bool inFocus, bool hasData);

  public slots:

  void onItemRemoveClicked();

  private slots :

  void onEditingFinished();

  void onModelObjectChange();

  void onModelObjectRemove(const Handle& handle);

  void emitItemClicked();

 private:

  void onModelObjectChangeInternal(bool startingup);
  void completeBind();
  void adjustWidth();

  boost::optional<model::ModelObject> m_modelObject;
  boost::optional<StringGetter> m_get;
  boost::optional<OptionalStringGetter> m_getOptional;
  boost::optional<OptionalStringGetterBoolArg> m_getOptionalBoolArg;
  boost::optional<StringSetter> m_set;
  boost::optional<StringSetterOptionalStringReturn> m_setOptionalStringReturn;
  boost::optional<StringSetterVoidReturn> m_setVoidReturn;
  boost::optional<NoFailAction> m_reset;
  boost::optional<BasicQuery> m_isDefaulted;

  OSItem * m_item = nullptr;

  std::string m_text = "";

  QTimer m_timer;

  bool m_hasClickFocus = false;

  bool m_deleteObject = false;
};

// class OSLineEdit : public QLineEdit, public Nano::Observer
// {
//   Q_OBJECT

//   public:

//   OSLineEdit(QWidget * parent = nullptr);

//   virtual ~OSLineEdit() {}

//   void bind(model::ModelObject & modelObject, const char * property);

//   void unbind();

//   private slots:

//   void onEditingFinished();

//   void onModelObjectChange();

//   void onModelObjectRemove(const Handle& handle);

//   private:

//   boost::optional<model::ModelObject> m_modelObject;

//   std::string m_property;
// };

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSLINEEDIT_HPP

