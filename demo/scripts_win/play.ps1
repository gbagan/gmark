param()


$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src

    ./test -c ../use-cases/test.xml -g ../demo/play/play-graph.txt -w ../demo/play/play-workload.xml -r ../demo/play/

    Set-Location querytranslate
    ./test -w ../../demo/play/play-workload.xml -o ../../demo/play/play-translated

    Set-Location ../queryinterface
    ./test -w ../../demo/play/play-workload.xml -t ../../demo/play/play-translated -o ../../demo/play/play-interface
}
finally {
    $prevPwd | Set-Location
}
