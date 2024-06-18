#!/bin/bash

attempts=0
successes=0
failes=0

cases=("[sm] smt.txt kff empty" "-e ^$. -e sm ss smt.txt test.txt" "-e sm -e mt smt.txt test.txt tt" "-f test.txt -e zz smt.txt")

flags=("v" "c" "l" "n" "h" "s" "o")

test_func() {
    ../s21_grep $@ > s21_grep_output
    grep $@ > grep_output
    ((attempts++))
    if diff "s21_grep_output" "grep_output"; then
        ((successes++))
        echo "success result - $successes with next template \""$@"\" at $attempts attempt"
    else
        ((failes++))
        echo "fail result - $failes with next template - \"($@)\"  at $attempts attempt"
    fi

}

# 1 param
for param1 in ${flags[@]}; do
    for template in "${cases[@]}"; do
        test_func "$template -$param1"
    done
done

#2 params
for param1 in ${flags[@]}; do
    for param2 in ${flags[@]}; do
    if [ $param1 != $param2 ]; then
        params="${param1}${param2}"
        for template in "${cases[@]}"; do
            test_func "$template -$params"
        done
    fi
    done
done

# 3 params
for param1 in ${flags[@]}; do
    for param2 in ${flags[@]}; do
        for param3 in ${flags[@]}; do
            if [ $param1 != $param2 ] && [ $param2 != $param3 ] && [ $param1 != $param3 ]; then
                for template in "${cases[@]}"; do
                    test_func "$template -$param1$param2$param3"
                done
            fi
        done
    done
done

echo "Successes - $successes, Failes - $failes, Attempts - $attempts"