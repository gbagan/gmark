param(
    $Folder = "$((Get-Item $PSScriptRoot).Parent.Fullname)/test"
)

$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src

    ./test -c ../use-cases/test.xml -g "$folder/test-graph.txt" -w "$folder/test-workload.xml" -r "$folder/"

    Set-Location querytranslate
    ./test -w "$folder/test-workload.xml" -o "$folder/test-translated"

    Set-Location ../queryinterface
    ./test -w "$folder/test-workload.xml" -t "$folder/test-translated" -o "$folder/test-interface"

}
finally {
    $prevPwd | Set-Location
}
