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

#ifndef PATAPP_MEASURESVIEW_HPP
#define PATAPP_MEASURESVIEW_HPP

#include "PatMainTabView.hpp"
#include "../shared_gui_components/OSCollapsibleView.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/HeaderViews.hpp"
#include <QAbstractButton>
#include "../shared_gui_components/VariableView.hpp"

class QLineEdit;
class QLabel;
class QPushButton;

namespace openstudio {

class SyncMeasuresDialog;

namespace pat {

namespace measuretab {

class MeasuresView;
class VariableItemView;
class NewGroupDropZone;
class NewGroupView;
class VariableGroupContentView;
class MeasureDropZone;

} // measuretab

class MeasuresTabView : public PatMainTabView
{
  Q_OBJECT

  public:

  MeasuresTabView();

  virtual ~MeasuresTabView() {}

  QWidget * mainContent;

  OSListView * variableGroupListView;

  QLabel * baselineCautionLabel;

  QWidget * baselineNameBackground;

  QLabel * baselineLabel;

  QPushButton * selectBaselineButton;

  private:

  QPushButton * m_updateMeasuresButton;

  boost::shared_ptr<SyncMeasuresDialog> m_syncMeasuresDialog;

  private slots:

  void openUpdateMeasuresDlg();

};

} // pat

} // openstudio

#endif // PATAPP_MEASURESVIEW_HPP
