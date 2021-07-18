function Build-Program {
    param (
        $Src,
        $Dest
    )
    echo "from $Src to $Dest"
    docker run --rm -v "${pwd}:/root/env" myos-buildenv make -C Base program_dir=../$Src
    wsl ./inject.sh ${Src}/$(Split-Path -Path ${Src} -leaf).elf $Dest
}

foreach($line in Get-Content .\programList.txt) {
    if(!$line.StartsWith("#")) {
        $src = ($line -split ':')[0]
        $dest = ($line -split ':')[1]
        Build-Program $src $dest
    }
}