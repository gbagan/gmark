param(
    $Folder = "$((Get-Item $PSScriptRoot).Parent.Fullname)/test-a/"
)


$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src
    
    ./test -c ../use-cases/test.xml -g "$folder/test-a-graph.txt" -w "$folder/test-a-workload.xml" -r "$folder/" -a

    cd querytranslate
    ./test -w "$folder/test-a-workload.xml" -o "$folder/test-a-translated"

    cd ../queryinterface
    ./test -w "$folder/test-a-workload.xml" -t "$folder/test-a-translated" -o "$folder/test-a-interface"

}
finally {
    $prevPwd | Set-Location
}
