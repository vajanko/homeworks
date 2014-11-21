package fse2006daisy;

import fse2006daisy.daisy.Attribute;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import fse2006daisy.daisy.Petal;
import gov.nasa.jpf.vm.Verify;

public class Producer extends Thread {
	private static final int MAX_DIRS = 2;
	private static final int MAX_FILES = 2;
	private static String text;
	
	public void run() {
//		while (true) {
//			String path = StringHelper.randomPath(1 + Verify.random(2)); 
//			FSHelper.writeStr(path, "dasdfasdfasdfasdfasdf");
//			
//		}
//		String path = StringHelper.randomPath(1 + Verify.random(2));
//		//FileHandle fh = FSHelper.createPath(path);
//		String data = StringHelper.randomStr(Verify.random(10));
//		FSHelper.writeStr(path, data);
		
		
//		Attribute a = new Attribute();
//        FileHandle root = new FileHandle();
        //FileHandle fh = new FileHandle();
		
		//DaisyDir.creat(FSHelper.root, DaisyTest.stringToBytes(text), fh);
		FileHandle fh = FSHelper.createPath("/file" + text);
		FSHelper.writeStr(fh, 0, text);
	}
	
	public Producer(String text) {
		this.text = text;
	}
}
