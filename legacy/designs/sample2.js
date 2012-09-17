CS50.Check.define('cs50.2011.pennies', function() {

    CS50.Check.check('Does it compile?', 1, function(score) {

        // compile program
        var clang = CS50.Check.run(homedir, 'clang -std=c99 -Wall -Werror hello.c -o hello', null);

        // ensure exit code is 0
        if (clang.code === 0) {
            return { score: 1 }:
        else
            return CS50.Check.score(0, clang.stderr);

    });

    CS50.Check.check('Correct Style', 3, CS50.Check.lint);

});
