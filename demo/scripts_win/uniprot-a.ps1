param(
    $Folder = "$((Get-Item $PSScriptRoot).Parent.Fullname)/uniprot-a"
)

$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src

    ./test -c ../use-cases/uniprot.xml -g "$folder/uniprot-a-graph.txt" -w "$folder/uniprot-a-workload.xml" -r "$folder/" -a

    Set-Location querytranslate
    ./test -w "$folder/uniprot-a-workload.xml" -o "$folder/uniprot-a-translated"

    Set-Location ../queryinterface
    ./test -w "$folder/uniprot-a-workload.xml" -t "$folder/uniprot-a-translated" -o "$folder/uniprot-a-interface"
}
finally {
    $prevPwd | Set-Location
}
