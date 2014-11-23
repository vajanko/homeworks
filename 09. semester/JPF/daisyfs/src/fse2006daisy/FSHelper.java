package fse2006daisy;

import fse2006daisy.daisy.Attribute;
import fse2006daisy.daisy.DaisyDir;
import fse2006daisy.daisy.FileHandle;
import fse2006daisy.daisy.Petal;
import gov.nasa.jpf.vm.Verify;

public class FSHelper {
	public static FileHandle root;
	public static void init() {
		root = new FileHandle();
		root.inodenum = 0;
		Petal.init(false);
	}
	
	public static final int DAISY_ERR_OK = -0;
	public static final int DAISY_ERR_NOENT = -2;
	public static final int DAISY_ERR_BADHANDLE = -2;
	public static final int DAISY_ERR_EXIST = -17;
	public static final int DAISY_ERR_FBIG = -27;
	public static final int DAISY_ERR_NOSPC = -28;
	
	public static byte[] getBytes(String s) {
		byte b[] = new byte[s.length()];
		for (int i = 0; i < s.length(); i++) {
			b[i] = (byte) s.charAt(i);
		}
		return b;
	}
	
	private static int createFile(FileHandle parent, byte[] name, FileHandle fh) {
		if (DaisyDir.lookup(parent, name, fh) == DAISY_ERR_NOENT)
			return DaisyDir.creat(parent, name, fh);
		
		return DAISY_ERR_OK;
	}
	public static int createFile(FileHandle parent, String name, FileHandle fh) {
		return createFile(parent, getBytes(name), fh);
	}
	public static FileHandle createPath(String path) {
		String[] parts = path.split("/");
		FileHandle parent = root;
		
		for (String name : parts) {
			
			if (name.equals(""))
				continue;
			
			FileHandle child  = new FileHandle();
			int res = createFile(parent, name, child); 
			if (res != DAISY_ERR_OK)
				return null;
				
			parent = child;
			
		}

		return parent;
	}

	public static void writeStr(FileHandle file, long offset, String str) {
		byte[] bytes = getBytes(str);
		DaisyDir.write(file, (int)offset, bytes.length, bytes);
	}
	
	public static long getSize(FileHandle fh) {
		Attribute a = new Attribute();
		DaisyDir.get_attr(fh, a);
		return a.size;
	}
	public static long getSize(String filename) {
		FileHandle fh = createPath(filename);
		return getSize(fh);
	}
	public static void writeStr(String filename, String str) {
		FileHandle fh = createPath(filename);
		writeStr(fh, 0, str);
	}
	public static void appendStr(String filename, String str) {
		FileHandle fh = createPath(filename);
		long size = getSize(fh);
		writeStr(fh, size, str);
	}
	
}
