package fse2006daisy.tests;

import fse2006daisy.FSHelper;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import gov.nasa.jpf.vm.Verify;

public class PathCreat {

	public static void main(String[] args) {
		
		FSHelper.init();
		
		FileHandle parent = FSHelper.root;
		
		byte[] file = new byte[1]; 
				//FSHelper.getBytes("f");
		
		for (int i = 0; i < Verify.random(20); i++) {
			FileHandle fh = new FileHandle();
			DaisyDir.creat(parent, file, fh);
			parent = fh;
		}
	}

}
