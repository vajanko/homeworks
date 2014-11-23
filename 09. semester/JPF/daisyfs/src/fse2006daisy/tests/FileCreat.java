package fse2006daisy.tests;

import fse2006daisy.FSHelper;
import fse2006daisy.StringHelper;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import gov.nasa.jpf.vm.Verify;

public class FileCreat {
	public static void main(String[] args) {
		
		FSHelper.init();
		
		FileHandle fh = new FileHandle();
		String file = StringHelper.randomStr(Verify.random(50));
		DaisyDir.creat(FSHelper.root, FSHelper.getBytes(file), fh);
	}
}
