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

#ifndef SHAREDGUICOMPONENTS_PAGENAVIGATOR_HPP
#define SHAREDGUICOMPONENTS_PAGENAVIGATOR_HPP

#include <QWidget>

class QAbstractButton;
class QButtonGroup;
class QColor;
class QHBoxLayout;

namespace openstudio {

class PageNavigator : public QWidget
{
  Q_OBJECT

public:
  PageNavigator(int numPages,
    int numPagesListed = 5,
    QWidget * parent = nullptr);
  virtual ~PageNavigator();
  void setNumPages(int numPages);
  void setNumPagesListed(int numPagesListed = 5);
  ///! Sets widget to display first page (pageIdx = 0)
  void firstPage();

protected:

private:
  void createLayout();
  ///! sets all the page numbers displayed by the buttons
  void setButtonPageNumbers(int lowestPageNum);
  void makeNumberedButtons();

  int m_numPagesListed;
  int m_numPages;
  QColor m_checkedTextColor;
  QColor m_uncheckedTextColor;
  QButtonGroup * m_buttonGroup;
  QHBoxLayout * m_buttonLayout;

private slots:
  void on_buttonClicked(QAbstractButton * button);
  void on_firstPage(bool checked);
  void on_previousPage(bool checked);
  void on_nextPage(bool checked);
  void on_lastPage(bool checked);

signals:
  void getComponentsByPage(int pageNum);

};

} // namespace openstudio

#endif // SHAREDGUICOMPONENTS_PAGENAVIGATOR_HPP
