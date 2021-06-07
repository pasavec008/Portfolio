#! /bin/bash
#
# Meno: Marek Adamovic
# Kruzok: Piatok 8:00, Ing. Martin Vojtko, PhD.
# Datum: 30.10.2020
# Zadanie: zadanie04
#
# Text zadania:
#
# V zadanych adresaroch uvedenych ako argumenty najdite symbolicke linky,
# ktorych cielova cesta ma zo vsetkych najviac komponentov - to znamena, ze sa
# na ceste k cielovemu suboru nachadza najviac adresarov. Prehladavajte vsetky
# zadane adresare a aj ich podadresare.
# Ak nebude uvedena ako argument ziadna cesta, prehladava sa aktualny pracovny
# adresar (teda .).
# Ak bude skript spusteny s prepinacom -d <hlbka>, prehlada adresare len do
# hlbky <hlbka> (vratane). Hlbka znamena pocet adresarov na ceste medzi
# startovacim adresarom a spracovavanym suborom. Hlbka 1 znamena, ze bude
# prezerat subory len v priamo zadanych adresaroch.
#
# Syntax:
# zadanie.sh [-h] [-d <hlbka>] [cesta ...]
#
# Vystup ma tvar:
# Output: '<cesta k najdenej linke> -> <cielova cesta>'
#
# Priklad vystupu:
# Output: 'testdir3/lvl1_2/lvl2_2/symlink_4 -> ../../lvl1_1/lvl2_1/testfile_17'
#
#
# Program musi osetrovat pocet a spravnost argumentov. Program musi mat help,
# ktory sa vypise pri zadani argumentu -h a ma tvar:
# Meno programu (C) meno autora
#
# Usage: <meno_programu> <arg1> <arg2> ...
#       <arg1>: xxxxxx
#       <arg2>: yyyyy
#
# Parametre uvedene v <> treba nahradit skutocnymi hodnotami.
# Ked ma skript prehladavat adresare, tak vzdy treba prehladat vsetky zadane
# adresare a vsetky ich podadresare do hlbky.
# Pri hladani maxim alebo minim treba vzdy najst maximum (minimum) vo vsetkych
# zadanych adresaroch (suboroch) spolu. Ked viacero suborov (adresarov, ...)
# splna maximum (minimum), treba vypisat vsetky.
#
# Korektny vystup programu musi ist na standardny vystup (stdout).
# Chybovy vystup programu by mal ist na chybovy vystup (stderr).
# Chybovy vystup musi mat tvar (vratane apostrofov):
# Error: 'adresar, subor, ... pri ktorom nastala chyba': popis chyby ...
# Ak program pouziva nejake pomocne vypisy, musia mat tvar:
# Debug: vypis ...
#
# Poznamky: (sem vlozte pripadne poznamky k vypracovanemu zadaniu)
#
# Riesenie:

help(){
        #funkcia vypisujuca help
        echo "adamovic_z4.sh (C) Marek Adamovic";
        echo ;
        echo "Usage: adamovic_z4.sh -h -d 10 /public/";
        echo "  -h: writes help of program in addition to output";
        echo "  -d: option to specify depth of search";
        echo "  10: number which belong to -d option";
        echo "  /public/: directory where script starts search";
        echo ;
}

#najskor prezriem argumenty a hladam v nich zname prepinace
#nezname prepinace chape program ako cestu, teda vyhodi error zlej cesty
for var in "$@"
do
        if [ $var = "-d" ]; then
                #osetrim pripad zleho poradia argumentov
                if [ ${1} != "-d" ]; then
                        >&2 echo "Error: Option -d need to be before directories";
                        return 7;
                fi
                #osetrim pripad, kedy za -d nie je cislo
                if ! [ ${2} -eq ${2} ] 2> /dev/null; then
                        >&2 echo "Error: There is not number after -d option";
                        return 8;
                fi
                #osetrim pripad, kde po -d je zaporne cislo alebo 0
                if [ 1 -gt ${2} ]; then
                        >&2 echo "Error: Number after -d need to be positive integer";
                        return 10;
                fi
                depth="-maxdepth ${2}";
                shift;
                shift;
        elif [ $var = "-h" ]; then
                #osetrim pripad zleho poradia argumentov
                if [ ${1} != "-h" ]; then
                        >&2 echo "Error: Option -h need to be before directories";
                        return 9;
                fi
                help;
                shift;
        fi
done

minimum=0;
for var in "$@"
do
        #tu skontrolujem spravny format cesty, teda ci je na konci / a taktiez ci cesta vedie k priecinku
        check=$(echo -n $var | tail -c 1);
        if ! [ -d "$var" ]; then
                >&2 echo "Error: $var is not directory";
                return 1;
        fi

        if [ $check != "/" ]; then
                >&2 echo "Error: $var is not correct format (missing /)";
                return 2;
        fi


        #prejdem si vsetky linky a zapisem najvacsiu hlbku, ktoru potom pri vypise
        #nastavim ako mindepth

        minimumL=0;
        minimumL=`find $var $depth -type l | awk -F "$var" '{printf "/%s\n", $2}' | awk -F "/" -v m=$minimumL '{if (NF - 1 > m) m=NF - 1}; END {printf "%d\n", m}'`
        #echo "Debug: $minimumL";
        if [ $minimumL -gt $minimum ]; then
                minimum=$minimumL;
        fi


        #echo "Debug: $minimum";
        #vypis pre mindepth, co dam v dalsom cykle
done
minimum="-mindepth $minimum";

for var in "$@"
do
        vystup=$(find $var $depth $minimum -type l -ls | awk -F "$var" '{for ( i = 2; i <= NF; i++) printf "/%s", $i}; {printf "\n"}');

        printf "$vystup\n";
done

#teraz osetrim pripad bez argumentu cesty, teda prehladavanie working dir
if [ $# = 0 ]; then
        minimum=0;
        minimum=`find . $depth -type l | awk -F "." '{printf "%s\n", $2}' | awk -F "/" -v m=$minimum '{if (NF - 1 > m) m=NF - 1}; END {printf "%d\n", m}'`
        minimum="-mindepth $minimum";

        #echo "Debug: $minimum";

        vystup=$(find . $depth $minimum -type l -ls | awk -F "/" '{for ( i = 2; i <= NF; i++) printf "/%s", $i}; {printf "\n"}');
        printf "$vystup\n";
fi

return 0;