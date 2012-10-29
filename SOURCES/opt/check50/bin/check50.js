#!/bin/env nodejs
// 
// This is CS50 Check.
//
// David J. Malan
// malan@harvard.edu
//

// version
var VERSION = '1.12';

// endpoint
var ENDPOINT = 'https://sandbox.cs50.net';

// modules
var argv = require('../lib/node_modules/optimist').boolean(['d','h','v']).alias('d', 'debug').alias('h', 'help').alias('v', 'version').argv;
var async = require('../lib/node_modules/async');
var child_process = require('child_process');
var fs = require('fs');
var http = require('http');
var https = require('https');
var JSZip = require('../lib/node_modules/node-zip');
var path = require('path');
var request = require('request');
var _ = require('../lib/node_modules/underscore');
var wrench = require('../lib/node_modules/wrench');

// -v, --version
if (argv.version === true) {
    process.stdout.write(VERSION + '\n');
    process.exit(0);
}

// -h, --help
if (argv.help === true) {
    process.stdout.write('Usage: check50 checks path [path ...]\n');
    process.exit(0);
}
else if (argv._.length < 2) {
    process.stderr.write('Usage: check50 checks path [path ...]\n');
    process.exit(1);
}

// prepare to union paths
var paths = [];

// iterate over roots
_.each(argv._.slice(1), function(root) {

    // resolve root to absolute path (so that we can trim longest common prefix)
    root = path.resolve(root);

    // ensure path exists
    if (!path.existsSync(root)) {
        process.stderr.write('No such file or directory: ' + root + '\n');
        process.exit(1);
    }

    // blacklist / since readdirSyncRecursive fails on it)
    if (_.contains(['/'], root)) {
        process.stderr.write('Illegal file or directory: ' + root + '\n');
        process.exit(1);
    }

    // stat root
    var stats = fs.statSync(root);

    // file
    if (stats.isFile()) {
        paths = _.union(paths, [root]);
    }

    // directory
    else if (stats.isDirectory()) {

        // recurse into root, prepending root, avoiding duplicates
        paths = _.union(paths, root, _.map(wrench.readdirSyncRecursive(root), function(descendant) {
            return path.join(root, descendant);
        }));

    }

});

// ensure a path exists
if (paths.length === 0) {
    process.stderr.write('Nothing to check' + '\n');
    process.exit(1);
}

// sort paths so that parent directories are created before children
paths.sort();

// find paths' longest common prefix
// http://stackoverflow.com/questions/1916218/find-the-longest-common-starting-substring-in-a-set-of-strings/1917041#1917041
var prefix = new RegExp('^' + (function() {
    var first = path.join(path.dirname(paths[0]), '/');
    var length = first.length;
    var last = path.join(path.dirname(paths[paths.length - 1]), '/');
    while (length > 0 && last.indexOf(first) === -1) {
        first = first.substring(0, --length);
    }
    return first.replace(/([.?*+^$[\]\\(){}|-])/g, "\\$1");
})());

// create ZIP
var zip = new JSZip();

// iterate over paths
if (argv.debug === false) {
    process.stdout.write('Compressing...');
}
_.each(paths, function(p) {

    // trim prefix
    var suffix = p.replace(prefix, '');

    // ignore common parent
    if (suffix.length === 0) {
        return;
    }

    // add path to ZIP
    var stats = fs.statSync(p);
    if (stats.isDirectory()) {
        zip.folder(path.join(suffix, '/'));
    }
    else if (stats.isFile()) {
        try {
            zip.file(suffix, fs.readFileSync(p).toString());
        }
        catch (e) {
            process.stderr.write(' Error.\n');
            switch (e.code) {

                case 'EACCES':
                    process.stderr.write('could not read file'+ '\n');
                    break;
            }
            process.stderr.write('\n');
            process.exit(1);
        }
    }

});
if (argv.debug === false) {
    process.stdout.write(' Compressed.\n');
}

