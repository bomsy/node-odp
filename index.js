//***************************************
// Move some of the processing to javascript allowing for flexibility
// Json Encoding and parsing would now be done in javascript
// Support for Utf-8 , utf-16 characters sets has to be implemented in javascript because v8
// does not provide support for it https://code.google.com/p/v8/issues/detail?id=761
//********************************************

//require c++ addon
var _odpnode = require('./libs/build/Release/odpnode.node');

module.exports.OracleData = _odpnode.OracleData;
module.exports.OracleConnection = function(connectionString){
	return new _odpnode.OracleConnection(connectionString);
}
module.exports.OracleCommand = function(commandString, commandType, commandParameters, connectionObject){
	var oCommand = null;
	oCommand = new _odpnode.OracleCommand()
};