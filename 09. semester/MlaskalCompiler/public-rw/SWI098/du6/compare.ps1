
$rootDir = "C:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\"
$dir = $rootDir + "public-ro\\mlc\\du6\\tests\\"
$debugDir = $rootDir + "public-rw\\SWI098\\du6\\Debug\\"
$mlcFile = $debugDir + "mlc.exe"
$vmFile = $rootDir + "public-ro\\common\\tools\\icm.exe"

cd $debugDir

foreach ($sourceFile in [System.IO.Directory]::EnumerateFiles($dir, "*.mls"))
{
    $outFile1 = $sourceFile.Replace("mls", "out")
    $asmFile1 = $sourceFile.Replace("mls", "moa")
    $icmFile1 = $sourceFile.Replace("mls", "icmout")
    $binFile1 = $sourceFile.Replace("mls", "mo")

    $outFile2 = [System.IO.Path]::GetFileName($outFile1)
    $binFile2 = [System.IO.Path]::GetFileName($binFile1)
    $asmFile2 = [System.IO.Path]::GetFileName($asmFile1)
    $icmFile2 = [System.IO.Path]::GetFileName($icmFile1)

    $binFile2 = $debugDir + $binFile2

    # compile the program
    & $mlcFile $sourceFile $binFile2 "-S$asmFile2" > $outFile2
    # run the program
    & $vmFile $binFile2 stack > $icmFile2

    # $outFile2 = $debugDir + $outFile2;
    # $asmFile2 = $debugDir + $asmFile2;

    if (Test-Path $outFile1)
    {
        if ([System.IO.File]::ReadAllText($outFile1) -ne [System.IO.File]::ReadAllText($debugDir + $outFile2))
        {
            echo "X - $outFile2"
        }
        else
        {
            echo ". - $outFile2"
        }
    }
    if (Test-Path $asmFile1)
    {
        if ([System.IO.File]::ReadAllText($asmFile1) -ne [System.IO.File]::ReadAllText($debugDir + $asmFile2))
        {
            echo "X - $asmFile2"
        }
        else
        {
            echo ". - $asmFile2"
        }
    }
    if (Test-Path $icmFile1)
    {
        if ([System.IO.File]::ReadAllText($icmFile1) -ne [System.IO.File]::ReadAllText($debugDir + $icmFile2))
        {
            echo "X - $icmFile2"
        }
        else
        {
            echo ". - $icmFile2"
        }
    }
}

cd ..