package serenity.disk;

import java.io.*;

/**
 * Created by Sea on 11/23/2015.
 */
public class VirtualDisk {
    private MBR mbr;
    private RandomAccessFile file;
    private Partition p;

    public VirtualDisk() {

//        byte[] tmp = new byte[50 * 1024 * 1024];
        try {
            file = new RandomAccessFile("E:\\Education\\My Resource\\AaI\\SVD.vhd", "rb+");

//            file.read(tmp, 0, 50 * 1024 * 1024);
//            mbr = new MBR(tmp);
//            int pre = mbr.pte[0].getSectors_preceding();
//            int len = mbr.pte[0].getLength();
//
//            for (int i = pre * 512; i < (pre + len) * 512; i++) {
//                tmp[i - pre * 512] = tmp[i];
//            }
//
//            p = new Partition(tmp);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public byte[] readSector(int i) throws IOException {
        byte[] bytes = new byte[512];
        file.seek(i * 512);
        file.read(bytes, 0, 512);
        return bytes;
    }

    public void writeSector(byte[] bytes, int i) throws IOException {
        file.seek(i*512);
        file.write(bytes);
    }
}
