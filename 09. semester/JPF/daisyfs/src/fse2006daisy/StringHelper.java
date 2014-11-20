package fse2006daisy;

import gov.nasa.jpf.vm.Verify;

public class StringHelper {
	
	public static String randomStr(int size) {
		Verify.ignoreIf(true);
		
		StringBuilder builder = new StringBuilder();
		for (int i =0; i < size; i++)
			builder.append('x');
		
		return builder.toString();
	}
	public static String randomPath(int length) {
		StringBuilder builder = new StringBuilder();
		for (int i = 0; i < length; i++) {
			builder.append('/');
			builder.append(randomStr(Verify.random(9)));
		}

		return builder.toString();
	}
}
