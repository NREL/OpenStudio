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

#ifndef RUNMANAGER_LIB_ADVANCEDSTATUS_HPP
#define RUNMANAGER_LIB_ADVANCEDSTATUS_HPP


#include <string>
#include "../../utilities/core/Enum.hpp"
#include "RunManagerAPI.hpp"
#include <QMetaType>

namespace openstudio {
namespace runmanager {

  /** \class AdvancedStatusEnum
   *
   *  \relates AdvancedStatus */
  OPENSTUDIO_ENUM(AdvancedStatusEnum,
      ((Idle))
      ((Starting))
      ((CopyingRequiredFiles)(Copying Required Files))
      ((CopyingRequiredTools)(Copying Required Tools))
      ((ExtractingTools)(Extracting Tools))
      ((Queuing))
      ((WaitingInQueue)(Waiting In Queue))
      ((Processing))
      ((CopyingResultFiles)(Copying Result Files))
      ((Finishing))
   );

  /// Holds an AdvancedStatusEnum value along with an additional description
  ///
  /// Used to report detailed information about the status of an executing Job tree.
  /// It is particularly of use during execution of remote jobs, when the individual steps
  /// of setting up a job take longer.
  class RUNMANAGER_API AdvancedStatus
  {
    public:
      /** Default constructor sets status to Idle. */
      AdvancedStatus();
      AdvancedStatus(const AdvancedStatusEnum &t_enum);
      AdvancedStatus(const AdvancedStatusEnum &t_enum, const std::string &description);

      bool operator==(const AdvancedStatus &rhs) const;
      bool operator!=(const AdvancedStatus &rhs) const;
      bool operator<(const AdvancedStatus &rhs) const;

      std::string toString() const;

      AdvancedStatusEnum value() const;
      std::string description() const;

    private:

      AdvancedStatusEnum m_enum;

      std::string m_description;
  };

}
}

Q_DECLARE_METATYPE(openstudio::runmanager::AdvancedStatus);



#endif // RUNMANAGER_LIB_ADVANCEDSTATUS_HPP
