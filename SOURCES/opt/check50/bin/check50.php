#!/bin/env php
<?php

    // 
    $archive = new ZipArchive();
    $filename = tempnam(sys_get_temp_dir(), '');
    if ($archive->open($filename, ZIPARCHIVE::CREATE) === false) {
        // fail
    }

    // iterate over arguments
    $paths = array();
    for ($i = 1, $n = count($argv); $i < $n; $i++) {

        // file
        if (is_file($argv[$i])) {
            $paths[] = realpath($argv[$i]);
        }

        // directory
        else if (is_dir($argv[$i])) {

            // http://stackoverflow.com/questions/10609805/php-recursively-delete-empty-directories-with-spl-iterators
            $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($argv[$i], FilesystemIterator::SKIP_DOTS), RecursiveIteratorIterator::SELF_FIRST);
            foreach ($iterator as $file) {
                $paths[] = realpath($file->getPathname());
            }

        }
    }
    if (count($paths) === 0) {
        // fail
    }

    // compute length of longest common prefix
    // TODO: change _commonPath to return dirname for $paths[0] when count($paths) == 1
    $prefix = strlen((count($paths) == 1) ? dirname($paths[0]) : _commonPath($paths));

    // add directories and files to ZIP, trimming prefix
    for ($i = 0, $n = count($paths); $i < $n; $i++) {
        print("Compressing " . ($i+1) . " of $n...\r");
        if (is_dir($paths[$i])) {
            $archive->addEmptyDir(substr($paths[$i], $prefix+1));
        }
        else if (is_file($paths[$i])) {
            $archive->addFile($paths[$i], substr($paths[$i], $prefix+1));
        }
    }
    print("\n");

    // close ZIP
    $archive->close();

    // POST /upload
    $file = file_get_contents($filename);
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_HEADER, false);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
     'Content-length: ' . strlen($file),
     'Content-type: application/zip',
     'Content-Transfer-Encoding: binary'
    ));
    curl_setopt($ch, CURLOPT_POST, true);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $file);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    //curl_setopt($ch, CURLOPT_URL, 'https://sandbox.cs50.net/upload');
    curl_setopt($ch, CURLOPT_URL, 'http://localhost:8080/upload');
    $response = json_decode(curl_exec($ch));
    print_r($response);


    // adapted from http://rosettacode.org/wiki/Find_common_directory_path#PHP
    function _commonPath($paths)
    {
        $arr = array();
        foreach ($paths as $i => $path)
        {
            $paths[$i] = explode('/', $path);
            unset($paths[$i][0]);
            $arr[$i] = count($paths[$i]);
        }
        $min = min($arr);
        for ($i = 0, $n = count($paths); $i < $n; $i++)
        {
            while (count($paths[$i]) > $min)
                array_pop($paths[$i]);
            $paths[$i] = '/' . implode('/', $paths[$i]);
        }
        $paths = array_unique($paths);
        while (count($paths) !== 1)
        {
            $paths = array_map('dirname', $paths);
            $paths = array_unique($paths);
        }
        reset($paths);
        return current($paths);
    }

?>
