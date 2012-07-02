var yaml = require("js-yaml");
var fs = require("fs");
var http = require('http');
var qs = require('querystring');

var tests = {};

// parse a test file and add it into the tests object. For now, the object is 
// indexed *just* by the filename (and not the full path), so the filename must 
// be unique across all *.yml files in the tests directory
function parseTest(dir, file) {
    fs.readFile(dir + '/' + file, function(err, data) {
        if (err) {
            console.log('Error reading ' + dir + '/' + file);
            process.exit(2);
        }
        try {
            var test = yaml.load(data);
        }
        catch (e) {
            console.log('Invalid yaml file: ' + dir + '/' + file);
            process.exit(3);
        }

        if (typeof test.run === 'undefined' || test.run === null) {
            console.log (dir + '/' + file + ' must have a "run" directive');
            process.exit(4);
        }
        
        if (!(test.run instanceof Array))
            test.run = [test.run];
        
        if (typeof test.supporting === 'undefined' || test.supporting === null)
            delete test.supporting;
        else {
            if (!(test.supporting instanceof Array))
                test.supporting = [test.supporting];

            // for now, this is synchronous, but since it's done beforehand that 
            // should be fine for now
            var supporting = {};
            test.supporting.forEach(function(support) {
                fs.readFileSync(dir + '/' + support, function(err, data) {
                    if (err) {
                        console.log('Error reading support file "' + dir + '/' + support);
                        process.exit(5);
                    }
                    // have to pay attention to the path of the support file relative 
                    // to the source code on the local vs. run file systems
                    // XXX conflicting filenames / creating directories on run50?
                    supporting[support] = data;
                });
            });
            test.supporting = supporting;
        }
        tests[file] = test;
    });
}

// function for recursively walking a directory, adding each "*.yml" file to 
// the tests object above
var walk = function(dir) {
    var results = [];
    fs.readdir(dir, function(err, list) {
        if (err) {
            console.log('Error reading tests directory');
            process.exit(1);
        }

        list.forEach(function(file) {
            path = dir + '/' + file;
            fs.stat(path, function(err, stat) {
                if (stat && stat.isDirectory())
                    walk(path);
                else if (/\.yml$/.test(file))
                    parseTest(dir, file);
            });
        });
    });
};

// "compile" all tests
// if the number of tests gets too large, then we'll have to do this on-demand.
// But for now, this should be better
walk('../etc/tests');


function parseOutput() {
}

// for now, we just 404 on any unexpected input
http.createServer(function(request, response) {
    // enable CORS
    if (request.method != 'POST') {
        response.writeHead(404);
        response.end();
    }

    response.writeHead(200, {
        'Content-Type': 'text/plain',
        'Access-Control-Allow-Origin': '*',
        'Access-Control-Allow-Methods': 'POST',
        'Access-Control-Allow-Headers': 'Content-Type, Authorization'
    });

    var body = '';
    // read the data from the request into body, cutting the connection if body grows 
    // too large
    request.on('data', function(data) {
            body += data;
            // Too much data
            if (body.length > 1e6)
                request.connection.destroy();    
    });

    // parse the body into the POST object. doesn't support file-upload, 
    // but a zip could be transmitted as just a stream of bytes instead
    // for now, just 404 on any kind of error
    request.on('end', function() {
        var POST = qs.parse(body);
        
        if (typeof POST.sourceCode === 'undefined'
            || typeof POST.testIds === 'undefined') {
            response.writeHead(404);
            response.end();
        }
       
        if (!(POST['testIds[]'] instanceof Array))
            POST['testIds[]'] = [POST['testIds[]']];
       
        // automatically name the file "source" if no name was supplied
        POST.fileName = POST.filename ? POST.fileName : 'source';
        
        POST['testIds[]'].forEach(function(id) {
            // for now, slam all test files and source code in the same dir
            var x = POST['testIds[]'][0];
            var test = tests[POST['testIds[]'][0]];
            var source = {};
            source[POST.fileName] = POST.sourceCode;
            var data = qs.stringify(source);

            if (typeof test.supporting !== 'undefined')
                data = data + '&' + qs.stringify(test.supporting);

            var options = {
                host: 'run50',
                port: '5050',
                path: '/upload',
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                    'Content-Length': data.length
                }
            };

            // setup the connection to run50 to setup the files
            var req = http.request(options, function(res) {
                res.setEncoding('utf8');

                var runEnv = '';
                res.on('data', function(chunk) {
                    runEnv += chunk;
                    // Too much data from run50
                    if (runEnv.length > 1e6) {
                        res.connection.destroy();    
                        response.writeHead(404);
                        response.end();
                    }
                });

                res.on('end', function() {
                    // now we start running the actual commands


                });
            });
        
            req.write(data);
        });
    });

    response.end();
    
    
}).listen('50501'); 

