#!/bin/env php
<?php

    $archive = new ZipArchive();
    $filename = tempnam(sys_get_temp_dir(), '');
    if ($archive->open($filename, ZIPARCHIVE::CREATE) === false) {
        // fail
    }

    $paths = array();

    // iterate over arguments
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
    $prefix = strlen(_commonPath($paths));

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
    print(strlen($file) . "\n");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_URL, 'http://localhost:8080/upload');
    curl_setopt($ch, CURLOPT_VERBOSE, true);
    print_r(curl_exec($ch));
    print_r(curl_getinfo($ch));



    //  http://rosettacode.org/wiki/Find_common_directory_path#PHP
    function _commonPath($dirList)
    {
        $arr = array();
        foreach($dirList as $i => $path)
        {
            $dirList[$i]    = explode('/', $path);
            unset($dirList[$i][0]);
     
            $arr[$i] = count($dirList[$i]);
        }
     
        $min = min($arr);
     
        for($i = 0; $i < count($dirList); $i++)
        {
            while(count($dirList[$i]) > $min)
            {
                array_pop($dirList[$i]);
            }
     
            $dirList[$i] = '/' . implode('/' , $dirList[$i]);
        }
     
        $dirList = array_unique($dirList);
        while(count($dirList) !== 1)
        {
            $dirList = array_map('dirname', $dirList);
            $dirList = array_unique($dirList);
        }
        reset($dirList);
     
        return current($dirList);
    }

?>
