package fse2006daisy.tests;

import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import fse2006daisy.daisy.Petal;
import gov.nasa.jpf.vm.Verify;

class AppThread extends Thread {
	public void run() {
		Petal.init(false);
		
		FileHandle root = new FileHandle();
		root.inodenum = 0;
		
		FileHandle fh = new FileHandle();
		DaisyDir.creat(root, FSHelper.getBytes("f"), fh);
	}
}

public class MultipleCreat {

	public static void main(String[] args) throws InterruptedException {
		
		AppThread th1 = new AppThread();
		AppThread th2 = new AppThread();
		
		th1.start();
		th2.start();
		
		th1.join();
		th2.join();
	}

}
