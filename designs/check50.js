// load modules
var 
 argv = require('optimist').usage('Usage: $0').demand([]).argv, // https://github.com/substack/node-optimist
 async = require('async'), // https://github.com/caolan/async
 child_process = require('child_process'),
 fs = require('fs'),
 iniparser = require('iniparser'),
 $ = require('jquery'),
 path = require('path'),
 _ = require('underscore'),
 uuid = require('node-uuid');

var homedir = CS50.Check.upload('/path/to/files');
var cmd = CS50.Run.run(homedir, 'clang -ggdb -std=c99 -Wall -Werror    pennies.c  -lcrypt -lcs50 -lm -o pennies', null);

//
async.parallel([

    // 
    function(callback) {

        assertZero(cmd.code);

    },

    // 
    function(callback) {

        assertEquals(cmd.stdout, 'hello, world\n');

    }

],

//
function(err, result) {
    // TODO
});


///////////////////////

var scores = [];

// - does it compile?
// - run with stdin. match exitcode || (match last line || all lines || regex)
// - run 

var results = run('clang -ggdb -std=c99 -Wall -Werror    pennies.c  -lcrypt -lcs50 -lm -o pennies', null);
assertEquals(results.code, 0);
assertEquals(results.stdout, someString);
assertRegEx(regex, results.stdout);


if (results.code !== 0) {
    scores.push({ score: -1
     score: 0,
     stderr: string
    };
}

var student = run('./pennies', '30\n1');
var staff = run('./pennies', '30\n1');
if (student.stdout !== staff.stdout) {
    score: 0,
}
