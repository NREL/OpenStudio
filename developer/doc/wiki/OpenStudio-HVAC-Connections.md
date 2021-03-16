# OpenStudio Connections

OpenStudio uses a different method of connecting HVAC components compared to EnergyPlus. In EnergyPlus, two components are connected together using a unique node name.

For example in EnergyPlus a heating coil followed by a fan is connected via a unique node name, ```Node 20```.

```
Coil:Heating:Fuel,
  Coil Heating Gas 1,                     !- Name
  Always On Discrete,                     !- Availability Schedule Name
  NaturalGas,                             !- Fuel Type
  0.8,                                    !- Burner Efficiency
  AutoSize,                               !- Nominal Capacity {W}
  Node 19,                                !- Air Inlet Node Name
  Node 20,                                !- Air Outlet Node Name
  Node 20,                                !- Temperature Setpoint Node Name
  0,                                      !- Parasitic Electric Load {W}
  ,                                       !- Part Load Fraction Correlation Curve Name
  0;                                      !- Parasitic Fuel Load {W}

Fan:ConstantVolume,
  Fan Constant Volume 1,                  !- Name
  Always On Discrete,                     !- Availability Schedule Name
  0.7,                                    !- Fan Total Efficiency
  500,                                    !- Pressure Rise {Pa}
  AutoSize,                               !- Maximum Flow Rate {m3/s}
  0.9,                                    !- Motor Efficiency
  1,                                      !- Motor In Airstream Fraction
  Node 20,                                !- Air Inlet Node Name
  Node 12;                                !- Air Outlet Node Name

```

In OpenStudio, the fluid outlet of an HVAC component is joined to the inlet of another component via a ```OS:Connection``` object. The fluid inlets and outlets are called ```Ports``` in OpenStudio. The connection object references a source object and port combined with the target object and port, and in this way arbitrary connections can be made between any two components.

Unlike EnergyPlus, a ```Node``` is simply another component type in the HVAC topology. In the following example there are three components, a gas heating coil, followed by a node, followed by a fan.

```
OS:Coil:Heating:Gas,
  {363b7008-0a0d-4fe7-9db3-68a79a48bff9}, !- Handle
  Coil Heating Gas 1,                     !- Name
  {3f88ddae-e2a9-41a1-a71b-ecde6608231c}, !- Availability Schedule Name
  0.8,                                    !- Gas Burner Efficiency
  AutoSize,                               !- Nominal Capacity {W}
  {dccdd7bc-d226-4808-9efa-90bdc10b94eb}, !- Air Inlet Port, port "5"
  {f449861e-566d-4029-8f94-75a3714fe22c}, !- Air Outlet Port, port "6"
  ,                                       !- Temperature Setpoint Node Name
  0,                                      !- Parasitic Electric Load {W}
  ,                                       !- Part Load Fraction Correlation Curve Name
  0;                                      !- Parasitic Gas Load {W}

OS:Connection,
  {f449861e-566d-4029-8f94-75a3714fe22c}, !- Handle
  {7be3c785-24a5-42c4-b3d4-0e7066304acb}, !- Name
  {363b7008-0a0d-4fe7-9db3-68a79a48bff9}, !- Source Object
  6,                                      !- Outlet Port
  {048330b6-77f4-4087-b2f3-e0f09c368faf}, !- Target Object
  2;                                      !- Inlet Port

OS:Node,
  {048330b6-77f4-4087-b2f3-e0f09c368faf}, !- Handle
  Node 20,                                !- Name
  {f449861e-566d-4029-8f94-75a3714fe22c}, !- Inlet Port, port "2"
  {ec2a011e-f6aa-4857-a70b-e23706f580ea}; !- Outlet Port, port "3"

OS:Connection,
  {ec2a011e-f6aa-4857-a70b-e23706f580ea}, !- Handle
  {de2173c8-53af-4352-a30d-fc5a67f36184}, !- Name
  {048330b6-77f4-4087-b2f3-e0f09c368faf}, !- Source Object
  3,                                      !- Outlet Port
  {a0f6145b-064a-41c9-9c68-c63cb91aa3e5}, !- Target Object
  8;                                      !- Inlet Port

OS:Fan:ConstantVolume,
  {a0f6145b-064a-41c9-9c68-c63cb91aa3e5}, !- Handle
  Fan Constant Volume 1,                  !- Name
  {3f88ddae-e2a9-41a1-a71b-ecde6608231c}, !- Availability Schedule Name
  ,                                       !- Fan Efficiency
  500,                                    !- Pressure Rise {Pa}
  AutoSize,                               !- Maximum Flow Rate {m3/s}
  ,                                       !- Motor Efficiency
  ,                                       !- Motor In Airstream Fraction
  {ec2a011e-f6aa-4857-a70b-e23706f580ea}, !- Air Inlet Port, Port "8"
  {d78d663b-7d54-41b1-acf0-1919b4e28ec0}, !- Air Outlet Port
  ;                                       !- End-Use Subcategory

```

[Here](https://docs.google.com/presentation/d/1NKT7YJzoWFk8EyH1c496dXnqCXlHgACy79H6ZF5zG3k/) is a slightly more visual representation of the data, showing the relationships between the coil, node, and connecting data structure. 

In most cases there is an ```OS:Node``` object between HVAC components, but it is not a requirement. There are a few examples where two OpenStudio HVAC Components connect without an intermediate node, and certainly the connection scheme is designed to be flexible enough to not require a node involved.

The Connection mechanism in OpenStudio involves more data relative to EnergyPlus. Typically this data is not authored directly, but instead connections are formed using OpenStudio's API.

```
m = OpenStudio::Model::Model.new

coil = OpenStudio::Model::CoilHeatingGas.new(m)
node = OpenStudio::Model::Node(m)
fan = OpenStudio::Model::FanConstantVolume.new(m)


m.connect(coil, coil.outletPort, node, node.inletPort)
m.connect(node, node.outletPort, fan, fan.inletPort)
```

OpenStudio clients do not generally use the connection API directly. Instead OpenStudio encourages higher level APIs, that use ```connect``` behind the scenes. With ```HVACComponent::addToNode```, nodes and connections are formed automatically given the context of where an HVAC component is added.

```
m = OpenStudio::Model::Model.new

loop = OpenStudio::Model::AirLoopHVAC.new

coil = OpenStudio::Model::CoilHeatingGas.new(m)
fan = OpenStudio::Model::FanConstantVolume.new(m)

coil.addToNode(loop.supplyOutletNode())
fan.addToNode(loop.supplyOutletNode())
```

# Potential Improvements

Currently ports are nothing but a field index, but they could be enhanced. For starters, ports would be better represented as names instead of indexes, and this improvement would flow naturally with a change from positional based osm format, to a key valued format. Secondly, there is currently only one type of port, a fluid connection, but it would be nice to have port "types" to allow different ports of the same type to be joined.