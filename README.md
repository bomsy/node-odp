
HOW TO USE

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
				var cmd = new odpnode.OracleCommand("BEGIN :vResult := MID.MI_MAXSCRIPT.MITest( :pFOLDERID ); END;", cmdType.TEXT, parameters, con);
				cmd.executeNonQuery(function(err, rowsAffected, params){
					if(err){
						console.log(err);
					}else{
						console.log(params);
					}	
				});

UPDATES

v 0.1.0 - just .node file .

v 0.1.1 - added all the source files, updated binding.gyp.

v 0.1.2 - corrections.

v 0.1.3 - bug fixes.

v 0.1.4 - bug fixes.

v 0.1.9 - added dependency dlls to package msvcp100d.dll, msvcr100d.dll.

v 0.1.10 - compiled to 32 bit node.

v 0.1.11 - complied to x64.

v 0.1.14 - fixed exception handling in odpconnection.cc