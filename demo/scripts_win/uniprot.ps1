param(
    $Folder = "$((Get-Item $PSScriptRoot).Parent.Fullname)/uniprot"
)

$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src

    ./test -c ../use-cases/uniprot.xml -g "$folder/uniprot-graph.txt" -w "$folder/uniprot-workload.xml" -r "$folder/"

    Set-Location querytranslate
    ./test -w "$folder/uniprot-workload.xml" -o "$folder/uniprot-translated"

    Set-Location ../queryinterface
    ./test -w "$folder/uniprot-workload.xml" -t "$folder/uniprot-translated" -o "$folder/uniprot-interface"

}
finally {
    $prevPwd | Set-Location
}
