var AdmZip = require("adm-zip");
var argv = require('optimist').alias('h', 'help').argv;
var fs = require('fs');
var path = require('path');
var _ = require('underscore');
var wrench = require('wrench');

// ensure proper usage
if (argv._.length === 0 || !_.isUndefined(argv.h)) {
    console.log('Usage: check50 path [path ...]');
    process.exit(1);
}

var zip = new AdmZip();

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
        zip.addFile(path.join(suffix, '/'), []); // using addFile because addLocalFile trims leading directory
    }
    else if (stats.isFile()) {
        zip.addFile(suffix, fs.readFileSync(p));
    }

});

