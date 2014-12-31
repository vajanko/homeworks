
$rootDir = "C:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\"
$dir = $rootDir + "public-ro\\mlc\\du5\\tests\\"
$debugDir = $rootDir + "public-rw\\SWI098\\du5\\Debug\\"
$exeFile = $debugDir + "mlc.exe"

cd $debugDir

foreach ($sourceFile in [System.IO.Directory]::EnumerateFiles($dir, "*.mls"))
{
    $outFile1 = $sourceFile.Replace("mls", "out")
    $binFile1 = $sourceFile.Replace("mls", "mo")
    $stcdFile1 = $sourceFile.Replace("mls", "stcd")
    $asmFile1 = $sourceFile.Replace("mls", "moa")

    $outFile2 = [System.IO.Path]::GetFileName($outFile1)
    $binFile2 = [System.IO.Path]::GetFileName($binFile1)
    $stcdFile2 = [System.IO.Path]::GetFileName($stcdFile1)
    $asmFile2 = [System.IO.Path]::GetFileName($asmFile1)

    # $outputFile = "output.output"
    # [System.IO.Path]::GetFileName($sourceFile.Replace("mls", "output"))

    & $exeFile $sourceFile $binFile2 "-D$stcdFile2" "-S$asmFile2" > $outFile2

    $outFile2 = $debugDir + $outFile2;
    $binFile2 = $debugDir + $binFile2;
    $stcdFile2 = $debugDir + $stcdFile2;
    $asmFile2 = $debugDir + $asmFile2;

    if ([System.IO.File]::ReadAllText($outFile1) -ne [System.IO.File]::ReadAllText($outFile2))
    {
        echo "Error in file $outFile2"
    }
    if ([System.IO.File]::ReadAllText($binFile1) -ne [System.IO.File]::ReadAllText($binFile2))
    {
        echo "Error in file $binFile2"
    }
    if ([System.IO.File]::ReadAllText($stcdFile1) -ne [System.IO.File]::ReadAllText($stcdFile2))
    {
        echo "Error in file $stcdFile2"
    }
    if ([System.IO.File]::ReadAllText($asmFile1) -ne [System.IO.File]::ReadAllText($asmFile2))
    {
        echo "Error in file $asmFile2"
    }
}

cd ..