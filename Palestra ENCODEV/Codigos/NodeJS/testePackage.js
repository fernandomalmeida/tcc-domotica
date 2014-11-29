var PackageModule = require("../Projeto/server/Package.js");
var HTTPRequest = PackageModule.HTTPRequest;
var Package = PackageModule.Package;

var p = new Package(1, 2, 1, 1, 0, "Teste Package");

var p2 = new Package();

p2.setFromByteArray(p.toByteArray());

console.log(p2.toString());
