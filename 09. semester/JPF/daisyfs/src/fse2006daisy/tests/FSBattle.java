package fse2006daisy.tests;

import fse2006daisy.FSHelper;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;

class ProducerThread extends Thread {
	private byte[] file;
	private byte[] text;
	
	public void run() {
		FileHandle fh = new FileHandle();	
		while (true) {
			DaisyDir.creat(FSHelper.root, file, fh);
			//DaisyDir.write(fh, 0, text.length, text);
		}
	}
	
	public ProducerThread(String filename) {
		file = FSHelper.getBytes(filename);
		//text = FSHelper.getBytes("a");
	}
}
class ConsumerThread extends Thread {
	private byte[] file;
	
	public void run() {
		while (true) {
			DaisyDir.unlink(FSHelper.root, file);
		}
	}
	
	public ConsumerThread(String filename) {
		file = FSHelper.getBytes(filename);
	}
}

public class FSBattle {
	public static void main(String[] args) throws InterruptedException {
		ProducerThread prod = new ProducerThread("fff");
		ConsumerThread cons = new ConsumerThread("fff");
		
		FSHelper.init();
		
		prod.start();
		cons.run();
		
		prod.join();
	}
}
