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

#ifndef ANALYSIS_ANALYSISOBJECT_IMPL_HPP
#define ANALYSIS_ANALYSISOBJECT_IMPL_HPP

#include "AnalysisAPI.hpp"

#include "AnalysisObject.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/UUID.hpp"

#include <QObject>

namespace openstudio {
namespace analysis {
namespace detail {

  /** AnalysisObject_Impl is the implementation class for AnalysisObject.*/
  class ANALYSIS_API AnalysisObject_Impl : public QObject, public std::enable_shared_from_this<AnalysisObject_Impl>
  {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor. Initializes UUIDs. */
    AnalysisObject_Impl(const std::string& name);

    /** Constructor provided for deserialization; not for general use. */
    AnalysisObject_Impl(const UUID& uuid,
                        const UUID& versionUUID,
                        const std::string& name,
                        const std::string& displayName,
                        const std::string& description);

    AnalysisObject_Impl(const AnalysisObject_Impl& other);

    virtual ~AnalysisObject_Impl() {}

    /** Deep copy. */
    virtual AnalysisObject clone() const = 0;

    //@}
    /** @name Getters */
    //@{

    std::string name() const;

    std::string displayName() const;

    std::string description() const;

    openstudio::UUID uuid() const;

    openstudio::UUID versionUUID() const;

    //@}
    /** @name Queries */
    //@{

    /** Returns true if this object has been changed since its dirty flag was last cleared. New
     *  objects are dirty post-construction. De-serialized objects are not. The intention is for
     *  users of the analysis framework to clear the dirty flag after an object is saved to its
     *  ProjectDatabase. */
    bool isDirty() const;

    bool uuidEqual(const AnalysisObject& other) const;

    bool uuidAndVersionEqual(const AnalysisObject& other) const;

    //@}
    /** @name Setters */
    //@{

    virtual void setName(const std::string& newName);

    virtual void setDisplayName(const std::string& newDisplayName);

    virtual void setDescription(const std::string& newDescription);

    //@}
    /** @name Actions */
    //@{

    /** Call this method to reset the dirty flag of this object and all its children. In general,
     *  this method should be called after this object has been saved to the ProjectDatabase
     *  (record constructed, database saved, and transaction committed). Returns false if the flag
     *  cannot be cleared for some reason. */
    virtual bool clearDirtyFlag();

    //@}
    /** @name Type Casting */
    //@{

    /** Get a public object that wraps this impl.*/
    template<typename T>
    T getPublicObject() const {
      T result(std::dynamic_pointer_cast<typename T::ImplType>(
                 std::const_pointer_cast<AnalysisObject_Impl>(shared_from_this())));
      return result;
    }

    //@}
    enum ChangeType {
      Benign,
      InvalidatesResults,
      InvalidatesDataPoints,
    };

    /** @name Protected in or Absent from Public Class */
    //@{

    /** Changes version UUID, sets to dirty, and emits changed signal. */
    virtual void onChange(ChangeType changeType);

    boost::optional<AnalysisObject> parent() const;

    void setParent(const AnalysisObject& parent) const;

    void clearParent() const;

    virtual QVariant toVariant() const;

    virtual QVariant toServerFormulationVariant() const;

    virtual QVariant toServerDataPointsVariant() const;

    /// Relocate path data from originalBase to newBase.
    virtual void updateInputPathData(const openstudio::path& originalBase,
                                     const openstudio::path& newBase);

    //@}
   public slots:
    /** Default implementation calls onChange(invalidateDataPoints). */
    void onChildChanged(ChangeType changeType);

    /** Default implementation calls clearDirtyFlag(). */
    void onParentClean();

   signals:
    void changed(ChangeType changeType);

    void clean();
   protected:
    openstudio::UUID m_uuid;
    openstudio::UUID m_versionUUID;
    std::string m_name;
    std::string m_displayName;
    std::string m_description;

    void connectChild(AnalysisObject& child, bool setParent) const;

    void disconnectChild(AnalysisObject& child, bool clearParent=true) const;

    // Intended for overriding default construction behavior, as appropriate. Example:
    // deserializing an Analysis with resultsAreInvalid or dataPointsAreInvalid.
    void setDirtyFlag();

   private:
    REGISTER_LOGGER("openstudio.analysis.AnalysisObject");

    bool m_dirty;

    // Gross to make this mutable, but if don't, constructor interfaces
    // all change from taking const references to taking references whenever
    // need to hook up a parent-child relationship.
    mutable boost::optional<AnalysisObject> m_parent;
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_ANALYSISOBJECT_IMPL_HPP

