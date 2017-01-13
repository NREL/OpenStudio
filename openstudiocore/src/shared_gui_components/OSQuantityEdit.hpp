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

#ifndef SHAREDGUICOMPONENTS_OSQUANTITYEDIT_HPP
#define SHAREDGUICOMPONENTS_OSQUANTITYEDIT_HPP

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include "FieldMethodTypedefs.hpp"

#include "../model/ModelObject.hpp"

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Logger.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QValidator>

class QFocusEvent;

namespace openstudio {

class Unit;

class QuantityLineEdit : public QLineEdit {
  Q_OBJECT
public:

  QuantityLineEdit(QWidget * parent = nullptr);

  virtual ~QuantityLineEdit() {}

  void enableClickFocus() { this->m_hasClickFocus = true; }

protected:

  virtual void focusInEvent(QFocusEvent * e) override;

  virtual void focusOutEvent(QFocusEvent * e) override;

private:

  bool m_hasClickFocus = false;

signals:

  void inFocus(bool inFocus);

};

class OSQuantityEdit2: public QWidget, public Nano::Observer {
  Q_OBJECT
 public:

  OSQuantityEdit2(const std::string& modelUnits, const std::string& siUnits, 
                  const std::string& ipUnits, bool isIP, QWidget * parent = nullptr);

  virtual ~OSQuantityEdit2();

  void enableClickFocus();

  QDoubleValidator * doubleValidator() { return m_doubleValidator; }

  bool hasData() { return !this->m_lineEdit->text().isEmpty(); }

  void bind(bool isIP,
            model::ModelObject& modelObject,
            DoubleGetter get,
            boost::optional<DoubleSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(bool isIP,
            model::ModelObject& modelObject,
            OptionalDoubleGetter optionalGet,
            boost::optional<DoubleSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(bool isIP,
            model::ModelObject& modelObject,
            DoubleGetter get,
            DoubleSetterVoidReturn set,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(bool isIP,
            model::ModelObject& modelObject,
            OptionalDoubleGetter optionalGet,
            DoubleSetterVoidReturn set,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void unbind();

 signals:

  void inFocus(bool inFocus, bool hasData);

 public slots:

  void onUnitSystemChange(bool isIP);

 private slots:

  void onEditingFinished();

  void onModelObjectChange();

  void onModelObjectRemove(const Handle& handle);

  void onInFocus(bool inFocus);

 private:

  QuantityLineEdit* m_lineEdit;
  QLabel* m_units;
  QString m_text = "UNINITIALIZED";
  std::string m_unitsStr = "";
  QDoubleValidator * m_doubleValidator;

  bool m_isIP;
  std::string m_modelUnits;
  std::string m_siUnits;
  std::string m_ipUnits;
  boost::optional<model::ModelObject> m_modelObject;
  boost::optional<DoubleGetter> m_get;
  boost::optional<OptionalDoubleGetter> m_optionalGet;
  boost::optional<DoubleSetter> m_set;
  boost::optional<DoubleSetterVoidReturn> m_setVoidReturn;
  boost::optional<NoFailAction> m_reset;
  boost::optional<NoFailAction> m_autosize;
  boost::optional<NoFailAction> m_autocalculate;
  boost::optional<BasicQuery> m_isDefaulted;
  boost::optional<BasicQuery> m_isAutosized;
  boost::optional<BasicQuery> m_isAutocalculated;

  bool m_isScientific;
  boost::optional<int> m_precision;

  void refreshTextAndLabel();

  void setPrecision(const std::string& str);

  void completeBind(bool isIP,
                    model::ModelObject& modelObject,
                    boost::optional<NoFailAction> reset,
                    boost::optional<NoFailAction> autosize,
                    boost::optional<NoFailAction> autocalculate,
                    boost::optional<BasicQuery> isDefaulted,
                    boost::optional<BasicQuery> isAutosized,
                    boost::optional<BasicQuery> isAutocalculated);

  REGISTER_LOGGER("openstudio.OSQuantityEdit");
};

/** \deprecated Use OSQuantityEdit2. */
// class OSQuantityEdit: public QWidget, public Nano::Observer {
//   Q_OBJECT
  
//  public:

//   OSQuantityEdit(bool isIP, QWidget * parent = nullptr);

//   virtual ~OSQuantityEdit() {}

//   QDoubleValidator * doubleValidator() { return m_doubleValidator; }

//   void bind(model::ModelObject& modelObject,
//             const char* property,
//             bool isIP,
//             const boost::optional<std::string>& isDefaultedProperty = boost::none,
//             const boost::optional<std::string>& isAutosizedProperty = boost::none,
//             const boost::optional<std::string>& isAutocalucatedProperty = boost::none);

//   void unbind();

//  public slots:

//   void onUnitSystemChange(bool isIP);

//  private slots:

//   void onEditingFinished();

//   void onModelObjectChange();

//   void onModelObjectRemove(const Handle& handle);

//  private:

//   QLineEdit* m_lineEdit;
//   QLabel* m_units;

//   boost::optional<model::ModelObject> m_modelObject;
//   std::string m_property;
//   bool m_isIP;
//   boost::optional<std::string> m_isDefaultedProperty;
//   boost::optional<std::string> m_isAutosizedProperty;
//   boost::optional<std::string> m_isAutocalculatedProperty;

//   bool m_isScientific;
//   boost::optional<int> m_precision;
//   QDoubleValidator * m_doubleValidator;

//   std::string propertyWithSystem() const;

//   void refreshTextAndLabel();

//   void setPrecision(const std::string& str);

//   REGISTER_LOGGER("openstudio.OSQuantityEdit");
// };

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSQUANTITYEDIT_HPP

