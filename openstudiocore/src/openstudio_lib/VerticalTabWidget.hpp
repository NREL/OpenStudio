/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_VERTICALTABWIDGET_HPP
#define OPENSTUDIO_VERTICALTABWIDGET_HPP

#include <QWidget>

#include <vector>

class QPushButton;

namespace openstudio {

class MainTabView;

class OSViewSwitcher;

class VerticalTabWidget : public QWidget
{
  Q_OBJECT

  public:

  VerticalTabWidget(QWidget * parent = 0);

  virtual ~VerticalTabWidget() {}

  void addTabButton(int id,
                    QString toolTip,
                    const QString & selectedImagePath,
                    const QString & unSelectedImagePath,
                    const QString & disabledImagePath);

  void enableTabButton(int id, bool enable);

  void refreshTabButtons();

  void setView(MainTabView * view, int id);

  MainTabView * view() const;

  int verticalTabIndex();

  signals:

  void tabSelected(int id);

  public slots:

  void setCurrentIndex(int index);

  private slots:

  void select();

  private:

  int getIndex(int id);

  OSViewSwitcher * m_viewSwitcher = nullptr;

  QWidget * m_tabBar;

  std::vector<QString> m_selectedPixmaps;

  std::vector<QString> m_unSelectedPixmaps;

  std::vector<QString> m_disabledPixmaps;

  std::vector<QPushButton *> m_tabButtons; 

  std::vector<int> m_ids;

  int currentIndex = -1;
};

} // namespace openstudio

#endif // OPENSTUDIO_VERTICALTABWIDGET_HPP
