package fse2006daisy.tests;

import fse2006daisy.FSHelper;
import fse2006daisy.StringHelper;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import gov.nasa.jpf.vm.Verify;

public class FileSearch {
	public static void main(String[] args) {
		
		FSHelper.init();
		
		FileHandle fh = new FileHandle();
		byte[] file = FSHelper.getBytes("f");
		DaisyDir.lookup(FSHelper.root, file, fh);
		
		int offset = 0;
		byte[] data = new byte[1];
		while (DaisyDir.write(fh, offset, data.length, data) == FSHelper.DAISY_ERR_OK)
			offset++;
		
		
//		FileHandle parent = FSHelper.root;
//		
//		
//		byte[] file = FSHelper.getBytes("f");
//		
//		for (int i = 0; i < 3; i++) {
//			FileHandle fh = new FileHandle();
//			
//			assert(DaisyDir.lookup(parent, file, fh) == FSHelper.DAISY_ERR_NOENT);
//			
//			assert(DaisyDir.creat(parent, file, fh) == FSHelper.DAISY_ERR_OK);
//			
//			assert(DaisyDir.lookup(parent, file, fh) == FSHelper.DAISY_ERR_OK);
//			
//			parent = fh;
//		}
		
//		String text = StringHelper.randomStr(Verify.random(10));
//		byte[] writeBytes = FSHelper.getBytes(text);
//		int offset = Verify.random(10);
//		
//		DaisyDir.write(fh, offset, writeBytes.length, writeBytes);
//		
//		byte[] readBytes = new byte[writeBytes.length];
//		DaisyDir.read(fh, offset, readBytes.length, readBytes);
//		
//		assert(writeBytes.equals(readBytes));
	}
}
