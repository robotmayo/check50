#!/bin/env nodejs
// 
// This is CS50 Check.
//
// David J. Malan
// malan@harvard.edu
//

// version
var VERSION = 1;

// modules
var argv = require('optimist').alias('c', 'checks').alias('h', 'help').alias('v', 'version').argv;
var async = require('async');
var child_process = require('child_process');
var fs = require('fs');
var http = require('http');
var https = require('https');
var JSZip = require('node-zip');
var path = require('path');
var request = require('request');
var _ = require('underscore');
var wrench = require('wrench');

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
         uri: 'http://192.168.74.135:8080/upload'

        }, function(err, response, body) {

            // handle response
            clearInterval(interval);
            if (err === null) {
                var response = JSON.parse(body);
                if (!_.isUndefined(response.id)) {
                    process.stdout.write(' Uploaded.\n');
                    callback(null, response.id);
                }
                else if (!_.isUndefined(response.error)) {
                    callback(new Error(response.error));
                }
                else {
                    callback(new Error('Invalid response from server'));
                }
            }
            else {
                callback(err);
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
         uri: 'http://192.168.74.135:8080/check'

        }, function(err, response, body) {

            // handle response
            clearInterval(interval);
            if (err !== null) {
                callback(err);
            }
            else {

                // parse response
                var response;
                try {
                    response = JSON.parse(body);
                }
                catch (e) {
                    callback(e);
                }
                if (!_.isUndefined(response.error)) {
                    callback(new Error(response.error.message));
                }
                else if (/*_.isUndefined(response.id) || */ _.isUndefined(response.results)) {
                    callback(new Error('invalid response from server'));
                }
                else {
                    process.stdout.write(' Checked.\n');
                    callback(null, response.results);
                }
            }

        });

}], function(err, results) {

    if (err !== null) {
        process.stdout.write(' Error: ' + err.message + '.\n');
        process.exit(1);
    }
    else {
        for (check in results) {
            if (results[check].result === true) {
                process.stdout.write('\033[32m');
                process.stdout.write(':-) ' + results[check].description + '\n');
                process.stdout.write('\033[39m');
            }
            else {
                process.stdout.write('\033[31m');
                process.stdout.write('    ' + results[check].description + '\n');
                process.stdout.write('\033[39m');
            }
            console.log(results[check].result);
        }
    }

});
