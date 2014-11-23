package fse2006daisy.tests;

import java.util.Arrays;

import fse2006daisy.FSHelper;
import fse2006daisy.StringHelper;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import gov.nasa.jpf.vm.Verify;

public class ReadWriteCheck {

	public static void main(String[] args) {
	
		FSHelper.init();
		
		FileHandle fh = new FileHandle();
		DaisyDir.creat(FSHelper.root, FSHelper.getBytes("f"), fh);
			
		int len = Verify.random(4);
		int offset = Verify.random(4);
		byte[] writeBytes = new byte[len];
		for (int i = 0; i < len; i++) {
			writeBytes[i] = (byte)Verify.random(4);
		}
		DaisyDir.write(fh, offset, writeBytes.length, writeBytes);
		
//		String text = StringHelper.randomStr(Verify.random(10));
//		byte[] writeBytes = FSHelper.getBytes(text);
//		int offset = Verify.random(10);
//		
//		DaisyDir.write(fh, offset, writeBytes.length, writeBytes);
		
		byte[] readBytes = new byte[writeBytes.length];
		DaisyDir.read(fh, offset, readBytes.length, readBytes);
		
		assert(Arrays.equals(writeBytes, readBytes));
	}

}
