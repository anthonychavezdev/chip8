#!/usr/bin/env bash

set -e

##
# Check that the dependency is installed.
##
which graph-easy > /dev/null 2>&1 || {
		echo "Please install graph library by running:"
		echo "$ cpan Graph::Easy"
		exit
}

tf=`mktemp`
tf2=`mktemp`

get_lines(){
		find .dep -type f |
				while read name; do
						cat $name |
								tr '\n' ' '|
								sed 's/[\\]//' |
								sed 's/[^ ]*[.]d//g' |
								sed 's/://' |
								sed 's/[.]\(cpp\|hpp\|c\|h\|o\)//g'|
								sed 's/\([^ ]*\)  *\1/\1 /' |
								sed 's/\([^ ]*\)  *\1/\1 /' |
								sed 's/  */ /g'
						echo
				done
}

sort_edge(){
		first=$1
		if grep "$first" "$tf" > /dev/null; then
			 echo "$@" >> "$tf"
			 return
		else
				echo "    $first;"
				echo "$@" >> "$tf2"
		fi
}

sort_edges(){
		# Assuming that there are no cycles, just loop though the edges
		# until none are left, and edge is extracted if it's not included
		# by any others.

		# TODO: If there is a cycle this will get stuck.  Add some sort of
		# failure check and just let GraphViz deal with it.
		while grep "" "$tf" > /dev/null; do
				line=`head -1 "$tf"`
				sed -i '1d' "$tf"
				sort_edge $line
		done
}

build_edge(){
		first=$1
		shift
		while(($#)); do
				echo "    $first -> $1;"
				shift
		done
}

build_edges(){
		while grep "" "$tf2" > /dev/null; do
				line=`head -1 "$tf2"`
				sed -i '1d' "$tf2"
				build_edge $line
		done
}

build_dot(){
		echo "digraph graphname {"
		get_lines > "$tf"
		sort_edges
		echo ""
		build_edges
		echo "}"
}

build_dot | graph-easy

rm -- "$tf"
rm -- "$tf2"
