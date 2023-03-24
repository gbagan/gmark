param()


$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location demo/play

    

    Remove-Item play*txt
    Remove-Item *html
    Remove-Item *xml
    Remove-Item play-interface/query*
    Remove-Item play-translated/query*
    
}
finally {
    $prevPwd | Set-Location
}
