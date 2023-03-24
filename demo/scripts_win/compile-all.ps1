param(
    [switch]
    $CleanBefore
)


$prevPwd = $PWD

try {
    $base = (Get-Item $PSScriptRoot).parent.parent
    Set-Location ($base.Fullname)
    
    Set-Location src

    if ($CleanBefore) {
        make "clean"
    }

    make

    Set-Location querytranslate

    if ($CleanBefore) {
        make "clean"
    }
    make

    Set-Location ../queryinterface

    if ($CleanBefore) {
        make "clean"
    }
    make
}
finally {
    $prevPwd | Set-Location
}
