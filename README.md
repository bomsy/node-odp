# NODE-ODP

A node library that wraps the Oracle Data Provider for.NET (ODP.NET) library to provide bindings to oracle on windows.

## Requirements
 Install [ODAC](http://www.oracle.com/technetwork/database/windows/downloads/index-101290.html), making sure the policy dlls are shared (placed in the GAC). See installation instructions [here](http://www.oracle.com/technetwork/topics/dotnet/install112012-164342.html)

 Install Microsoft visual C++ 2010  redistributable package (x86 or x64 or both).

 [x86](http://www.microsoft.com/en-us/download/details.aspx?id=5555)
 
 [x64](http://www.microsoft.com/en-us/download/details.aspx?id=14632)

 Update: Install [Node v0.10.x](http://nodejs.org/)

## Installation

    npm install node-odp

## How to use (Sample code) ***edit to fit your scenario***

    var odpnode = require('node-odp');
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
    var cmdSelStatment = new odpnode.OracleCommand("SELECT m.FOLDERID,m.ZONE.ZONECODE FROM MID.MIFOLDER2 m,MID.MIVERSION s WHERE m.VERSION.VERSIONID = s.VERSIONID AND  m.ZONE.ZONECODE ='chwhrf-bldg'", cmdType.TEXT, con);
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

The constructor takes one parameter and returns a connection object. The connection string is any valid Oracle connection string. For oracle connectionstring formats see [here](http://www.connectionstrings.com/oracle)

### OracleCommand

    OracleCommand(commandString, commandType, commandParameters, connectionObject);

##### commandString

This can either be a select statement or a function as shown  in the sample code above. This is a required parameter.

##### commandType

This an object of the OracleCommand.commandType enumeration object. This is a required parameter.

##### commandParameters

This an array of parameter objects as shown above. Each parameter object is defined as 

    {
    	name: parameterName,
    	type: dataTypeObject,
    	size: size(optional),
    	value: parameterContent,
    	direction: parameterDirectionObject
    }

This parameter is optional.

##### connectionObject

This is a connection object created by OracleConnection. This is a required parameter.


#### .executeReader

    executeReader(function(err, rows){

    })

This function takes a callback and passes the err and rows objects to that callback.

The err object holds the error if there is any and undefined if there is none. 

The rows holds the JSON object of the rows returned. These rows are identified by their row numbers and column names as shown in the sample code above.


#### .executeNonQuery
    executeNonQuery(function(err, rowsAffected, parameters){

    })

This function takes a callback and passes the err, rowsAffected and parameters objects to the callback.

The err object holds the error if there is any and undefined if there is none. 

The rowsAffected returns the number of rows Affected by current execution, but returns -1 if there is none.

The parameters object returns all the parameters attached to the command, allowing for access to any out or in/out parameter data.

#### .executeScalar
    executeScalar(function(err, rowsAffected, parameters){

    })

Sames as for executeNonQuery

#### .commandType

Oracle Command Constants for defining data text type

                commandType.TEXT
                            .STOREDPROCEDURE

returns constants defining the type of the command.

### OracleData

A data object storing key/value pairs of constants representing various oracle types

#### .parameterDirection

Various constants for specifying parameter direction

    parameterDirection.INPUT
                      .OUTPUT
                      .INPUTOUTPUT
                      .RETURNVALUE

#### .dbType

Various constants for specifying data types

                dbType.BFILE
                     .BLOB
                     .BYTE
                     .CHAR
                     .CLOB
                     .DATE
                     .DECIMAL
                     .DOUBLE
                     .INT16
                     .INT32
                     .INT64
                     .INTERVALDS
                     .INTERVALYM
                     .LONG
                     .LONGRAW
                     .RAW
                     .REFCURSOR
                     .NVARCHAR
                     .SINGLE
                     .TIMESTAMP
                     .TIMESTAMPLTZ
                     .TIMESTAMPTZ
                     .VARCHAR2
                     .XMLTYPE

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

v 0.2.0 - updated to support node v 0.10.0, might not be backward compatible

v 0.2.1 - changed Runtime Library for Release Mode to 'MultiThreadedDLL'. This was causing it not to run on release machines. Now fixed! Unit tests added.

v 0.2.2 - Readme.md fixes
## Licence

Copyright (c) 2013 Hubert Boma Manilla  
Licensed under the MIT license.
=======
