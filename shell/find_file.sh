#/bin/sh

echo -n "Input a directory:"
read DIR
if [ ! -d "$DIR" ]
then
	echo "\"$DIR\" not directory"
	unset DIR
	exit 1
fi

C_FILE=0
Cpp_FILE=0
JAVA_FILE=0
SH_FILE=0
MF_FILE=0

#enter directory
function enter_dir
{
	for FILE_N in "$1"/*
	do
		case FILE_N in
		*.c)
			C_FILE=`expr $C_FILE + 1`
			;;
		*.cpp)
			Cpp_FILE=`expr $Cpp_FILE + 1`
			;;
		*.java)
			JAVA_FILE=`expr $JAVA_FILE + 1`
			;;
		*.sh)
			SH_FILE=`expr $SH_FILE + 1`
			;;
		*.makefie)
			MF_FILE=`expr $MF_FILE + 1`		
		esac
	done


}

D_NUM=0
F_NUM=0
TOTAL=0

for FILE in "$DIR"/*
do
	if [ -f "$FILE" ]
	then
		F_NUM=`expr $F_NUM + 1`
		
	elif [ -d "$FILE" ]
	then
		D_NUM=`expr $D_NUM + 1`
		enter_dir FILE

	fi
done

echo "Dirtory number: $D_NUM"
	echo "C file: $C_FILE"
	echo "C++ file: $Cpp_FILE"
	echo "Java file: $JAVA_FILE"
	echo "Shell file: $SH_FILE"
	echo "Makefile file: $MF_FILE"
echo "File number: $F_NUM"
echo "Total: $TOTAL=`expr $D_NUM + $F_NUM`"

unset D_NUM F_NUM TOTAL

