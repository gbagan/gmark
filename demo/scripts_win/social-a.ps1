param(
    $Folder = "$((Get-Item $PSScriptRoot).Parent.Fullname)/social-a"
)


$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src

    ./test -c ../use-cases/social-network.xml -g "$folder/social-a-graph.txt" -w "$folder/social-a-workload.xml" -r "$folder/" -a

    Set-Location querytranslate
    ./test -w "$folder/social-a-workload.xml" -o "$folder/social-a-translated"

    Set-Location ../queryinterface
    ./test -w "$folder/social-a-workload.xml" -t "$folder/social-a-translated" -o "$folder/social-a-interface"
}
finally {
    $prevPwd | Set-Location
}
