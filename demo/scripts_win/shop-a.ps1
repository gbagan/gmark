param(
    $Folder = "$((Get-Item $PSScriptRoot).Parent.Fullname)/shop-a"
)


$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src

    ./test -c ../use-cases/shop.xml -g "$folder/shop-a-graph.txt" -w "$folder/shop-a-workload.xml" -r "$folder/" -a

    Set-Location querytranslate
    ./test -w "$folder/shop-a-workload.xml" -o "$folder/shop-a-translated"

    Set-Location ../queryinterface
    ./test -w "$folder/shop-a-workload.xml" -t "$folder/shop-a-translated" -o "$folder/shop-a-interface"

}
finally {
    $prevPwd | Set-Location
}
