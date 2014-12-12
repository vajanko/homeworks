
$rootDir = "C:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\"
$dir = $rootDir + "public-ro\\mlc\\du4\\tests\\"
$debugDir = $rootDir + "public-rw\\SWI098\\du4\\Debug\\"
$exeFile = $debugDir + "mlc.exe"

cd $debugDir

foreach ($sourceFile in [System.IO.Directory]::EnumerateFiles($dir, "*.mls"))
{
    $outFile1 = $sourceFile.Replace("mls", "out")
    $stcdFile1 = $sourceFile.Replace("mls", "stcd")

    $outFile2 = [System.IO.Path]::GetFileName($outFile1)
    $stcdFile2 = [System.IO.Path]::GetFileName($stcdFile1)
    $outputFile = "output.output"
    #[System.IO.Path]::GetFileName($sourceFile.Replace("mls", "output"))

    & $exeFile $sourceFile $outputFile "-D$stcdFile2" > $outFile2

    $outFile2 = $debugDir + $outFile2;
    $stcdFile2 = $debugDir + $stcdFile2;

    if ([System.IO.File]::ReadAllText($outFile1) -ne [System.IO.File]::ReadAllText($outFile2))
    {
        echo "Error in file $outFile2"
    }
    if ([System.IO.File]::ReadAllText($stcdFile1) -ne [System.IO.File]::ReadAllText($stcdFile2))
    {
        echo "Error in file $stcdFile2"
    }
}