Overview
========
This is the directory for the main PHP library files.

* Check50 is designed to run unit tests on untrusted code. Clients can choose to
run either a whole test suite or a subset thereof.
* Current thinking is that tests will be stored hierarchically on the filesystem
so that a parent dir like `etc/pset1` will constitute a test suite where the
individual tests are stored as files like `etc/pset1/pennies-basic` and
`etc/pset1/pennies-negative-days`. Users can specify paths and Check50 will run
all the tests under that path.
* Parsing the test files is the job of the TestBuilder. Right now the hope is to
be able to specify tests just like you'd run them on the command line (e.g.
`pennies < 32\n1`) or as a function call (e.g. `oldman:print_verse(3)`).
However, I'd like to make the parsing part of the TestBuilder modular so that we
can plug in different parsers if we find a more convenient specification.
* The Run50API defines the Check50 interface to any run50 API wrappers. This
should remain constant and we should plug in different wrappers as needed.
* The TestScorer takes the output of run50 and does any needed output
comparisons and then emits the appropriate numeric score plus explanation.