/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef RUNMANAGER_LIB_RUNMANAGERSTATUS_HPP
#define RUNMANAGER_LIB_RUNMANAGERSTATUS_HPP

#include <string>
#include <QTimer>
#include "RunManager.hpp"
#include <ui_RunManagerStatus.h>
#include "JobErrors.hpp"
#include "JobStatusWidget.hpp"


namespace openstudio {
namespace runmanager {
  class RunManager;

  //! QDialog based user interface for displaying RunManager queue job status to the user
  //! \sa http://doc.qt.nokia.com/qdialog.html
  class RunManagerStatus : public QDialog
  {
    Q_OBJECT

  public:
    //! Create a RunManagerStatus dialog for displaying job status
    //! \param[in] errors The errors to display
    RunManagerStatus(QWidget *parent, Qt::WindowFlags flags, const RunManager &t_runmanager);
    virtual ~RunManagerStatus();

  private:
    //! Used for QDesigner classes.
    Ui::RunManagerStatus ui;

    JobStatusWidget *m_statuswidget;
  };


} // runmanager namespace
} // openstudio namespace


#endif // RUNMANAGER_LIB_RUNMANAGERSTATUS_HPP
