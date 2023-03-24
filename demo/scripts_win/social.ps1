param(
    $Folder = "$((Get-Item $PSScriptRoot).Parent.Fullname)/social"
)


$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src

    ./test -c ../use-cases/social-network.xml -g "$folder/social-graph.txt" -w "$folder/social-workload.xml" -r "$folder/"

    Set-Location querytranslate
    ./test -w "$folder/social-workload.xml" -o "$folder/social-translated"

    Set-Location ../queryinterface
    ./test -w "$folder/social-workload.xml" -t "$folder/social-translated" -o "$folder/social-interface"
}
finally {
    $prevPwd | Set-Location
}
