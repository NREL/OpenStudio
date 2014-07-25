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
    Q_OBJECT;
    Q_PROPERTY(std::string city READ city WRITE setCity);
    Q_PROPERTY(std::string stateProvinceRegion READ stateProvinceRegion WRITE setStateProvinceRegion);
    Q_PROPERTY(std::string country READ country WRITE setCountry);
    Q_PROPERTY(std::string dataSource READ dataSource WRITE setDataSource);
    Q_PROPERTY(std::string wMONumber READ wMONumber WRITE setWMONumber);
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude);
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude);
    Q_PROPERTY(double timeZone READ timeZone WRITE setTimeZone);
    Q_PROPERTY(double elevation READ elevation WRITE setElevation RESET resetElevation);
    Q_PROPERTY(bool isElevationDefaulted READ isElevationDefaulted);
    Q_PROPERTY(boost::optional<std::string> url READ url);
    Q_PROPERTY(boost::optional<std::string> checksum READ checksum);
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
    virtual boost::optional<ParentObject> parent() const;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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

    void setCity(std::string city);

    void setStateProvinceRegion(std::string stateProvinceRegion);

    void setCountry(std::string country);

    void setDataSource(std::string dataSource);

    void setWMONumber(std::string wMONumber);

    bool setLatitude(double latitude);

    bool setLongitude(double longitude);

    bool setTimeZone(double timeZone);

    bool setElevation(double elevation);

    void resetElevation();

    //void setUrl(boost::optional<std::string> url);

    //void setUrl(std::string url);

    //void resetUrl();

    //void setChecksum(boost::optional<std::string> checksum);

    //void setChecksum(std::string checksum);

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

