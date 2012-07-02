<?php

/**
 * @file
 * 
 * Module for parsing output and determining conformance to 
 *
 * @author Nate Hardison <nate@cs.harvard.edu>
 * @package CS50
 * @version 0.1
 */

namespace CS50\Check50\TestScorer;

/**
 * Copies contents of $str into a temporary file in the system /tmp directory.
 * Appends a newline to the file to keep diff happy. Returns the name of the
 * temp file.
 *
 * @param string $str
 *
 * @return string
 */
function copy_to_temp_file($str)
{
    $tmp_filename = tempnam("/tmp", "");

    $tmp_file = fopen($tmp_filename, "w");
    fwrite($tmp_file, $str);
    fclose($tmp_file);

    return $tmp_filename;
}

/**
 * Diffs strings $a and $b with the options provided in the $options_array.
 * Returns the result.
 *
 * @param string $a
 * @param string $b
 * @param array|string $options_array
 *
 * @return string
 */
function diff($a, $b, $options_array)
{
    // php diff libs don't coalesce whitespace, so use UNIX's diff
    $tmp_a = copy_to_temp_file($a);
    $tmp_b = copy_to_temp_file($b);

    $result = shell_exec("diff " . implode(' ', $options_array) . $tmp_a . ' ' . $tmp_b);

    unlink($tmp_a);
    unlink($tmp_b);

    return $result;
}

/**
 *
 * @param string $a
 * @param string $b
 * @param array|string $options_array
 *
 * @return string
 */
function diff_ignore_whitespace($a, $b, $options_array)
{
    return diff($a, $b, array_push($options_array, "-B", "-w");
}



?>
