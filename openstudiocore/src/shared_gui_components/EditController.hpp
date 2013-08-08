/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_EDITCONTROLLER_H
#define OPENSTUDIO_EDITCONTROLLER_H

#include <ruleset/OSArgument.hpp>
#include <utilities/core/Logger.hpp>
#include <analysis/RubyMeasure.hpp>
#include <QObject>
#include <QPointer>
#include <QSharedPointer> 
#include "VariableList.hpp"

class QWidget;

namespace openstudio {

class OSViewSwitcher;
  
class EditRubyMeasureView;
class EditNullView;
class InputController;
class InputView;

namespace measuretab {

  class MeasureItem;

}

class EditController : public QObject
{
  Q_OBJECT

  public:

  QPointer<OSViewSwitcher> editView;

  QPointer<EditRubyMeasureView> editRubyMeasureView;

  EditController();

  virtual ~EditController();

  void setMeasureItem(measuretab::MeasureItem * measureItem, BaseApp *t_app);

  measuretab::MeasureItem * measureItem() const;

  // Show something when no RubyMeasure is selected
  void reset();

  private slots:

  void updateDescription();

  private:

  REGISTER_LOGGER("openstudio.pat.EditController");

  QPointer<EditNullView> m_editNullView;

  std::vector<QSharedPointer<InputController> > m_inputControllers;

  QPointer<measuretab::MeasureItem> m_measureItem;
};

class InputController : public QObject
{
  Q_OBJECT

  public:

  InputController(EditController * editController,const ruleset::OSArgument & argument, BaseApp *t_app);

  virtual ~InputController();

  QPointer<InputView> inputView;

  private slots:

  void setValue(const QString & value);

  void setValue(bool value);

  void setValueForIndex(int index);

  private:

  bool isItOKToClearResults();

  bool isArgumentIncomplete() const;

  BaseApp *m_app;

  QPointer<EditController> m_editController;

  ruleset::OSArgument m_argument;
};

} // openstudio

#endif // OPENSTUDIO_EDITCONTROLLER_H

