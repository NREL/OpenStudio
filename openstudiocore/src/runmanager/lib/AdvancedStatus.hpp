#ifndef __runmanager_advanced_status_hpp__
#define __runmanager_advanced_status_hpp__


#include <string>
#include <utilities/core/Enum.hpp>
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



#endif
