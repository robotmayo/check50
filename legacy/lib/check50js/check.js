// pull in our required node modules
var async = require("async"),
    fs    = require("fs"),
    yaml  = require("js-yaml");

// define our exports
exports = module.exports = Check50;

function Check50(executionManager) {

    this.executionManager = executionManager;

    this.tests = {};

    var self = this;

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
            
            if (test.run instanceof Array)
                test.run = test.run.join(';');
            
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
            self.tests[file] = test;
        });
    }

    // function for recursively walking a directory, adding each "*.yml" file to 
    // the tests object above
    function walk(dir) {
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
    walk('../../etc/tests');
};

Check50.prototype.check = function(sourceCode, testIds, callback) {
    var results = [];
    var self = this;
    async.forEachSeries(testIds, function(testId, callback) {
        var test = self.tests[testId];
        if (typeof test !== 'undefined') {
            self.executionManager.execute(sourceCode, test, function(error, output) {
                if (!error) {
                    results.push(self.parseOutput(output, test));
                }
                // since we already tested error, it's okay to just pass it in here
                callback(error);
            });
        } else {
            callback("Invalid test ID " + testId);
        }
    }, function(error) {
        console.log(results);
        callback(error, results);
    });
};

// TODO
Check50.prototype.parseOutput = function(output, test) {
    var result = { 
        pointsReceived: test.points,
        explanation: "Congratulations!"
    };
   return result;
}
