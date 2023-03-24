param(
    $Folder = "$((Get-Item $PSScriptRoot).Parent.Fullname)/shop"
)


$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src

    ./test -c ../use-cases/shop.xml -g "$folder/shop-graph.txt" -w "$folder/shop-workload.xml" -r "$folder/"

    Set-Location querytranslate
    ./test -w "$folder/shop-workload.xml" -o "$folder/shop-translated"

    Set-Location ../queryinterface
    ./test -w "$folder/shop-workload.xml" -t "$folder/shop-translated" -o "$folder/shop-interface"

}
finally {
    $prevPwd | Set-Location
}
