#!/bin/bash

#
# Tema3 Test Suite
#
# 2021, SD
#

# ----------------- General declarations and util functions ------------------ #

INPUT_DIR=_test/input/
REF_DIR=_test/ref/
OUT_DIR=_test/output/
EXEC_NAME=./tema3
VALGRIND="valgrind --leak-check=full --error-exitcode=1"
max_points=135
max_bonus=20
result_points=0
bonus_points=0.0

print_header()
{
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 71 ]; then
		padding=$(((71 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

test_do_fail()
{
	if test ${1} -eq 1; then
		printf "example\n"
	else
		printf "failed  [ 0/%02d]\n" "$max_points"
		if test "x$EXIT_IF_FAIL" = "x1"; then
			exit 1
		fi
	fi
}

test_do_pass()
{
	if test ${2} -eq 1; then
		printf "example\n"
	else
		printf "passed  [%02d/%02d]\n" "${1}" "$max_points"
		((result_points+=${1}))
	fi
}

valgrind_passed()
{
	if test ${2} -eq 1; then
		printf "example\n"
	else
		printf "passed  [%02.1f/%02d]\n" "${1}" "$max_bonus"
		bonus_points=`echo $bonus_points + ${1} | bc`
	fi
}

valgrind_failed()
{
	if test ${1} -eq 1; then
		printf "example\n"
	else
		printf "failed  [ 0/%02d]\n" "$max_bonus"
	fi
}

test_function()
{
	input_file="$INPUT_DIR${1}"
	output_file="$OUT_DIR${2}"
	ref_file="$REF_DIR${3}"
	points_per_test="${4}"
	points_per_test_vg="${5}"
	is_example=${6:-0}
    
	$EXEC_NAME $input_file $output_file 

	diff $output_file $ref_file > /dev/null

	if test $? -eq 0; then
		printf "[%s] ........................................... " ${2}
		test_do_pass $points_per_test $is_example
		
        #valgrind test
		$VALGRIND $EXEC_NAME $input_file $output_file &> /dev/null
		if [ $? -eq 0 ]; then
			printf "[%s - VALGRIND: PASSED] ........................ " ${2}
			valgrind_passed $points_per_test_vg $is_example
		else
			printf "[%s - VALGRIND: FAILED] ........................ " ${2}
			valgrind_failed $is_example
		fi

	else
		printf "[%s] ........................................... " ${2}
		test_do_fail $is_example
	fi
}

init_function()
{
    make build
	mkdir -p $OUT_DIR &> /dev/null
}

clean_out_function()
{
    rm _test/output/*.*
    rmdir _test/output
}

init_function

print_header "Tema 3 SD Biblioteca online"

#Testing

declare tests_values=5
declare valgrind_tests_values=(0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 1 1 1 1 1 1 1 1 1 1 1 1 1) # 14 * 0.5 + 13 * 1

# Run the example:
test_function "example.txt" "output_example.txt" "ref_example.txt" "0" "0" 1
echo ""

# Run tests
for i in {1..27}
do
	test_function "input$i.txt" "output$i.txt" "ref$i.txt" "${tests_values}" "${valgrind_tests_values[$i-1]}" 
    echo ""
done

#end Testing

printf "%65s [%02d/$max_points]\n" "Total:" $result_points;
printf "%65s [%02.1f/%02.1f]\n" "Bonus:" $bonus_points $max_bonus;

# clean_out_function

echo 

