package fse2006daisy.tests;

import fse2006daisy.FSHelper;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import gov.nasa.jpf.vm.Verify;

class CreatThread extends Thread {
	private byte[] text;
	private byte[] file;
	
	public void run() {
		FileHandle fh = new FileHandle();
		
		int res = DaisyDir.creat(FSHelper.root, FSHelper.getBytes("lock"), fh);
		if (res == FSHelper.DAISY_ERR_EXIST) {
			while (DaisyDir.creat(FSHelper.root, FSHelper.getBytes("lock"), fh) == FSHelper.DAISY_ERR_EXIST) {
				
			}
			
			DaisyDir.read(FSHelper.root, 0, text.length, text);
		}
		else if (res == FSHelper.DAISY_ERR_OK) {
			
		}
	}
	
	public CreatThread(String file, String text) {
		this.file = FSHelper.getBytes(file);
		this.text = FSHelper.getBytes(text);
	}
}

public class ConcurrentCreat {
		
	public static void main(String[] args) throws InterruptedException {
		
		CreatThread th1 = new CreatThread("ff", "aa");
		CreatThread th2 = new CreatThread("ffasdfasdfasdfas", "aaasdf");
		
		FSHelper.init();
		
		//th1.start();
		th2.run();		// run in this thread
		
		//th1.join();
		//System.out.println("done");
	}
}
