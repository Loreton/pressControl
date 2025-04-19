#!/bin/bash
#
# updated by ...: Loreto Notarantonio
# Date .........: 19-04-2025 07.59.07
#
# Updates:
# #########################################################
####################à
# Colors:
# Black        0;30     Dark Gray     1;30
# Red          0;31     Light Red     1;31
# Green        0;32     Light Green   1;32
# Brown/Orange 0;33     Yellow        1;33
# Blue         0;34     Light Blue    1;34
# Purple       0;35     Light Purple  1;35
# Cyan         0;36     Light Cyan    1;36
# Light Gray   0;37     White         1;37
redH='\033[1;31m'
cyanH='\033[1;36m'
yellowH='\033[1;33m'
purpleH='\033[1;35m'
colorReset='\033[0m' # No Color
TAB='    '




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
echo -e "${TAB}${yellowH}$zipName has been created."