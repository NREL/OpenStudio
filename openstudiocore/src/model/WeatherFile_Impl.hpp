/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_WEATHERFILE_IMPL_HPP
#define MODEL_WEATHERFILE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

class EpwFile;

namespace model {

class Site;
class WeatherFile;

namespace detail {

  /** WeatherFile_Impl is a ModelObject_Impl that is the implementation class for WeatherFile.*/
  class MODEL_API WeatherFile_Impl : public ModelObject_Impl {













   public:
    /** @name Constructors and Destructors */
    //@{

    WeatherFile_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    WeatherFile_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    WeatherFile_Impl(const WeatherFile_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    virtual ~WeatherFile_Impl() {}

    //@}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const override;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent) override;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    /** @name Getters */
    //@{

    std::string city() const;

    std::string stateProvinceRegion() const;

    std::string country() const;

    std::string dataSource() const;

    std::string wMONumber() const;

    double latitude() const;

    double longitude() const;

    double timeZone() const;

    double elevation() const;

    bool isElevationDefaulted() const;

    boost::optional<std::string> url() const;

    boost::optional<openstudio::path> path() const;

    boost::optional<std::string> checksum() const;

    //@}
    /** @name Setters */
    //@{

    bool setCity(std::string city);

    bool setStateProvinceRegion(std::string stateProvinceRegion);

    bool setCountry(std::string country);

    bool setDataSource(std::string dataSource);

    bool setWMONumber(std::string wMONumber);

    bool setLatitude(double latitude);

    bool setLongitude(double longitude);

    bool setTimeZone(double timeZone);

    bool setElevation(double elevation);

    void resetElevation();

    //bool setUrl(boost::optional<std::string> url);

    //bool setUrl(std::string url);

    //void resetUrl();

    //bool setChecksum(boost::optional<std::string> checksum);

    //bool setChecksum(std::string checksum);

    //void resetChecksum();

    //@}

    // get the site
    boost::optional<Site> site() const;

    /** Load and return the EpwFile, if it can be located. */
    boost::optional<EpwFile> file(const openstudio::path& dir=openstudio::path()) const;

    /** Save the url as relative to basePath, or just keep the file name and extension if
    *  basePath.empty(). */
    bool makeUrlRelative(const openstudio::path& basePath=openstudio::path());

    /** Search for file and save the absolute url if successful. */
    bool makeUrlAbsolute(const openstudio::path& searchDirectory);

    // get environment name as reported in the sql file
    // environment name = "City StateProvinceRegion Country DataSource WMO#=WMONumber"
    boost::optional<std::string> environmentName() const;

protected:
   private:
    REGISTER_LOGGER("openstudio.model.WeatherFile");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_WEATHERFILE_IMPL_HPP
