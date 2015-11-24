package serenity.disk;

import com.sun.org.apache.xml.internal.dtm.ref.DTMDefaultBaseIterators;

/**
 * Created by Sea on 11/24/2015.
 */
public class Partition {
    byte[] bytes;
    public BPB bpb;

    public class BPB {
        int bytes_per_sector;
        int sector_per_cluster;
        int reserved_sectors;
        int number_of_fat;
        int sectors_per_fat;
        int root_dir_1_cluster;

        public BPB(byte[] bytes) {
            bytes_per_sector = MBR.byteToInt(bytes[0xB], bytes[0xC]);
            sector_per_cluster = MBR.byteToInt(bytes[0xD]);
            reserved_sectors = MBR.byteToInt(bytes[0xE], bytes[0xF]);
            number_of_fat = MBR.byteToInt(bytes[0x10]);
            sectors_per_fat = MBR.byteToInt(bytes[0x24], bytes[0x25], bytes[0x26], bytes[0x27]);
            root_dir_1_cluster = MBR.byteToInt(bytes[0x2C], bytes[0x2D], bytes[0x2E], bytes[0x2F]);
        }
    }

    public Partition(byte[] bytes) {
        byte[] tmp = new byte[0x200];
        for (int i = 0; i < 0x200; i++)
            tmp[i] = bytes[i];
        this.bytes = bytes;
        this.bpb = new BPB(tmp);
        System.out.println(bpb.sectors_per_fat);
        System.out.println(bpb.sector_per_cluster);
        System.out.println(bpb.reserved_sectors);
        System.out.println(bpb.root_dir_1_cluster);
    }
}
