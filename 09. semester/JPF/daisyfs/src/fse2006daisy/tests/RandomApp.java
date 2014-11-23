package fse2006daisy.tests;

import fse2006daisy.daisy.Attribute;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import gov.nasa.jpf.vm.Verify;

public class RandomApp {
	
	
	private static void randomeOp(FileHandle parent, FileHandle file, Attribute a, byte[] filename) {
		int op = Verify.random(5);
		int offset = Verify.random(2);
		
		switch (op) {
		case 0:
			DaisyDir.creat(parent, filename, file);
			break;
		case 1:
			DaisyDir.unlink(parent, filename);
			break;
		case 2:
			DaisyDir.write(file, offset, filename.length, filename);
			DaisyDir.get_attr(file, a);
			break;
		case 3:
			DaisyDir.read(file, offset, filename.length, filename);
			break;
		case 4:
			DaisyDir.get_attr(file, a);
			break;
		case 5:
			a.size = filename.length;
			DaisyDir.set_attr(file, a);
			break;
		}
	}
	
	public static void main(String[] args) {
		
		FSHelper.init();
		
		FileHandle fh = new FileHandle();
		Attribute a = new Attribute();
		String str = StringHelper.randomStr(Verify.random(3));
		byte[] filename = FSHelper.getBytes(str);
		
		while (Verify.randomBool()) {
			
			randomeOp(FSHelper.root, fh, a, filename);
		}
	}
}
