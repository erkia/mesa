$env:PIGLIT_NO_FAST_SKIP = 1

Copy-Item -Path _install\bin\opengl32.dll -Destination C:\Piglit\lib\piglit\bin\opengl32.dll
Copy-Item -Path _install\bin\libgallium_wgl.dll -Destination C:\Piglit\lib\piglit\bin\libgallium_wgl.dll
Copy-Item -Path _install\bin\libglapi.dll -Destination C:\Piglit\lib\piglit\bin\libglapi.dll

cmd.exe /C "py -3 C:\Piglit\bin\piglit.py run --timeout 240 `"$env:PIGLIT_PROFILE`" $env:PIGLIT_OPTIONS $env:PIGLIT_TESTS .\results"

py -3 C:\Piglit\bin\piglit.py summary console .\results | Select -SkipLast 1 | Select-String -NotMatch -Pattern ': pass' | Set-Content -Path .\result.txt

$reference = Get-Content ".\_install\$env:PIGLIT_RESULTS.txt"
$result = Get-Content .\result.txt
if (-Not ($reference -And $result)) {
  Exit 1
}

$diff = Compare-Object -ReferenceObject $reference -DifferenceObject $result
if (-Not $diff) {
  Exit 0
}

py -3 C:\Piglit\bin\piglit.py summary html --exclude-details=pass .\summary .\results

Write-Host "Unexpected change in results:"
Write-Output $diff | Format-Table -Property SideIndicator,InputObject -Wrap

Exit 1
