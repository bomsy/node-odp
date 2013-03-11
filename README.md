# Node-odp

A node library that wraps the odp.net dll to provide bindings to oracle on windows.

## Requirements
 Install [ODAC]: http://www.oracle.com/technetwork/database/windows/downloads/index-101290.html, making sure the policy dlls are shared (placed in the GAC). See installation instructions [here]: http://www.oracle.com/technetwork/topics/dotnet/install112012-164342.html

 Install Microsoft visual C++ 2010 sp1 redistributable package.

## Installation

    npm install node-odp

## How to use (Sample code)

    var odp = require('node-odp');
    var con = new odpnode.OracleConnection("Data Source=mhladmin;User Id=/;");

    var parameterDirection = odpnode.OracleData.parameterDirection;
    var datatypes = odpnode.OracleData.dbType;
    var cmdType = odpnode.OracleCommand.commandType;

    var parameters = [
	    {
		    name: "vResult",
		    type: datatypes.VARCHAR2,
		    size: 4000,
		    value: '',
		    direction: parameterDirection.OUTPUT
	    },
	    {
		    name: "pFOLDERID",
		    type: datatypes.INT32,
		    value: "1845",
		    direction: parameterDirection.INPUT
	    }	
    ]

    //Simple select statement
    var cmdSelStatment = new odpnode.OracleCommand("SELECT m.FOLDERID,m.ZONE.ZONECODE FROM MID.MIFOLDER2 m,MID.MIVERSION s WHERE m.VERSION.VERSIONID = s.VERSIONID AND  m.ZONE.ZONECODE ='chwhrf-bldg'", commandType.TEXT, con);
    cmdSelStatement.executeReader(function(err, rows){
	    if(err){
		    console.log(err);
	    }else{
		    console.log(rows);
		    console.log(rows[1]["ZONE_ZONECODE"]);
	    }	
    });

    //Oracle function with parameters
    var cmdProc = new odpnode.OracleCommand("BEGIN :vResult := MID.MI_MAXSCRIPT.MITest( :pFOLDERID ); END;", cmdType.TEXT, parameters, con);
    cmdProc.executeNonQuery(function(err, rowsAffected, params){
	    if(err){
		    console.log(err);
	    }else{
		    console.log(params);
	    }	
    });

## API Definition

### OracleConnection

    OracleConnection(connectionString);

The constructor takes one parameter and returns a connection object. The connection string is any valid Oracle connection string.

### OracleCommand

    OracleCommand(commandString, commandType, commandParameters, connectionObject);

##### COMMANDSTRING

This can either be a select statement or a function as shown  in the sample code above. This is a required parameter.

##### COMMANDTYPE

This an object of the OracleCommand.commandType enumeration object. This is a required parameter.

##### COMMANDPARAMETERS

This an array of parameter objects as shown above. Each parameter object is defined as 

    {
    	name: parameterName,
    	type: dataTypeObject,
    	size: size(optional),
    	value: parameterContent,
    	direction: parameterDirectionObject
    }

This parameter is optional.

##### CONNECTIONOBJECT

This is a connection object created by OracleConnection. This is a required parameter.

#### .executeReader

#### .executeNonQuery

#### .executeScalar

#### .commandType

### OracleData

#### .parameterDirection

#### .dbType



## Release History

v 0.1.0 - just .node file .

v 0.1.1 - added all the source files, updated binding.gyp.

v 0.1.2 - corrections.

v 0.1.3 - bug fixes.

v 0.1.4 - bug fixes.

v 0.1.9 - added dependency dlls to package msvcp100d.dll, msvcr100d.dll.

v 0.1.10 - compiled to 32 bit node.

v 0.1.11 - complied to x64.

v 0.1.14 - fixed exception handling in odpconnection.cc

v 0.1.15 - changed dependency path in and PLATFORM to win32 binding.gyp 

v 0.1.18 - updated readme

v 0.1.19 - deps on node-gyp, bindings not needed. updated readme, remove dlls not needed, added 32 bit support

## Licence

Copyright (c) 2013 Hubert Boma Manilla  
Licensed under the MIT license.
=======