// check!
async.waterfall([

    // POST /upload
    function(callback) {

        // upload ZIP
        if (argv.debug === false) {
            process.stdout.write('Uploading...');
        }
        var buffer = new Buffer(zip.generate({ base64: false, compression:'DEFLATE' }), 'binary');
        var interval = setInterval(function() {
            if (argv.debug === false) {
                process.stdout.write('.');
            }
        }, 500);
        request.post({
         body: buffer,
         headers: {
          'Content-Length': buffer.length,
          'Content-Type': 'application/zip',
          'Content-Transfer-Encoding': 'binary'
         },
         uri: ENDPOINT + '/upload'

        }, function(err, response, body) {

            // handle response
            clearInterval(interval);
            if (err === null) {

                // parse body
                var payload;
                try {
                    payload = JSON.parse(body);
                }
                catch (e) {
                    return callback(new Error(e));
                }
                if (!_.isUndefined(payload.id)) {
                    if (argv.debug === false) {
                        process.stdout.write(' Uploaded.\n');
                    }
                    return callback(null, payload.id);
                }
                else if (!_.isUndefined(payload.error)) {
                    callback(new Error(payload.error));
                }
                else {
                    return callback(new Error('Invalid response from server'));
                }

            }
            else {
                return callback(err);
            }

        });
    },

    // POST /check
    function(id, callback) {

        // run checks
        if (argv.debug === false) {
            process.stdout.write('Checking...');
        }
        var interval = setInterval(function() {
            if (argv.debug === false) {
                process.stdout.write('.');
            }
        }, 500);
        request.post({
         form: {
          checks: argv._[0],
          homedir: id
         },
         headers: {
          'Content-Type': 'application/json'
         },
         uri: ENDPOINT + '/check'

        }, function(err, response, body) {

            // handle response
            clearInterval(interval);
            if (err !== null) {
                callback(err);
            }
            else {

                // parse body
                var payload;
                try {
                    payload = JSON.parse(body);
                }
                catch (e) {
                    return callback(e);
                }
                if (!_.isUndefined(payload.error)) {
                    return callback(payload.error);
                }
                else if (_.isUndefined(payload.id) || _.isUndefined(payload.results)) {
                    return callback(new Error('invalid response from server'));
                }
                else {
                    if (argv.debug === false) {
                        process.stdout.write(' Checked.\n');
                    }
                    return callback(null, payload.id, payload.results);
                }
            }

        });

}], function(err, id, results) {

    // report results
    if (err !== null) {
        process.stderr.write(' Error.\n');
        switch (err.code) {

            case 'ECONNREFUSED':
                process.stderr.write('could not reach server' + '\n');
                break;

            case 'ECONNRESET':
                process.stderr.write('connection to server died' + '\n');
                break;

            case 'E_USAGE':
                process.stderr.write(err.message + '\n');
                break;

            default:
                process.stderr.write('unknown\n');
                process.stderr.write(err.code + '\n');
        }
        process.exit(1);
    }
    else {

        // -d, --debug
        if (argv.debug === true) {
            process.stdout.write(JSON.stringify(results, undefined, '  '));
            process.exit(0);
        }

        // iterate over checks
        for (check in results) {

            // report passed check in green
            if (results[check].result === true) {

                // :)
                process.stdout.write('\033[32m'); // green
                process.stdout.write(':) ' + results[check].description + '\n');
                process.stdout.write('\033[39m'); // default

            }

            // report failed dependency in yellow
            else if (results[check].result === null) {

                // :|
                process.stdout.write('\033[33m'); // yellow
                process.stdout.write(':| ' + results[check].description + '\n');
                process.stdout.write('\033[39m'); // default
                process.stdout.write('   \\ can\'t check until a frown turns upside down\n');

            }

            // report failed check in red
            else {

                // :( 
                process.stdout.write('\033[31m'); // red
                process.stdout.write(':( ' + results[check].description + '\n');
                process.stdout.write('\033[39m'); // default

                // check for error
                if (!_.isUndefined(results[check].error)) {
                    process.stdout.write('   \\ ' + results[check].error + '\n');
                    continue;
                }
 
                // check for script
                if (_.isUndefined(results[check].script)) {
                    continue;
                }

                // mismatch is always at end of script
                var script = results[check].script[results[check].script.length - 1];

                // explain mismatch
                if (!_.isUndefined(script.expected)) {
                    switch (script.expected.type) {

                        // diff
                        case 'diff':
                            
                            // TEMP
                            if (script.expected.value === null && script.actual.value === null) {
                                process.stdout.write('   \\ binary files differed\n');
                                break;
                            }

                            var expected = script.expected.value.replace(/\n$/, '').split(/\n/);
                            var actual = script.actual.value.replace(/\n$/, '').split(/\n/);
                            for (var i = 0; i < expected.length; i++) {
                                if (i >= actual.length || expected[i] !== actual[i]) {
                                    var string = JSON.stringify(expected[i]);
                                    var substring = string.substring(0, 40);
                                    process.stdout.write('   \\ expected ' + substring + ' on line ' + (i + 1) + '\n');
                                    break;
                                }
                            }
                            if (actual.length > expected.length) {
                                var string = JSON.stringify(actual[expected.length]);
                                var substring = string.substring(0, 40);
                                process.stdout.write('   \\ wasn\'t expecting ' + substring + ' on line ' + (expected.length + 1) + '\n');
                            }
                            break;

                        // exists
                        case 'exists':
                            process.stdout.write('   \\ expected ' + script.expected.value + ' to exist\n');
                            break;

                        // exit, stderr, stdin, stdout
                        default:
                            if (!_.isUndefined(script.actual)) {
                                switch (script.actual.type) {

                                    // exit
                                    case 'exit':
                                        process.stdout.write('   \\ wasn\'t expecting an exit code of ' + script.actual.value + '\n');
                                        break;

                                    // stdin
                                    case 'stdin':
                                        process.stdout.write('   \\ wasn\'t expecting to be prompted for input\n');
                                        break;

                                    // stderr, stdout
                                    case 'stderr':
                                    case 'stdout':

                                        // display up to 40 characters
                                        var string = JSON.stringify(script.actual.value);
                                        var substring = string.substring(0, 40);
                                        process.stdout.write('   \\ wasn\'t expecting ' + substring);
                                        if (substring.length < string.length) {
                                            process.stdout.write('..."');
                                        }
                                        if (script.actual.type === 'stderr') {
                                            process.stdout.write(' on stderr\n');
                                        }
                                        process.stdout.write('\n');
                                        break;

                                }
                            }
                    }

                }
                else if (!_.isUndefined(script.actual)) {
                    switch (script.actual.type) {

                        // signal
                        case 'signal':
                            if (script.actual.value === 'SIGKILL') {
                                process.stdout.write('   \\ took too long to run\n');
                            }
                            break;
                    }
                }
            }
        }

        // diagnostics
        process.stdout.write('https://check.cs50.net/?check=' + id + '\n');
    }

    // This was CS50 Check.
    process.exit(0);

});
