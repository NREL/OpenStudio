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

#ifndef SHAREDGUICOMPONENTS_OSINTEGEREDIT_HPP
#define SHAREDGUICOMPONENTS_OSINTEGEREDIT_HPP

#include "FieldMethodTypedefs.hpp"

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include "../model/ModelObject.hpp"
#include "../model/ModelExtensibleGroup.hpp"

#include "../utilities/core/Logger.hpp"

#include <QLineEdit>
#include <QString>
#include <QValidator>
#include <QIntValidator>

class QFocusEvent;

namespace openstudio {

class OSIntegerEdit2: public QLineEdit, public Nano::Observer {
  Q_OBJECT

 public:

  OSIntegerEdit2(QWidget * parent = nullptr);

  virtual ~OSIntegerEdit2();

  void enableClickFocus() { this->m_hasClickFocus = true; }

  bool hasData() { return !this->text().isEmpty(); }

  QIntValidator * intValidator() { return m_intValidator; }

  void bind(model::ModelObject& modelObject,
            IntGetter get,
            boost::optional<IntSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(model::ModelObject& modelObject,
            OptionalIntGetter get,
            boost::optional<IntSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(model::ModelExtensibleGroup& modelExtensibleGroup,
            IntGetter get,
            boost::optional<IntSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(model::ModelExtensibleGroup& modelExtensibleGroup,
            OptionalIntGetter get,
            boost::optional<IntSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void unbind();

 protected:

  virtual void focusInEvent(QFocusEvent * e) override;

  virtual void focusOutEvent(QFocusEvent * e) override;

 signals:

  void inFocus(bool inFocus, bool hasData);

 private slots:

  void onEditingFinished();

  void onModelObjectChange();

  void onModelObjectRemove(const Handle& handle);

 private:
  boost::optional<model::ModelObject> m_modelObject; // will be set if attached to ModelObject or ModelExtensibleGroup
  boost::optional<model::ModelExtensibleGroup> m_modelExtensibleGroup; // will only be set if attached to ModelExtensibleGroup
  boost::optional<IntGetter> m_get;
  boost::optional<OptionalIntGetter> m_getOptional;
  boost::optional<IntSetter> m_set;
  boost::optional<NoFailAction> m_reset;
  boost::optional<NoFailAction> m_autosize;
  boost::optional<NoFailAction> m_autocalculate;
  boost::optional<BasicQuery> m_isDefaulted;
  boost::optional<BasicQuery> m_isAutosized;
  boost::optional<BasicQuery> m_isAutocalculated;

  bool m_isScientific;
  bool m_hasClickFocus = false;
  boost::optional<int> m_precision;
  QString m_text = "UNINITIALIZED";
  QIntValidator * m_intValidator = nullptr;

  void refreshTextAndLabel();

  void setPrecision(const std::string& str);

  void completeBind();

  REGISTER_LOGGER("openstudio.OSIntegerEdit");
};

// class OSIntegerEdit: public QLineEdit, public Nano::Observer {
//   Q_OBJECT

//  public:

//   OSIntegerEdit(QWidget * parent = nullptr);

//   virtual ~OSIntegerEdit() {}

//   QIntValidator * intValidator() { return m_intValidator; }

//   void bind(model::ModelObject& modelObject,
//             const char* property,
//             const boost::optional<std::string>& isDefaultedProperty = boost::none,
//             const boost::optional<std::string>& isAutosizedProperty = boost::none,
//             const boost::optional<std::string>& isAutocalucatedProperty = boost::none);

//   void unbind();

//  private slots:

//   void onEditingFinished();

//   void onModelObjectChange();

//   void onModelObjectRemove(const Handle& handle);

//  private:
//   boost::optional<model::ModelObject> m_modelObject;
//   std::string m_property;
//   boost::optional<std::string> m_isDefaultedProperty;
//   boost::optional<std::string> m_isAutosizedProperty;
//   boost::optional<std::string> m_isAutocalculatedProperty;

//   bool m_isScientific;
//   boost::optional<int> m_precision;
//   QIntValidator * m_intValidator = nullptr;

//   void refreshTextAndLabel();

//   void setPrecision(const std::string& str);

//   REGISTER_LOGGER("openstudio.OSIntegerEdit");
// };

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSINTEGEREDIT_HPP

