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

#ifndef OPENSTUDIO_OSITEMSELECTORBUTTONS_HPP
#define OPENSTUDIO_OSITEMSELECTORBUTTONS_HPP

#include "MainTabView.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <QWidget>

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

namespace openstudio {

class OSItem;
class OSItemId;
class OSVectorController;
class OSDropZone;

class OSItemSelectorButtons : public QWidget
{
  Q_OBJECT

  public:

    OSItemSelectorButtons(QWidget* parent = nullptr);

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

    void paintEvent ( QPaintEvent * event ) override;

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

    QHBoxLayout * m_dropZoneLayout;
};


} // openstudio

#endif // OPENSTUDIO_OSITEMSELECTORBUTTONS_HPP

