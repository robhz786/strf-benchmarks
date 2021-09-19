#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROGRAMS="to_char_ptr-header-only to_char_ptr-static-lib to_string-header-only to_string-static-lib"
SUFFIXES=$(seq -w 1 16)
MEAN_CPU_TIMES_EXE=$(find $SCRIPT_DIR -name mean_cpu_times -o -name mean_cpu_times.exe | tail -n 1)


DIR=.
if [ -n "$1" ]; then
    DIR=${1%/}
fi
OUTDIR=out

for program in $PROGRAMS;
do
    if [ ! -x $DIR/$program ]; then
	echo $DIR/$program does not exist or is not executable
	exit 1
    fi
done

if ! mkdir $OUTDIR; then
    exit 2
fi

for program in $PROGRAMS
do
    for i in $SUFFIXES
    do
        echo Writing $OUTDIR/$program.$i
        $DIR/$program >| $OUTDIR/$program.$i
	echo
    done

    if [ -n "$MEAN_CPU_TIMES_EXE" ]; then
	outfiles=$(for i in $SUFFIXES; do echo $OUTDIR/$program.$i; done)
	$MEAN_CPU_TIMES_EXE $outfiles >| $OUTDIR/$program.stats
    else
	echo "Could not find program mean_cpu_times. Skipping it."
    fi
done


