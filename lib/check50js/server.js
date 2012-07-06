// required node modules
var argv    = require("optimist").usage("Usage: $0 -p port").demand(['p']).argv,
	express = require("express");

// load in Check50
Check50 = require("./check.js");
Run50 = require("./execution.js");
var check50 = new Check50(new Run50('run-1242295192.us-east-1.elb.amazonaws.com', 80));

// configure the server
var server = express.createServer();
server.configure(function() {
 	server.use(express.bodyParser({ keepExtensions: true }));
  	server.use(express.methodOverride());
  	server.use(express.limit("1mb"));
  	server.use(server.router);
});
server.configure("development", function() {
  	server.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
});
server.configure("production", function() {
  	server.use(express.errorHandler());
});
server.set("view options", {
 	layout: false
});

// listen for connections on port p
server.listen(argv.p);

// POST /check
server.post("/check", function(request, response) {

	// enable CORS
	response.header("Access-Control-Allow-Origin", '*');
	response.header("Access-Control-Allow-Methods", "POST");
	response.header("Access-Control-Allow-Headers", "Content-Type, Authorization");

	// need to handle files and stuff
	// (see run50 code for how-to)
	var sourceCode = {
		fileName: request.body.fileName,
		sourceCode: request.body.sourceCode
	};

	if (!sourceCode.fileName || !sourceCode.sourceCode) {
		response.json({
			error: "Bad source code specification!"
		}, 404);
	}

	var testIds = request.body.testIds;
	if (!testIds || !testIds instanceof Array) {
		response.json({
			error: "Bad test ID specification!"
		}, 404);
	}

	check50.check(sourceCode, testIds, function(error, results) {
		if (error) {
			response.json({
				error: error
			}, 500);
		} else {
			response.json(results, 200);
		}
	});
});