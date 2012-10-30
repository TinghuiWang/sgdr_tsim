if [ $# -gt 0 ]; then
	if [ -f test/$1.S ]; then
		if [ -f test/$1.r ]; then
			if [ -f test/$1.m ]; then
				./sgdr_tsim -S test/$1.S -r test/$1.r -m test/$1.m $2 $3 $4 $5
			else
				echo "test/$1.m not found"
			fi;
		else
			echo "test/$1.r not found"
		fi;
	else
		echo "test/$1.S not found"
	fi;
else
	echo "Useage: run_test.sh <test_name> [-sgx]"
fi;
