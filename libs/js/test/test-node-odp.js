var odp = require('../../build/Release/odpnode'),
	connectString = 'Data Source=mhladmin;User Id=/;',
	errorString = 'Data Source=foo;User Id=/;',
	invalidString = 'blafoofoo';

	module.exports.testOracleConnection = {
		'Valid connection': function(test){
			test.ok(new odp.OracleConnection(connectString), 'This assertion should pass');
			test.done();
		},
		'Invalid connection string': function(test){
			test.throws(function(){ new odp.OracleConnection(invalidString); });
			test.done();
		},
		'Connection string is null': function(test){
			test.throws(function(){ new odp.OracleConnection(null); });
			test.done();
		},
		'Connection string is an integer': function(test){
			test.throws(function(){ new odp.OracleConnection(4); });
			test.done();
		},
		'Connection string is undefined': function(test){
			test.throws(function(){ new odp.OracleConnection(undefined); });
			test.done();
		}
	};

	module.exports.testOracleConnection_open = {
		setUp: function(callback){
			this.connect = null;
			callback();
		},
		'Error in the connection string': function(test){
			this.connect = new odp.OracleConnection(errorString);
			test.throws(function(){ this.connect.open(); });
			test.done();
		},
		'Connection open ok': function(test){
			this.connect = new odp.OracleConnection(connectString);
			test.equal(this.connect.open(), undefined, 'This assertion should pass');
			test.done();
		}	
	};

	module.exports.testOracleCommand = {
		setUp: function(callback){
			this.cmdText = "SELECT * FROM DUAL;";
			this.cmdType = odp.OracleCommand.commandType.TEXT;
			this.params = undefined;
			this.connect = new odp.OracleConnection(connectString);
			this.connect.open();
			callback();
		},
		tearDown: function(callback){
			this.cmdText = null;
			this.cmdType = null;
			this.params = null;
			this.connect = null;
			callback();
		},
		'All parameters specified': function(test){
			this.params = undefined;
			test.equal(typeof new odp.OracleCommand(this.cmdText, this.cmdType, this.connect), typeof {}, 'This assertion should pass');
			test.done();
		}
	};