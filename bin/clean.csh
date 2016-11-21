#!/bin/csh


    
### delete specified folders
foreach p("debug" "release" "__tmp" "lib" "_a-docs" "iphoneos" "iphonesimulator" "Debug" "Release-iphoneos" "Release-iphonesimulator"  "project.xcworkspace" "xcshareddata" "__pycache__" "x64" ".vs")
    set ftemp=$p
    echo  $p `find . -type d -name "$ftemp" | wc -l`
    find . -type d -name "$ftemp" -exec rm -r {} \;
end

echo "delete specified files by extention"
echo "cleaning tmp-files.."
foreach p("pdb" "o" "a" "lib" "so" "ilk" "res" "exe" "sdf" "pro.user" "vcxproj" "vcxproj.user" "vcxproj.filters" "so-deployment-settings.json" "qmake.stash" "DS_Store" "log" "mak" "pbxproj" "plist" "suo" "VC.db" "htm")
    set ftemp="*."$p
    echo  $p `find . -type f -name "$ftemp" | wc -l`
    find . -type f -name "$ftemp" -exec rm {} \;
end

### delete specified files by name
foreach p("Makefile" "core" "._")
    set ftemp=$p"*"
    echo $p `find . -type f -name "$ftemp" | wc -l`
    find . -type f -name "$ftemp" -exec rm {} \;
end


echo "qrc_.....-res.cpp" `find . -type f -name "qrc_*-res.cpp" | wc -l`
find . -type f -name "qrc_*-res.cpp" -exec rm {} \;



