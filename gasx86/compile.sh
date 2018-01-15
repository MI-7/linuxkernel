echo 'compiling '$1'..'
as -g -o $1.ld $1
ld $1.ld
echo 'removing link file..'
rm $1.ld
echo 'executing a.out..'
./a.out
echo 'done'
