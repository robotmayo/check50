#!/bin/env nodejs
// 
// This is CS50 Check.
//
// David J. Malan
// malan@harvard.edu
//

// version
var VERSION = 1.1;

// modules
var argv = require('../lib/node_modules/optimist').alias('c', 'checks').alias('h', 'help').alias('v', 'version').argv;
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

// -h, --help
if (argv._.length === 0 || _.isUndefined(argv.c) || !_.isUndefined(argv.h)) {
    console.log('Usage: check50 -c checks path [path ...]');
    process.exit(0);
}

// -v, --version
if (!_.isUndefined(argv.v)) {
    console.log(VERSION);
    process.exit(0);
}

// prepare to union paths
var paths = [];

// iterate over roots
_.each(argv._, function(root) {

    // resolve root to absolute path (so that we can trim longest common prefix)
    root = path.resolve(root);

    // ensure path exists
    if (!path.existsSync(root)) {
        console.log('No such file or directory: ' + root);
        process.exit(1);
    }

    // blacklist / since readdirSyncRecursive fails on it)
    if (_.contains(['/'], root)) {
        console.log('Illegal file or directory: ' + root);
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
    console.log('Nothing to check');
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
        zip.file(suffix, fs.readFileSync(p).toString());
    }

});

// check!
async.waterfall([

    // POST /upload
    function(callback) {

        // upload ZIP
        process.stdout.write('Uploading.');
        var buffer = new Buffer(zip.generate({ base64: false, compression:'DEFLATE' }), 'binary');
        var interval = setInterval(function() {
            process.stdout.write('.');
        }, 500);
        request.post({
         body: buffer,
         headers: {
          'Content-Length': buffer.length,
          'Content-Type': 'application/zip',
          'Content-Transfer-Encoding': 'binary'
         },
         uri: 'https://sandbox.cs50.net/upload'

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
                    process.stdout.write(' Uploaded.\n');
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
        process.stdout.write('Checking.');
        var interval = setInterval(function() {
            process.stdout.write('.');
        }, 500);
        request.post({
         form: {
          checks: argv.c,
          homedir: id
         },
         headers: {
          'Content-Type': 'application/json'
         },
         uri: 'https://sandbox.cs50.net/check'

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
                    return callback(new Error(payload.error.message));
                }
                else if (_.isUndefined(payload.id) || _.isUndefined(payload.results)) {
                    return callback(new Error('invalid response from server'));
                }
                else {
                    process.stdout.write(' Checked.\n');
                    return callback(null, payload.id, payload.results);
                }
            }

        });

}], function(err, id, results) {

    // report results
    if (err !== null) {
        process.stdout.write(' An error occurred.\n');
        process.exit(1);
    }
    else {

        // iterate over checks
        for (check in results) {

            // report passed check in green
            if (results[check].result === true) {
                process.stdout.write('\033[32m'); // green
                process.stdout.write(':) ' + results[check].description + '\n');
                process.stdout.write('\033[39m'); // default
            }

            // report failed dependency in yellow
            else if (results[check].result === null) {
                process.stdout.write('\033[33m'); // yellow
                process.stdout.write(':| ' + results[check].description + '\n');
                process.stdout.write('\033[39m'); // default
            }

            // report failed check in red
            else {
                process.stdout.write('\033[31m'); // red
                process.stdout.write(':( ' + results[check].description + '\n');
                process.stdout.write('\033[39m'); // default
            }

        }
        process.stdout.write('https://check.cs50.net/?check=' + id + '\n');
    }

    // This was CS50 Check.
    process.exit(0);

});
