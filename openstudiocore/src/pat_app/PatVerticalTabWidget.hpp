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

#ifndef PATAPP_PATVERTICALTABWIDGET_HPP
#define PATAPP_PATVERTICALTABWIDGET_HPP

#include <QWidget>
#include <QSharedPointer>
#include <vector>

class QButtonGroup;

class QPushButton;

namespace openstudio {

class OSViewSwitcher;
  
namespace pat { 

class PatVerticalTabWidget : public QWidget
{
  Q_OBJECT

  public:

  OSViewSwitcher * mainViewSwitcher;

  PatVerticalTabWidget(QWidget * parent = nullptr);

  virtual ~PatVerticalTabWidget();
  
  void addTab( int id,
               QString toolTip,
               const QString & selectedImagePath,
               const QString & unSelectedImagePath,
               const QString & disabledImagePath,
               const QString & doneImagePath,
               bool enabled );

  void enableTab(bool enable, int index);

  void enableTabs(bool enable);

  signals:

  void tabSelected(int id);

  public slots:

  void selectTab(int id);

  void selectNone();

  void nextTab();

  void previousTab();

  private slots:

  void setCurrentIndex(int index);

  void refreshIcons();

  private:

  QWidget * m_tabBar;

  std::vector<QString> m_selectedPixmaps;

  std::vector<QString> m_unSelectedPixmaps;

  std::vector<QString> m_disabledPixmaps;

  std::vector<QString> m_donePixmaps;

  QButtonGroup * m_buttonGroup;

  int currentIndex;

  private slots:

  void on_buttonClicked(int id);

};

} // namespace pat

}

#endif // PATAPP_PATVERTICALTABWIDGET_HPP

