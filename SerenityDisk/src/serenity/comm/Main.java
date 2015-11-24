package serenity.comm;


import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import serenity.disk.VirtualDisk;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Main {
    static VirtualDisk disk;

    void connect(String portName) throws Exception {
        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
        if (portIdentifier.isCurrentlyOwned()) {
            System.out.println("Error: Port is currently in use");
        } else {
            CommPort commPort = portIdentifier.open(this.getClass().getName(), 2000);

            if (commPort instanceof SerialPort) {
                SerialPort serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(115200, SerialPort.DATABITS_8, SerialPort.STOPBITS_2, SerialPort.PARITY_NONE);

                InputStream in = serialPort.getInputStream();
                OutputStream out = serialPort.getOutputStream();

                (new Thread(new SerialReader(in))).start();
                (new Thread(new SerialWriter(out))).start();

            } else {
                System.out.println("Error: Only serial ports are handled by this example.");
            }
        }
    }

    /** */
    public static class SerialReader implements Runnable {
        InputStream in;

        public SerialReader(InputStream in) {
            this.in = in;
        }

        @Override
        public void run() {
            byte[] buffer = new byte[1024];
            int len = -1;
            try {
                while ((len = this.in.read(buffer)) > -1) {
                    if (len == 0) continue;
                    System.out.println(new String(buffer, 0, len));
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /** */
    public static class SerialWriter implements Runnable {
        OutputStream out;

        public SerialWriter(OutputStream out) {
            this.out = out;
        }

        @Override
        public void run() {
            try {
                int c = 1;
                while (true) {
                    this.out.write(c++);
                    if (c > 0xFF) c = 1;
                    System.out.println(c);
                    Thread.sleep(1000);
                }
            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        disk = new VirtualDisk();
//        try {
//            (new Main()).connect("COM3");
//        } catch (Exception e) {
//            // TODO Auto-generated catch block
//            e.printStackTrace();
//        }

    }
}
