package serenity;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.RandomAccessFile;

public class VHD {
	RandomAccessFile disk = new RandomAccessFile("Senerity.vhd", "rw");
	static final int SECTOR_SIZE = 512; 
	int offset;
	Boolean reading = new Boolean(false);
	Boolean writing = new Boolean(false);
	InputStream in;
	OutputStream out;
	final long MaxOffset = disk.length() / SECTOR_SIZE;
    /** */
    public Runnable SerialReader = new Runnable() {
    	int state = 0;
    	int count = 0;
    	int resetCount = 0;
        StringBuffer s = new StringBuffer();
        public void run() {
            byte[] buffer = new byte[1024];
            int len = -1;
            s.append("\nOffset: ");
            try {
                while ((len = in.read(buffer)) > -1) {
                    if (len == 0){
                    	++resetCount;
                    	if(resetCount == 10){
                    		resetCount = 0;
                    		count = 0;
                    		state = 0;
                    	}
                    	continue;
                    }
                    resetCount = 0;
//                    System.out.print(String.format("len = %d, ", len));
//                    for(int i = 0; i < len; ++i){
//                    	System.out.print(String.format("%02X ", buffer[i]));
//                    }
                    System.out.println();
                    for(int i = 0; i < len; ++i){
	                    switch(state){
		                case 0:
	                		offset = offset << 8;
	                		offset = offset | (buffer[i] & 0x00ff);
	                		count++;
		                	if(count == 4){
		                		count = 0;
		                		state = 1;
		                		s.append(String.format("%08X\n", offset));
		                	}
//		                	System.out.println(String.format("%08X\n", offset));
		                	break;
		                case 1:
	                		disk.seek(offset*SECTOR_SIZE);
		                	switch(buffer[i]){
		                	case 1:
		                		s.append("Read\n");
		                		if(offset <= MaxOffset){
		                			synchronized(reading){
		                				reading = true;
		                			}
		                		}
		                		state = 0;
		                		System.out.println(s);
		                		s = new StringBuffer("\nOffset: ");
		                		break;
		                	case 2:
		                		s.append("Write\n");
		                		if(offset <= MaxOffset){
			                		state = 2;
			                		count = 0;
		                		}
		                		else{
		                			System.out.println(s);
		                			System.out.println("Out of range " + MaxOffset);
		                			throw new IndexOutOfBoundsException();
//		                			state = 0;
		                		}
		                		writing = true;
		                		break;
		                	default:
	                			System.out.println(s);
		                		System.out.println("\nWrong command!");
		                	}
		                	break;
		                case 2:
		                	if(count < SECTOR_SIZE - 1){
		                		s.append(String.format("%02X ", buffer[i]));
		                		disk.write(buffer[i]);
		                		count++;
		                		if(count%32 == 0){
		                			s.append("\n");
		                		}
		                	}
		                	else{
		                		s.append(String.format("%02X\n", buffer[i]));
		                		disk.write(buffer[i]);
		                		System.out.println(s);
		                		s = new StringBuffer("\nOffset: ");
		                		count = 0;
		                		state = 0;
		                		writing = false;
		                	}
		                	break;
	                    }
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

    /** */
    public Runnable SerialWriter = new Runnable() {
    	byte[] buffer = new byte[SECTOR_SIZE];
    	int count = 0;
        public void run() {
            try {
                while (true) {
                	synchronized(reading){
                		count = 0;
	                	if(reading){
	                		disk.seek(offset*SECTOR_SIZE);
	                		while(count < SECTOR_SIZE){
	                			count += disk.read(buffer, count, buffer.length-count);
	                		}
	                		StringBuffer s = new StringBuffer();
	                		for(int i = 0; i < SECTOR_SIZE; i++){
	                			s.append(String.format("%02X ",buffer[i]));
	                			if(i%32 == 31)
	                				s.append('\n');
	                		}
	                		System.out.println(s);
	                		out.write(buffer);
	                		reading = false;
	                	}
                	}
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };
    
    Thread readThread;
    Thread writeThread;
    
    public VHD(String portName) throws Exception {
        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
        if (portIdentifier.isCurrentlyOwned()) {
            System.out.println("Error: Port is currently in use");
        } else {
            CommPort commPort = portIdentifier.open("VHD", 2000);

            if (commPort instanceof SerialPort) {
                SerialPort serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(115200, SerialPort.DATABITS_8, SerialPort.STOPBITS_2, SerialPort.PARITY_NONE);

                in = serialPort.getInputStream();
                out = serialPort.getOutputStream();
                readThread = new Thread(SerialReader);
                writeThread = new Thread(SerialWriter);
            } else {
                System.out.println("Error: Only serial ports are handled by this example.");
            }
        }
    }

    public static void main(String[] args) {
        try {
            VHD vhd = new VHD("COM3");
            
//        	byte[] buffer = new byte[SECTOR_SIZE];
//        	int count = 0;
//    		disk.seek(offset*SECTOR_SIZE);
//    		while(count < SECTOR_SIZE){
//    			count += disk.read(buffer, count, buffer.length-count);
//    		}
//    		StringBuffer s = new StringBuffer();
//    		for(int i = 0; i < SECTOR_SIZE; i++){
//    			s.append(String.format("%02X ",buffer[i]));
//    			if(i%32 == 31)
//    				s.append('\n');
//    		}
//    		System.out.println(s);
    		
            vhd.readThread.start();
            vhd.writeThread.start();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
