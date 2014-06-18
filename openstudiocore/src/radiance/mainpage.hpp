/** \mainpage OpenStudio Radiance Translator
*
* \section intro_sec Introduction
*
* The Radiance Translator translates a Building Model to Radiance specific formats and imports results
* from Radiance back into the OpenStudio SQLite database schema.
*
* \section design_intent_sec Design Intent
*
* The design intent of this block is to reuse as much of the EnergyPlus Idd as possible to define objects for Radiance simulations. For example, we hope to use EnergyPlus surfaces, sub surfaces, materials, light shelves, daylight sensors, etc. Where necessary, the OpenStudio Idd will be extended from EnergyPlus to include lighting layouts, lighting control zones, luminaire photometric distribution files, photosensor orientation, interior partitions, Radiance analysis zones and other data not present in the EnergyPlus Idd. 
*
* \subsection forward_sec Forward Translator
*
* Converts a OpenStudio Building Model to Radiance input format(s).
*
* \subsection results_sec Results Translator
*
* Converts results of Radiance simulation(s) to OpenStudio SqlFile format. We anticipate translating timeseries illuminance map data and lighting control response (lighting power schedules). Images and animations are a final product for presentation but are not used in the Building Model, therefore these are not translated. Comparison of EnergyPlus daylighting results with Radiance results is a known goal so care will be taken that Radiance results do not overwrite EnergyPlus results. 
*
*/
