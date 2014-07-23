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

#ifndef OPENSTUDIO_OSITEMSELECTORBUTTONS_HPP
#define OPENSTUDIO_OSITEMSELECTORBUTTONS_HPP

#include "MainTabView.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <QWidget>

class QPushButton;
class QVBoxLayout;

namespace openstudio {

class OSItem;
class OSItemId;
class OSVectorController;
class OSDropZone;

class OSItemSelectorButtons : public QWidget
{
  Q_OBJECT

  public:

    OSItemSelectorButtons(QWidget* parent = 0);

    virtual ~OSItemSelectorButtons() {}

    void showDropZone();
    void hideDropZone();
    void enableDropZone();
    void disableDropZone();

    void showAddButton();
    void hideAddButton();
    void enableAddButton();
    void disableAddButton();

    void showCopyButton();
    void hideCopyButton();
    void enableCopyButton();
    void disableCopyButton();

    void showRemoveButton();
    void hideRemoveButton();
    void enableRemoveButton();
    void disableRemoveButton();

    void showPurgeButton();
    void hidePurgeButton();
    void enablePurgeButton();
    void disablePurgeButton();

    void showBclDlgButton();
    void hideBclDlgButton();
    void enableBclDlgButton();
    void disableBclDlgButton();

  signals:

    void itemDropped(const OSItemId& itemId);

    void addClicked();

    void copyClicked();

    void removeClicked();

    void purgeClicked();

    void downloadComponentsClicked();

  protected:

    void paintEvent ( QPaintEvent * event );

  private:

    QVBoxLayout* m_vLayout;

    OSVectorController* m_dropZoneController;

    OSDropZone* m_dropZone;

    QPushButton* m_addButton;

    QPushButton* m_copyButton;

    QPushButton* m_removeButton;

    QPushButton* m_purgeButton;

    QPushButton* m_openBclDlgButton;

    QPushButton* m_openLibDlgButton;

};


} // openstudio

#endif // OPENSTUDIO_OSITEMSELECTORBUTTONS_HPP

