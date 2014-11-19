package fse2006daisy;

import gov.nasa.jpf.vm.Verify;

public class Producer extends Thread {
	private static final int MAX_DIRS = 2;
	private static final int MAX_FILES = 2;
	
	public void run() {
		while (true) {
			String path = "/file"; 
					//"/dir" + Verify.random(MAX_DIRS) + "/file" +Verify.random(MAX_FILES);
			
			FSHelper.writeStr(path, "dasdfasdfasdfasdfasdf");
			
		}
	}
}
