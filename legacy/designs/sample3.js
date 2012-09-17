var cs50 = require('cs50');

...

var check50 = new CS50.Check({ endpoint: 'http://run.cs50.org' });

// compile program
var clang = check50.run(homedir, 'clang -std=c99 -Wall -Werror say.c -lcs50 -o say', null);

// ensure exit code is 0
CS50.Check.assertZero(clang.code, stderr);

CS50.Check.parallel([

    function() {

        // run program with 'monkey' as stdin
        var say = CS50.Check.run(homedir, './say', 'monkey');

        // ensure no stderr
        CS50.Check.assertEquals(say.stderr, null);

        // ensure correct stdout
        CS50.Check.assertEquals(say.stdout, 'hello, monkey');

    },

    function() {

        // run program with 'banana' as stdin
        var say = CS50.Check.run(homedir, './say', 'banana');

        // ensure no stderr
        CS50.Check.assertEquals(say.stderr, null);

        // ensure correct stdout, ignoring case
        CS50.Check.assertRegExp(say.stdout, /hello, banana/i);

    }
]);
