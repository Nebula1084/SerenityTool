package serenity.disk;

/**
 * Created by Sea on 11/24/2015.
 */
public class MBR {
    public PTE[] pte = new PTE[4];

    public class PTE {
        int active_partition;
        int start_head;
        int start_sector;
        int start_cylinder;
        int operating_system_indicator;
        int end_head;
        int end_sector;
        int end_cylinder;
        int sectors_preceding;
        int length;

        public PTE(byte[] bytes) {
            this.active_partition = MBR.byteToInt(bytes[0]);
            this.start_head = MBR.byteToInt(bytes[1]);
            this.start_sector = 0;
            this.start_cylinder = 0;
            this.operating_system_indicator = MBR.byteToInt(bytes[4]);
            this.end_head = bytes[5];
            this.end_sector = 0;
            this.end_cylinder = 0;
            this.sectors_preceding = MBR.byteToInt(bytes[8], bytes[9], bytes[10], bytes[11]);
            this.length = MBR.byteToInt(bytes[12], bytes[13], bytes[14], bytes[15]);
        }

        public int getSectors_preceding() {
            return sectors_preceding;
        }

        public int getLength() {
            return length;
        }
    }

    static int byteToInt(byte... bytes) {
        int ret = 0;
        for (int i = 0; i < bytes.length; i++) {
            ret += (bytes[i] & 0xFF) << (i * 8);
        }
        return ret;
    }

    public MBR(byte[] bytes) {
        byte[] pbytes = new byte[16];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 16; j++)
                pbytes[j] = bytes[0x01BE + i * 16 + j];
            pte[i] = new PTE(pbytes);
            System.out.println("partion" + pte[i].getSectors_preceding());
        }
    }
}