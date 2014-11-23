package fse2006daisy;

import fse2006daisy.daisy.Attribute;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import fse2006daisy.daisy.Petal;
import gov.nasa.jpf.vm.Verify;

public class Application extends Thread {
	private static final int MAX_DIRS = 2;
	private static final int MAX_FILES = 2;
	private static byte[] text;
	private static byte[] filename;
	
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
        FileHandle fh = new FileHandle();
		
		DaisyDir.creat(FSHelper.root, text, fh);
		//FileHandle fh = FSHelper.createPath("/f");
		
		// create a new file in the root dir
//		FileHandle fh = new FileHandle();
//		
//		
//		while (Verify.randomBool()) 
//		{
//			for (int i = 0; i < 1 + Verify.random(1); i++) {
//				byte[] f = FSHelper.getBytes("f" + i);
//				if (DaisyDir.lookup(FSHelper.root, f, fh) == FSHelper.DAISY_ERR_NOENT)
//					DaisyDir.creat(FSHelper.root, f, fh);
//			}
//			
//			
//			// write some text at random position
//			while (Verify.randomBool())
//				DaisyDir.write(fh, Verify.random(2), text.length, text);
//			while (Verify.randomBool())
//				DaisyDir.read(fh, Verify.random(2), text.length, text);
//			
//			if (Verify.randomBool())
//				DaisyDir.unlink(FSHelper.root, filename);
//		}
		/*}
		else {
			DaisyDir.read(fh, Verify.random(2), text.length, text);
		}*/
	}
	
	public Application(String text) {
		this.text = FSHelper.getBytes(text);
		this.filename = FSHelper.getBytes("fklkk");
	}
}
