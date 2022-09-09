#!/bin/bash
head -n 50000 /dev/urandom | base64 > random;
head -n 50000 random > random2;
rm random;
mv random2 random;

sort --unique random > unique_sort_by_asc;
sort --unique --numeric-sort random > unique_sort_by_num;

cat unique_sort_by_asc > totall;
cat unique_sort_by_num >> totall;

grep -o "com" totall | wc -l > number_of_com;