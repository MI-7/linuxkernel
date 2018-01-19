
cd /home/michiel/Desktop/TukOS/Development/Bootloader/Stage1/
make -s
cd /home/michiel/Desktop/TukOS/Development/Bootloader/Stage2/
make -s
cd /home/michiel/Desktop/TukOS/Development/Kernel/
make -s

mkdosfs -F 12 -r 224 -s 1 -S 512 /home/michiel/Desktop/TukOS/Release/floppy.img

dd if=/home/michiel/Desktop/TukOS/Development/Bootloader/Stage1/stage1.bin of=/home/michiel/Desktop/TukOS/Release/floppy.img bs=512 count=1 conv=notrunc

sudo mount -o loop /home/michiel/Desktop/TukOS/Release/floppy.img /home/michiel/Desktop/TukOS/Release/FloppyMount
sudo cp /home/michiel/Desktop/TukOS/Development/Bootloader/Stage2/stage2.tuk /home/michiel/Desktop/TukOS/Release/FloppyMount/
sudo cp /home/michiel/Desktop/TukOS/Development/Kernel/kernel.tuk /home/michiel/Desktop/TukOS/Release/FloppyMount/
sudo umount -l /home/michiel/Desktop/TukOS/Release/FloppyMount/
cd /home/michiel/Desktop/TukOS/Release


while true; do
    read -p "Do you wish to run Bochs?" yn
    case $yn in
        [Yy]* ) 
bochs -q
break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done




