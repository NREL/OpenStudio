set -x

tag=v0.7.1

curl -sL -o component_v2.0.xsd https://raw.githubusercontent.com/NREL/bcl-gem/$tag/schemas/v2/component_2012_11_08.xsd
curl -sL -o measure_v2.0.xsd https://raw.githubusercontent.com/NREL/bcl-gem/$tag/schemas/v2/measure_2013_3_26.xsd

curl -sL -o component_v3.0.xsd https://raw.githubusercontent.com/NREL/bcl-gem/$tag/schemas/v3/component_v3.xsd
curl -sL -o measure_v3.0.xsd https://raw.githubusercontent.com/NREL/bcl-gem/$tag/schemas/v3/measure_v3.xsd
