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

#ifndef SHAREDGUICOMPONENTS_EDITVIEW_HPP
#define SHAREDGUICOMPONENTS_EDITVIEW_HPP

#include <QWidget>
#include <QComboBox>
#include <QAbstractButton>
#include <QLabel>

#include <boost/optional.hpp>

class QLineEdit;
class QTextEdit;
class QVBoxLayout;

namespace openstudio{


class InputCheckBox;

class EditNullView : public QWidget
{
  Q_OBJECT

  public:

  EditNullView(const QString & text = "Select a Measure to Edit");
  virtual ~EditNullView() {}

  protected:

  void paintEvent(QPaintEvent *) override;
};

class EditRubyMeasureView : public QWidget
{
  Q_OBJECT

  public:

  EditRubyMeasureView(bool applyMeasureNow);
  virtual ~EditRubyMeasureView() {}

  // Editable name, maps to OSW 'name'
  QLineEdit * nameLineEdit;

  // Non Editable name, maps to OSW 'measure_dir_name'
  QLineEdit * nameNonEditableLineEdit;

  QTextEdit * descriptionTextEdit;

  QTextEdit * modelerDescriptionTextEdit;

  void addInputView(QWidget * widget);

  // Clear all of the information specific to a particular ruby perturbation
  void clear();

  protected:

  void paintEvent(QPaintEvent *) override;

  private:

  std::vector<QWidget *> m_inputViews;

  QVBoxLayout * m_mainVLayout;

  QVBoxLayout * m_inputsVLayout;
};

class InputView : public QWidget
{
  public:

  virtual void setIncomplete(bool incomplete) {}

  virtual void setDisplayValue(const QVariant & value) {}
};

class DoubleInputView : public InputView
{
  Q_OBJECT

  public:

  DoubleInputView();
  virtual ~DoubleInputView() {}

  QLineEdit * lineEdit;

  void setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description);

  void setIncomplete(bool incomplete) override;

  void setDisplayValue(const QVariant & value) override;

  private:

  QLabel * nameLabel;
};

class ChoiceInputView : public InputView
{
  Q_OBJECT

  public:

  ChoiceInputView();
  virtual ~ChoiceInputView() {}

  QComboBox * comboBox;

  void setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description);

  void setIncomplete(bool incomplete) override;

  void setDisplayValue(const QVariant & value) override;

  private:

  QLabel * nameLabel;
};

class BoolInputView : public InputView
{
  Q_OBJECT

  public:

  BoolInputView();
  virtual ~BoolInputView() {}

  InputCheckBox * checkBox;

  void setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description);

  void setIncomplete(bool incomplete) override;

  void setDisplayValue(const QVariant & value) override;
};

class IntegerInputView : public InputView
{
  Q_OBJECT

  public:

  IntegerInputView();
  virtual ~IntegerInputView() {}

  QLineEdit * lineEdit;

  void setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description);

  void setIncomplete(bool incomplete) override;

  void setDisplayValue(const QVariant & value) override;

  private:

  QLabel * nameLabel;
};

class StringInputView : public InputView
{
  Q_OBJECT

  public:

  StringInputView();
  virtual ~StringInputView() {}

  QLineEdit * lineEdit;

  void setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description);

  void setIncomplete(bool incomplete) override;

  void setDisplayValue(const QVariant & value) override;

  private:

  QLabel * nameLabel;
};

class InputComboBox : public QComboBox
{
  Q_OBJECT

  protected:

  void wheelEvent(QWheelEvent * e) override;
};

class InputCheckBox : public QAbstractButton
{
  Q_OBJECT

  public:

  InputCheckBox();

  virtual ~InputCheckBox();

  void setText(const QString & text);

  void setIncomplete(bool incomplete);

  protected:

  void paintEvent(QPaintEvent * event) override;

  private:

  QLabel * m_label;
};


} // openstudio

#endif // SHAREDGUICOMPONENTS_EDITVIEW_HPP

