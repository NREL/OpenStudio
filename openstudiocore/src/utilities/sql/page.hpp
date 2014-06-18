/** \page sql_page OpenStudio Sql
*
* \section sql_file SqlFile
*
* EnergyPlus is capable of writing to many output data formats.  The SQLite output of EnergyPlus is particularly
* useful because it conforms to a well defined and documented schema, can be accessed through the well known
* SQL language, and includes a large portion of the possible EnergyPlus output.  The database schema used by EnergyPlus
* is well suited to storing results of building energy simulation engines; it includes timeseries, illuminance map data, as well
* as many others.  Therefore, similarly to the Idd, OpenStudio has adopted the EnergyPlus SQLite database schema as a starting point
* for it's own output data format.  The OpenStudio SQLite schema may be extended to include content not in the EnergyPlus schema.
*
*/