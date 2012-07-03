// required node modules
var async = require("async"),
    fs    = require("fs"),
    http  = require("http"),
    qs    = require("querystring");

exports = module.exports = Run50;

function Run50(host, port) {
    this.host = host;
    this.port = port;
}

Run50.prototype.post = function(data, path, callback) {
    var options = {
        host: this.host,
        port: this.port,
        path: path,
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
            'Content-Length': data.length
        }
    };

    var request = http.request(options, function(response) {
        response.setEncoding('utf8');

        var body = '';
        response.on('data', function(chunk) {
            body += chunk;
            // Too much data from run50
            if (body.length > 1e6) {
                response.connection.destroy();
                console.log("Too much data!");
                callback("Too much data", null);
            }
        });

        response.on('end', function() {
            callback(null, JSON.parse(body));
        });
    });

    request.on('error', function(e) {
        console.log("Error establishing connection to run50!");
        callback("Error establishing connection to run50", null);
    });

    request.write(data);
    request.end();
};

Run50.prototype.execute = function(sourceCode, test, callback) {
    var self = this;
    async.waterfall([
        // upload
        function(callback) {
            var data = qs.stringify(sourceCode);
            self.post(data, "/upload", callback);
        },

        // run
        function(token, callback) {
            var data = qs.stringify({
                cmd: test.run,
                homedir: token.id
            });
            self.post(data, "/run", callback);
        }
        // post-execute
    ], function(error, results) {
        console.log(results);
        callback(error, results);
    });        
};