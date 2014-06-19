/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
 *  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef SHAREDGUICOMPONENTS_EDITVIEW_HPP
#define SHAREDGUICOMPONENTS_EDITVIEW_HPP

#include <QWidget>
#include <QComboBox>
#include <QAbstractButton>
#include <QLabel>

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

  void paintEvent(QPaintEvent *);
};

class EditRubyMeasureView : public QWidget
{
  Q_OBJECT

  public:

  EditRubyMeasureView(bool applyMeasureNow);
  virtual ~EditRubyMeasureView() {}

  QLineEdit * nameLineEdit;

  QTextEdit * descriptionTextEdit;

  QLabel * modelerDescriptionLabel;

  void addInputView(QWidget * widget);

  // Clear all of the information specific to a particular ruby perturbation
  void clear();

  protected:

  void paintEvent(QPaintEvent *);

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

  QLabel * nameLabel; 

  void setIncomplete(bool incomplete);

  void setDisplayValue(const QVariant & value);
};

class ChoiceInputView : public InputView
{
  Q_OBJECT

  public:

  ChoiceInputView();
  virtual ~ChoiceInputView() {}

  QComboBox * comboBox;

  QLabel * nameLabel;

  void setIncomplete(bool incomplete);

  void setDisplayValue(const QVariant & value);
};

class BoolInputView : public InputView
{
  Q_OBJECT

  public:

  BoolInputView();
  virtual ~BoolInputView() {}

  InputCheckBox * checkBox;

  void setIncomplete(bool incomplete);

  void setDisplayValue(const QVariant & value);
};

class IntegerInputView : public InputView
{
  Q_OBJECT

  public:

  IntegerInputView();
  virtual ~IntegerInputView() {}

  QLineEdit * lineEdit;

  QLabel * nameLabel; 

  void setIncomplete(bool incomplete);

  void setDisplayValue(const QVariant & value);
};

class StringInputView : public InputView
{
  Q_OBJECT

  public:

  StringInputView();
  virtual ~StringInputView() {}

  QLineEdit * lineEdit;

  QLabel * nameLabel; 

  void setIncomplete(bool incomplete);

  void setDisplayValue(const QVariant & value);
};

class InputComboBox : public QComboBox
{
  Q_OBJECT

  protected:

  void wheelEvent(QWheelEvent * e);
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

  void paintEvent(QPaintEvent * event);

  private:

  QLabel * m_label;
};


} // openstudio

#endif // SHAREDGUICOMPONENTS_EDITVIEW_HPP

