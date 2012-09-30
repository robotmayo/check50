// 
// This is CS50 Check.
//
// David J. Malan
// malan@harvard.edu
//

var AdmZip = require("adm-zip");
var argv = require('optimist').alias('h', 'help').argv;
var fs = require('fs');
var http = require('http');
var https = require('https');
var path = require('path');
var request = require('request');
var _ = require('underscore');
var wrench = require('wrench');

// ensure proper usage
if (argv._.length === 0 || !_.isUndefined(argv.h)) {
    console.log('Usage: check50 path [path ...]');
    process.exit(1);
}

// create ZIP
//var zip = new AdmZip();

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

var zip = new require('node-zip')();

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
        //zip.addFile(path.join(suffix, '/'), []); // using addFile because addLocalFile trims leading directory
        zip.folder(path.join(suffix, '/'));
    }
    else if (stats.isFile()) {
        //zip.addFile(suffix, fs.readFileSync(p));
        zip.file(suffix, fs.readFileSync(p).toString('binary'));
        //zip.file(suffix, "here");
    }
    console.log('added: ' + suffix);

});

//var s = zip.toBuffer().toString();

var data = zip.generate({base64:false,compression:'DEFLATE'});
fs.writeFile('test.zip', data, 'binary');

/*
var request = http.request({
 headers: {
  'Content-Length': s.length,
  'Content-Type': 'Content-type: application/zip',
  'Content-Transfer-Encoding': 'binary'
 },
 host: '192.168.74.135',
 port: 8080,
 path: '/upload',
 method: 'POST'
}, function(response) {

    var chunks = [];
    response.on('data', function(chunk) {
        chunks.push(chunk.toString());
    });
    response.on('end', function(chunk) {
        console.log(chunks.join(''));
    });

});

request.on('error', function(err) {
    console.log(err);
});
*/

/*
zip.writeZip('test.zip');

request.post({
 body: zip.toBuffer(),
 headers: {
  'Content-Type': 'application/zip',
  'Content-Transfer-Encoding': 'binary'
 },
 uri: 'http://192.168.74.135:8080/upload'
}, function(err, response, body) {

    console.log(body);

});


// TODO: handle ECONNREFUSED

//request.write(s);
//request.end();
*/
