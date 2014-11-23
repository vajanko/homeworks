package fse2006daisy.tests;

import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;

class CreatThread extends Thread {
		
	public void run() {
		FileHandle fh = new FileHandle();
		DaisyDir.creat(FSHelper.root, FSHelper.getBytes("f"), fh);
	}
}

class UnlinkThread extends Thread {

	public void run() {
		DaisyDir.unlink(FSHelper.root, FSHelper.getBytes("f"));
	}
}

class WriteThread extends Thread {
	public void run() {
		FileHandle fh = new FileHandle();
		if (DaisyDir.lookup(FSHelper.root, FSHelper.getBytes("f"), fh) == FSHelper.DAISY_ERR_OK) {
			DaisyDir.write(fh, 0, 3, FSHelper.getBytes("abc"));
		}
	}
	public WriteThread() {
	}
}
class ReadThread extends Thread {
	public void run() {
		FileHandle fh = new FileHandle();
		if (DaisyDir.lookup(FSHelper.root, FSHelper.getBytes("f"), fh) == FSHelper.DAISY_ERR_OK) {
			byte[] buff = new byte[3];
			DaisyDir.read(fh, 0, 3, buff);
		}
		
	}
	public ReadThread() {
	}
}

public class ConcurrentCreat {
		
	public static void main(String[] args) throws InterruptedException {
			
		FSHelper.init();
		FileHandle fh = new FileHandle();
		DaisyDir.creat(FSHelper.root, FSHelper.getBytes("f"), fh);
		
		int count = 2;
		
		Thread[] threads = new Thread[count];
		threads[0] = new CreatThread();
		threads[1] = new WriteThread();
		//threads[2] = new ReadThread();
		
		for (int i = 0; i < count; i++) {
			threads[i].start();
		}
		
		for (int i = 0; i < count; i++) {
			threads[i].join();
		}
	}
}
