<?php

namespace cs50\check50;

/* initialization code here */

spl_autoload_register(function($name) {
    /* autoloading here */
});

/**
 * This class is the main entry point for Check50. Clients should load this file
 * with require_once and get an instance of this to then interact with the rest
 * of the library.
 */
class Check50
{

    /**
     * Filename of source code to check.
     *
     * @var string $_src_code_filename
     */
    private $_src_code_filename;

    /**
     * Creates an instance of Check50 to run on the provided source code.
     *
     * @param string src_code_filename
     */
    public function __construct($src_code_filename)
    {
        $this->_src_code_filename = $src_code_filename;
    }

    /**
     * Runs Check50. 
     *
     * @param string test_suite
     * @param array tests
     *
     * @return undecided
     */
    public function check($test_suite, array tests = NULL)
    {
        // Check what tests have been specified
        // Pass code and tests to TestBuilder
        // $tb = TestBuilder($this->src_code_filename);
        // $built_tests = $tb->build_tests(tests);

        // Pass built tests to run50
        // $run50 = Run50Wrapper();
        // $run50->upload($built_tests);
        // $run50->run(function($args){
        //     /* callback */
        // });

        // Pass results to TestScorer
        // $ts = TestScorer();
        // $ts->score();

        // Return results to the user
        // return $something;
    }

}

?>
