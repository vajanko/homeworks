
$rootDir = "C:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\"
$dir = $rootDir + "public-ro\\mlc\\du6\\tests\\"
$debugDir = $rootDir + "public-rw\\SWI098\\du6\\Debug\\"
$exeFile = $debugDir + "mlc.exe"

cd $debugDir

foreach ($sourceFile in [System.IO.Directory]::EnumerateFiles($dir, "*.mls"))
{
    $outFile1 = $sourceFile.Replace("mls", "out")
    $asmFile1 = $sourceFile.Replace("mls", "moa")

    $outFile2 = [System.IO.Path]::GetFileName($outFile1)
    $binFile2 = [System.IO.Path]::GetFileName($binFile1)
    $asmFile2 = [System.IO.Path]::GetFileName($asmFile1)

    & $exeFile $sourceFile $binFile2 "-S$asmFile2" > $outFile2

    # $outFile2 = $debugDir + $outFile2;
    # $asmFile2 = $debugDir + $asmFile2;

    if (Test-Path $outFile1)
    {
        if ([System.IO.File]::ReadAllText($outFile1) -ne [System.IO.File]::ReadAllText($debugDir + $outFile2))
        {
            echo "$outFile2 - Error"# in file $outFile2"
        }
        else
        {
            echo "$outFile2 - OK"
        }
    }
    if (Test-Path $asmFile1)
    {
        if ([System.IO.File]::ReadAllText($asmFile1) -ne [System.IO.File]::ReadAllText($debugDir + $asmFile2))
        {
            echo "$asmFile2 - Error"
        }
        else
        {
            echo "$asmFile2 - OK"
        }
    }
}

cd ..