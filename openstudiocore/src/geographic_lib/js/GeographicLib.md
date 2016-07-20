## Geodesic routines from GeographicLib

This documentation applies to version 1.45.

### Installation

This library is a JavaScript implementation of the geodesic routines
from [GeographicLib](http://geographiclib.sf.net).  This solves the
direct and inverse geodesic problems for an ellipsoid of revolution.

The library can be used in [node](https://nodejs.org) by first
installing the
[geographiclib node package](https://www.npmjs.com/package/geographiclib)
with [npm](https://www.npmjs.com)
```bash
$ npm install geographiclib
$ node
> var GeographicLib = require("geographiclib");
```
Alternatively, you can use it in client-side JavaScript, by including in
your HTML page
```html
<script type="text/javascript"
        src="http://geographiclib.sf.net/scripts/geographiclib.js">
</script>
```
Both of these prescriptions define a {@link GeographicLib} namespace.

### Examples

Now geodesic calculations can be carried out, for example,
```javascript
var geod = GeographicLib.Geodesic.WGS84, r;

// Find the distance from Wellington, NZ (41.32S, 174.81E) to
// Salamanca, Spain (40.96N, 5.50W)...
r = geod.Inverse(-41.32, 174.81, 40.96, -5.50);
console.log("The distance is " + r.s12.toFixed(3) + " m.");
// This prints "The distance is 19959679.267 m."

// Find the point 20000 km SW of Perth, Australia (32.06S, 115.74E)...
r = geod.Direct(-32.06, 115.74, 225, 20000e3);
console.log("The position is (" +
            r.lat2.toFixed(8) + ", " + r.lon2.toFixed(8) + ").");
// This prints "The position is (32.11195529, -63.95925278)."
```
Two examples of this library in use are
* [A geodesic calculator](http://geographiclib.sf.net/scripts/geod-calc.html)
* [Displaying geodesics on Google
  Maps](http://geographiclib.sf.net/scripts/geod-google.html)

### More information
* {@tutorial 1-geodesics}
* {@tutorial 2-interface}
* {@tutorial 3-examples}

### Authors

* algorithms + js code: Charles Karney (charles@karney.com)
* node.js port: Yurij Mikhalevich (0@39.yt)
