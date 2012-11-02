# runtest.sh - Run Testset for sgdr_tsim 
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 

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
