#!/bin/bash
#
# updated by ...: Loreto Notarantonio
# Date .........: 19-04-2025 07.55.58
#
# Updates:
# #########################################################



# #############################################
# # Per le candidate_directories cerchiamo di
# #     risolvere tutti i link inglobando il vero
# #     file in modo da avere, su git, il progetto completo.
# #############################################
function zip_logical_links() {
    # assumiamo che ci troviamo nella prj_dir
    root_dir="$PWD"

    # candidate_dirs="lnLib Source Source/Main Source/lnLib"
    candidate_dirs="lib"

    zipName="${PWD}/lib/lnLibary.zip"


    for _dir in $candidate_dirs; do
        if [ -d "$_dir" ]; then
            cd "$_dir"
            for name in *; do
                [ -L "$name" ] && zip -x '*.git*' -x '*.pio*' -ur --latest-time  $zipName $name
            done
        fi
        cd "$root_dir" # ritorna alla root dir
    done

    cd "$root_dir"
}


zip_logical_links
echo "$zipName has been created."