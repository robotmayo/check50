Overview
========
`check50` is CS50's autograding mechanism. It is intended to be used in the
following ways:

* Locally:
	* By TFs grading students' code
	* By students wanting to check their code using our tests

* Remotely:
	* By students checking code that they've written in the browser (using the
	  `run50` Courseware plugin)

`check50` defaults to using the remote `run50` execution environment for running
untrusted code, but in the local scenarios above, it can be configured to
use a local execution environment instead.

Directory Structure
===================
	check50/

		client/           # David wrote this...looks like it's client stuff?


		doc/              # Pics from whiteboarding


		etc/              # Configuration stuff

			tests/        # Test config files


		lib/              # Code lives here

			check50/      # Symlink to current library dir (check50js/ now)

			check50++/    # Nate's C++ fwk; abandoned for now

			check50js/    # Main library files at this point; written in node

			check50php/   # Not very well developed; ignore

			clang/        # Clang plugin stuff