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

#ifndef PROJECT_VAGRANTSETTINGSRECORD_HPP
#define PROJECT_VAGRANTSETTINGSRECORD_HPP

#include "ProjectAPI.hpp"
#include "CloudSettingsRecord.hpp"

namespace openstudio {

class VagrantSettings;

namespace project {

class UrlRecord;

namespace detail {

  class VagrantSettingsRecord_Impl;

} // detail

/** VagrantSettingsRecord is a CloudSettingsRecord. */
class PROJECT_API VagrantSettingsRecord : public CloudSettingsRecord {
 public:

  typedef detail::VagrantSettingsRecord_Impl ImplType;
  typedef CloudSettingsRecordColumns ColumnsType;
  typedef CloudSettingsRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  VagrantSettingsRecord(const VagrantSettings& vagrantSettings, ProjectDatabase& database);

  VagrantSettingsRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~VagrantSettingsRecord() {}

  //@}

  /** Get VagrantSettingsRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<VagrantSettingsRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<VagrantSettingsRecord> getVagrantSettingsRecords(ProjectDatabase& database);

  static boost::optional<VagrantSettingsRecord> getVagrantSettingsRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  UrlRecord serverUrlRecord() const;

  UrlRecord workerUrlRecord() const;

  VagrantSettings vagrantSettings() const;

  //@}
 protected:
  /// @cond
  explicit VagrantSettingsRecord(std::shared_ptr<detail::VagrantSettingsRecord_Impl> impl);

  friend class detail::VagrantSettingsRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  VagrantSettingsRecord(std::shared_ptr<detail::VagrantSettingsRecord_Impl> impl,
                        ProjectDatabase database);

  /// @endcond

  void constructRelatedRecords(const VagrantSettings& vagrantSettings, ProjectDatabase& database);

 private:
  REGISTER_LOGGER("openstudio.project.VagrantSettingsRecord");
};

/** \relates VagrantSettingsRecord*/
typedef boost::optional<VagrantSettingsRecord> OptionalVagrantSettingsRecord;

/** \relates VagrantSettingsRecord*/
typedef std::vector<VagrantSettingsRecord> VagrantSettingsRecordVector;

} // project
} // openstudio

#endif // PROJECT_VAGRANTSETTINGSRECORD_HPP

