Overview
========
This is the current implementation of `check50`, written in node.js. Right now,
there are three files in this directory: `check.js`, `execution.js`, and
`server.js`. We explain each of these in turn below.


`check.js`
==========
This is the main entry point to the library. Clients of the library need to
`require("./check.js")` and create a new `Check50` object a là:

	$ var check50 = new Check50(new Run50(<run50-host>, <run50-port>));

The argument to the `Check50` constructor, in the above case an instance of a
`Run50` object, is an execution manager object (defined in `execution.js`) that
controls the actual source code compilation and execution on a local or remote
(e.g. `run50`) machine/cloud.

From there, clients leverage the `.check(sourceCode, testIds, callback)` method
to run the specified tests on the provided source code.

The `Check50` object is responsible for loading the test configuration files,
combining test configuration with source code to create actual runnable tests,
shipping the tests to the execution manager in the appropriate order/way, and
parsing the execution manager's output (`stdout`, `stderr`, and exit code).
Where possible, the `Check50` object should delegate responsibility to other
objects (e.g. the `Run50` execution manager in `execution.js` manages the
interface with the `run50` API; there should also be an `OutputParser`-type
object to manage output parsing).

Currently the `Check50` object is designed to be a singleton; that is, a client
of the library should only need to instantiate one instance of the library since
it loads all test configuration files into memory at creation in order to avoid
hitting the filesystem each time `.check` is called. This is helpful for clients
such as servers, since all they have to do is instantiate the library on
startup, and then whenever a new request is received, the client needs only send
a message to library via the `.check` method.


`execution.js`
==============
This file defines the execution manager that controls how the untrusted source
code is executed. Right now this just defines and implements a `Run50` object
that wraps the `run50` API. Clients of the `check50` library need an instance
of an execution manager in order to instantiate the `Check50` object, so clients
should also `require("./execution.js")`. Eventually we'll probably want to have
other types of execution managers, so this file could evolve.


`server.js`
===========
This file is a simple server client of the `check50` library that uses the
`Express` module. It mimics the `run50` server's behavior. We'll also want to
have a terminal client of the library so that we can have TFs and students run
`check50` locally.