/*SELECT m.FOLDERID,m.ZONE.ZONECODE,m.FOLDERPATH.PATH,m.FOLDERPATH.DATELASTMODIFIED,s.MODELEDLOD.LODCODE,
s.MODELEDLOD.LODLABEL, s.BUILDINGSTATUS.BUILDINGSTATUSLABEL FROM MID.MIFOLDER2 m,MID.MIVERSION 
s WHERE m.VERSION.VERSIONID = s.VERSIONID AND  m.ZONE.ZONECODE ='chwhrf-bldg'*/

var odpnode = require('../build/Release/odpnode.node');
var os = require('os');
var con = new odpnode.OracleConnection("Data Source=mhladmin;User Id=/;");
var parameterDirection = odpnode.OracleData.parameterDirection;
var datatypes = odpnode.OracleData.dbType;
var commandType = odpnode.OracleCommand.commandType;
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
var p = [
	{
		name:"nResult",
		type: datatypes.INT32,
		direction: parameterDirection.OUTPUT
	},
	{
		name:"pString",
		type: datatypes.VARCHAR2,
		size: 1000,
		value: "400",
		direction: parameterDirection.INPUT
	},
	
]

var prms = [
		{
			name: 'pSERVERID',
			type: datatypes.INT32,
			value: 303,
			direction: parameterDirection.INPUT
		},{
			name: 'pBSSTATUSCODE',
			type: datatypes.VARCHAR2,
			size: 2000,
			value: "waiting",
			direction: parameterDirection.INPUTOUTPUT
		},{
			name: 'pBSSTATUSDESCRIPTION',
			type: datatypes.VARCHAR2,
			size: 2000,
			direction: parameterDirection.INPUTOUTPUT
		}
	];
con.open();
var cmd = new odpnode.OracleCommand("BEGIN :vResult := MID.MI_MAXSCRIPT.MITest( :pFOLDERID ); END;", commandType.TEXT, parameters, con);
console.log("---- Execute Non-Query ------");
cmd.executeNonQuery(function(err, rowsAffected, parameters){
	if(err){
		console.log(err);
	}else{
		console.log(parameters);
	}	
});
console.log("---- Execute Reader ------");
var cmd1 = new odpnode.OracleCommand("SELECT m.FOLDERID,m.ZONE.ZONECODE FROM MID.MIFOLDER2 m,MID.MIVERSION s WHERE m.VERSION.VERSIONID = s.VERSIONID AND  m.ZONE.ZONECODE ='chwhrf-bldg'", commandType.TEXT, con);
cmd1.executeReader(function(err, rowset){
	if(err){
		console.log(err);
	}else{
		console.log(rowset);
		//console.log(rowset[0]["ZONE_ZONECODE"]);
	}	
});
console.log("---- Execute Non-Query (Functions) ------");
var cmd2 = new odpnode.OracleCommand("BEGIN :nResult := MID.MI_MAXSCRIPT.MITest2( :pString ); END;", commandType.TEXT, p, con);
cmd2.executeNonQuery(function(err, rowsAffected, parameters){
	if(err){
		console.log(err);
	}else{
		console.log(parameters.pString);
	}	
});
console.log("---- Register Server ------");
var regServerCommand = new odpnode.OracleCommand('BEGIN BMD.SERVER_API.setServerStatus(:pSERVERID, :pBSSTATUSCODE, :pBSSTATUSDESCRIPTION); END;',commandType.TEXT, prms, con)
	regServerCommand.executeNonQuery(function(err, rowsAffected, parameters){
		if(err){
			console.log(err);
		}else{
			console.log(parameters);
		}
	});